#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include "utility.hpp"

#define LINE_END "\n"
#define WEB_ROOT "webroot"
#define HTTP_VERSION "HTTP/1.0"
#define HOME_PAGE "index.html"


// http请求
class HttpRequest
{
private:
    int _sockfd;
public:
    std::string _requestLine;
    std::vector<std::string> _requestHead;
    std::string _blank;
    std::string _requestBody;
    std::unordered_map<std::string, std::string> _headKV;

    std::string _method;
    std::string _url;
    std::string _version;
    std::string _query;
    int _contentLength;
private:


public:
    HttpRequest(int sockfd):_sockfd(sockfd),_contentLength(0)
    {}
    bool readRequest()
    {   // http/1.0 短链接, 超时处理    todo:http/1.1 长链接

        // 读取请求行
        if(Utility::readline(_sockfd, _requestLine) == false){
            return false;
        }
        _requestLine.resize(_requestLine.size() - 1);   // 去掉"\n"
        // 读取请求报头
        std::string line;
        while(Utility::readline(_sockfd, line) && line != LINE_END){
            line.resize(line.size() - 1);
            _requestHead.push_back(line);
            line.clear();
        }
        // 读取空行
        if(line != LINE_END) return false;
        _blank = line;
        return true;
    }
    void parseRequest()
    {   
        // 请求行 "GET /a/b/c.html HTTP/1.0"
        // auto endblank = _requestLine.rfind(" ");    //"GET /a/b/c.html"
        // Utility::cutString(_requestLine.substr(0, endblank), " ", _method, _url);
        std::stringstream ss(_requestLine);  
        ss >> _method >> _url >> _version;  // 默认空格分割
        for(char& ch : _method){
            ch = toupper(ch);
        }
        if(_method == "GET"){
            // 可能需要分离参数和url
            std::string path, args;
            if(Utility::cutString(_url, "?", path, args)){
                _url.swap(path);
                _query.swap(args);
            }
        }
        // 如果访问的是目录,默认访问该目录下的index.html
        if(_url.back() == '/'){
            _url += HOME_PAGE;
        }

        // 报头 "Content-Length: 123"
        for(auto& line : _requestHead){
            std::string key, value;
            Utility::cutString(line, ": ", key, value); //line已经去掉"\n"
            _headKV.insert({key, value});
        }
        if(_headKV.count("Content-Length")){
            _contentLength += atoi(_headKV["Content-Length"].c_str()); 
        }

        // for debug
        std::cout<<"method: "<<_method<<std::endl;
        std::cout<<"url: "<<_url<<std::endl;
        std::cout<<"content-length: "<<_contentLength<<std::endl;
        for(auto &[k, v] : _headKV){
            std::cout<<k<<": "<<v<<std::endl;
        }
    }

    bool isNeedReadBody()
    {
        if(_method == "POST" && _contentLength != 0) return true;
        else return false;
    }

    bool readBody()
    {
        // 分析请求,判断是否需要读取正文
        char ch = 0;
        int total = 0;
        while(total < _contentLength && read(_sockfd, &ch, sizeof(ch)) > 0){
            total++;
            _requestBody.push_back(ch);
        }

        return total == _contentLength ? true : false;
    }
};

// http响应
class HttpResponse
{
private:
    int _sockfd;
public:
    std::string _statusLine;
    std::vector<std::string> _responseHead;
    std::string _blank;
    std::string _responseBody;

    int _statusCode;
    int _contentLength;
    std::string _url;
    bool _cgiFlag;
private:
    enum StatusCode
    {
        OK = 200,
        NOT_FIND = 404
    };
    std::string stcode2desc(int status_code)
    {
        std::string ret;
        switch(status_code)
        {
        case OK: ret = "OK"; break;
        case NOT_FIND: ret = "NOT FIND"; break;
        default: ret = "NOT FIND"; break;
        }
        return ret;
    }
    bool buildResponseHead(const std::string& url)
    {
        std::string line;
        line += "Content-Length: ";
        line += std::to_string(_contentLength);
        line += LINE_END;
        _responseHead.push_back(line);

        line.clear();
        line += "Content-Type: ";
        std::string fileName, fileSuffix;
        if(Utility::cutString(url, ".", fileName, fileSuffix)){
            for(char& ch : fileSuffix){
                ch = tolower(ch);
            }
            if(fileSuffix == ".html") line += "text/html";
            else if(fileSuffix == ".js") line += "application/x-javascript";
            else if(fileSuffix == ".css") line += "text/css";
            else if(fileSuffix == ".png") line += "image/png";
            else if(fileSuffix == ".jpg") line += "image/jpeg";
            else line += "text/html";
        }
        else{
            line += "text/html";  // 没有后缀默认html
        }
        line += LINE_END;
        _responseHead.push_back(line);
        return true;
    }

    bool processNoncgi()
    {   // 这里似乎不需要正文,如果有正文数据,那么最好应该交给cgi处理
        
        // 构建响应报头 Content-Length: 123\nContent-Type: text/html\n
        if(_statusCode == 404){
            _url = WEB_ROOT;
            _url += "/404.html";
            struct stat fileStatus;
            memset(&fileStatus, 0, sizeof(fileStatus));
            if(stat(_url.c_str(), &fileStatus) != 0){
                LOG(ERROR, "stat file error, file: " + _url);
                return false;
            }
            _contentLength = fileStatus.st_size;
        }
        buildResponseHead(_url);
        // 构建空行
        _blank = LINE_END;

        // 构建正文 避免来回拷贝,直接sendfile
        _cgiFlag = false;
        return true;
    }
    bool processcgi(HttpRequest* httpRequest)
    {
        auto& method = httpRequest->_method;
        auto& url = httpRequest->_url;
        auto& requestBody = httpRequest->_requestBody;

        // 进程替换 + fd重定向 
        _contentLength = 0;

        // 将request body参数传递给子进程
        // get -> 环境变量, post -> 匿名管道

        // cgi程序处理数据

        // 从cgi读取结果,存入response body
        _cgiFlag = true;

        return true;
    }

    bool handlerOK()
    {}
    bool handlerError()
    {}
public:
    HttpResponse(int sockfd):_sockfd(sockfd),_statusCode(0),_contentLength(0), _cgiFlag(false)
    {}

    void buildResponse(HttpRequest* httpRequest)
    {
        _url = WEB_ROOT;
        _url += httpRequest->_url;
        LOG(INFO, "url: " + _url);

        // 查看url文件属性
        struct stat fileStatus;
        memset(&fileStatus, 0, sizeof(fileStatus));
        stat(_url.c_str(), &fileStatus);
        if(fileStatus.st_nlink == 0){
            LOG(ERROR, "url request resource not exists: " + _url); // 404
            _statusCode = 404;
        }
        else{
            _statusCode = 200; // 200 OK
            _contentLength = fileStatus.st_size;
        }

        // 构建状态行 http/1.0 200 OK
        _statusLine += "HTTP/1.0 ";
        _statusLine += std::to_string(_statusCode) + " ";
        _statusLine += stcode2desc(_statusCode);
        _statusLine += LINE_END;

        // 构建响应, cgi Noncgi
        if(_url.find("cgi") != std::string::npos && _statusCode != 404){
            processcgi(httpRequest);
        }
        else{
            processNoncgi();
        }
    }
    void sendResponse()
    {
        // 发送状态行
        if(send(_sockfd, _statusLine.c_str(), _statusLine.size(), 0) < 0){
            LOG(ERROR, "send error");
        }
        // 报头
        for(auto& line : _responseHead){
            if(send(_sockfd, line.c_str(), line.size(), 0) < 0){
                LOG(ERROR, "send error");
            }
        }
        // 空行
        send(_sockfd, _blank.c_str(), _blank.size(), 0);
        // 正文: noncgi / cgi
        if(_cgiFlag == true){  // cgi: 通过body返回数据
            if(send(_sockfd, _responseBody.c_str(), _responseBody.size(), 0) < 0){
                LOG(ERROR, "send _responseBody error");
            }

            LOG(DEBUG, "send by body, cgi");
        }
        else{
            int urlfd = open(_url.c_str(), O_RDONLY);
            if(urlfd < 0){
                LOG(ERROR, "open url error, file: " + _url);
                return;
            }
            ssize_t size = 0;
            if((size = sendfile(_sockfd, urlfd, nullptr, _contentLength)) < _contentLength){
                LOG(ERROR, "sendfile() error.");
            }

            LOG(DEBUG, "send by sendfile, noncgi");
            LOG(INFO, "sendfile : " + _url);
            LOG(INFO, "sendfile size: " + std::to_string(size));
        }
    }
};

class EndPoint
{

private:
    int _sockfd;
    HttpRequest* _httpRequest;
    HttpResponse* _httpResponse;
    bool _stop;
private: 


public:
    EndPoint(int sockfd):_sockfd(sockfd)
    {
        _httpRequest = new HttpRequest(_sockfd);
        _httpResponse = new HttpResponse(_sockfd);
    }
    bool handleRequest()
    {
        _httpRequest->readRequest();
        _httpRequest->parseRequest();
        _httpRequest->readBody();

        // for debug
        LOG(DEBUG, "receive request: ");
        std::cout<<_httpRequest->_requestLine;
        for(auto& headline : _httpRequest->_requestHead){
            std::cout<<headline;
        }
        std::cout<<_httpRequest->_blank;
        std::cout<<_httpRequest->_requestBody<<std::endl;

        return true;
    }
    bool buildResponse()
    {
        _httpResponse->buildResponse(_httpRequest);
        return true;
    }
    bool sendResponse()
    {
        _httpResponse->sendResponse();
        return true;
    }

    bool isStop()
    {
        return _stop;
    }

    ~EndPoint()
    {
        delete _httpRequest;
        delete _httpResponse;
    }
};


// http协议
class HttpProtocol
{
private:
    int _sockfd;
    HttpRequest* _httpRequest;
    HttpResponse* _httpResponse;
    bool _stop;
private: 


public:
    HttpProtocol(int sockfd):_sockfd(sockfd)
    {
        _httpRequest = new HttpRequest(_sockfd);
        _httpResponse = new HttpResponse(_sockfd);
    }
    bool handleRequest()
    {
        _httpRequest->readRequest();
        _httpRequest->parseRequest();
        _httpRequest->readBody();

        // for debug
        LOG(DEBUG, "receive request: ");
        std::cout<<_httpRequest->_requestLine;
        for(auto& headline : _httpRequest->_requestHead){
            std::cout<<headline;
        }
        std::cout<<_httpRequest->_blank;
        std::cout<<_httpRequest->_requestBody<<std::endl;

        return true;
    }
    bool buildResponse()
    {
        _httpResponse->buildResponse(_httpRequest);
        return true;
    }
    bool sendResponse()
    {
        _httpResponse->sendResponse();
        return true;
    }

    ~HttpProtocol()
    {
        delete _httpRequest;
        delete _httpResponse;
    }
};

class HttpServer
{
    typedef HttpServer Self;
    const static int backlog = 5;
private:
    static Self* _selfPtr;
    static pthread_mutex_t _sigleInstanceMutex;
    int _listensock;
    int _port;
    std::string _ip;
    bool _stop;

private:
    HttpServer(int port)
        :_listensock(-1),_port(port),_ip("0.0.0.0"), _stop(false)
    {
        // 初始化
        initialize();
    }
    HttpServer(const HttpServer& self) = delete;

    // 网络通讯准备工作
    void initialize()
    {
        //1.创建套接字,端口复用,绑定,监听
        if((_listensock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            LOG(FATAL, "create sock error!");
            exit(2);
        }
        int opt = 1;
        if(setsockopt(_listensock, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            LOG(ERROR, "reuse port error!");
        }
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        inet_aton(_ip.c_str(), &local.sin_addr);
        if(bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0){
            LOG(FATAL, "bind socket error!");
            exit(3);
        }
        if(listen(_listensock, backlog) < 0){
            LOG(FATAL, "listen socket error!");
            exit(4);
        }
        LOG(INFO, "socket initialized seccess.");
    }

    // 线程
    static void* routine(void* args)
    {
        // 线程分离,回收资源
        pthread_detach(pthread_self());
        int sockfd = *(int*)args;
        delete (int*)args;

        // 处理http请求
        std::unique_ptr<EndPoint> _httpHandler(new EndPoint(sockfd));
        _httpHandler->handleRequest();
        if(!_httpHandler->isStop()){
            _httpHandler->buildResponse();
            _httpHandler->sendResponse();
        }

        LOG(DEBUG, "handle http finish, close link.");
        // 关闭链接
        close(sockfd);
        return nullptr;
    }

public:
    static HttpServer* getInstance(int port)   // 单例
    {
        if(_selfPtr == nullptr)
        {
            pthread_mutex_lock(&_sigleInstanceMutex);
            if(_selfPtr == nullptr)
            {
                _selfPtr = new Self(port);
            }
            pthread_mutex_unlock(&_sigleInstanceMutex);
        }
        return _selfPtr;
    }

    // 循环
    void loop()
    {
        while(!_stop)
        {
            // 1.监听并建立链接
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            memset(&peer, 0, sizeof(peer));
            int sockfd = accept(_listensock, (struct sockaddr*)&peer, &len);
            if(sockfd < 0)
            {
                LOG(ERROR, "accept socket error!");
                continue;
            }
            LOG(INFO, "accept a new link, sockfd: " + std::to_string(sockfd));

            // 2.派发任务给新建线程
            pthread_t tid;
            pthread_create(&tid, nullptr, routine, (void*) new int(sockfd));
        }
    }

    ~HttpServer()
    {
        if(_listensock > 0)
        {
            close(_listensock);
        }
    }
};

HttpServer* HttpServer::_selfPtr = nullptr;
pthread_mutex_t HttpServer::_sigleInstanceMutex = PTHREAD_MUTEX_INITIALIZER;
