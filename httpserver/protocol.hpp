#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
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
    std::unordered_map<std::string, std::string> _requestHeadMap;

    std::string _method;
    std::string _url;
    int _content_length;
private:
    std::string& readline(std::string& line)
    {
        char ch = 0;
        while(read(_sockfd, &ch, sizeof(ch)) > 0)
        {
            // 不同平台换行符不同: "\n" "\r\n" "\r"  -> 统一处理为"\n"
            if(ch == '\r'){
                // 探测下一个字符是不是'\n'
                char next = 0;
                recv(_sockfd, &next, sizeof(next), MSG_PEEK); // MSG_PEEK 缓冲区队列不移除读取的
                if(next == '\n'){
                    read(_sockfd, &ch, sizeof(ch));
                }
                else{
                    ch = '\n';
                }

                line.push_back(ch);
                break;
            }
            else if(ch == '\n'){
                line.push_back(ch);
                break;
            }
            line.push_back(ch);
        }

        std::cout<< "read line: "<< line;
        return line;
    }

public:
    HttpRequest(int sockfd):_sockfd(sockfd),_content_length(0)
    {}
    void readRequest()
    {   // http/1.0 短链接, 超时处理    todo:http/1.1 长链接

        // 读取请求行
        readline(_requestLine);
        // 读取请求报头
        std::string line;
        while(readline(line) != "\n"){
            _requestHead.push_back(line);
            line.clear();
        }
        // 读取空行
        _blank = line;
    }
    void parseRequest()
    {   
        // 请求行 "GET /a/b/c.html HTTP/1.0"
        auto endblank = _requestLine.rfind(" ");    //"GET /a/b/c.html"
        Utility::cutString(_requestLine.substr(0, endblank), " ", _method, _url);
        for(char& ch : _method){
            ch = toupper(ch);
        }
        if(_method == "GET"){
            // 可能需要分离参数和url
            std::string uri, args;
            if(Utility::cutString(_url, "?", uri, args)){
                _url.swap(uri);
                _requestBody = std::move(args);  // 参数统一放在正文处理 ->todo:如果有别的正文,需要分隔
                _content_length = _requestBody.size();
            }
        }
        // 如果访问的是目录,默认访问该目录下的index.html
        if(_url.back() == '/'){
            _url += "index.html";
        }

        // 报头 "Content-Length: 123"
        for(auto& line : _requestHead){
            std::string key, value;
            Utility::cutString(line, ": ", key, value);
            value.pop_back();    // 去掉"\n"
            _requestHeadMap.insert({key, value});
        }
        if(_requestHeadMap.count("Content-Length")){
            _content_length += atoi(_requestHeadMap["Content-Length"].c_str()); 
        }


        // for debug
        std::cout<<"method: "<<_method<<std::endl;
        std::cout<<"url: "<<_url<<std::endl;
        std::cout<<"content-length: "<<_content_length<<std::endl;
        for(auto &[k, v] : _requestHeadMap){
            std::cout<<k<<": "<<v<<std::endl;
        }
    }

    void readBody()
    {
        // 分析请求,判断是否需要读取正文
        char ch = 0;
        int total = 0;
        while(total < _content_length && read(_sockfd, &ch, sizeof(ch)) > 0){
            total++;
            _requestBody.push_back(ch);
        }
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

    int _status_code;
    int _content_length;
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
        line += std::to_string(_content_length);
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
        if(_status_code == 404){
            _url = WEB_ROOT;
            _url += "/404.html";
            struct stat fileStatus;
            memset(&fileStatus, 0, sizeof(fileStatus));
            if(stat(_url.c_str(), &fileStatus) != 0){
                LOG(ERROR, "stat file error, file: " + _url);
                return false;
            }
            _content_length = fileStatus.st_size;
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
        _content_length = 0;

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
    HttpResponse(int sockfd):_sockfd(sockfd),_status_code(0),_content_length(0), _cgiFlag(false)
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
            _status_code = 404;
        }
        else{
            _status_code = 200; // 200 OK
            _content_length = fileStatus.st_size;
        }

        // 构建状态行 http/1.0 200 OK
        _statusLine += "HTTP/1.0 ";
        _statusLine += std::to_string(_status_code) + " ";
        _statusLine += stcode2desc(_status_code);
        _statusLine += LINE_END;

        // 构建响应, cgi Noncgi
        if(_url.find("cgi") != std::string::npos && _status_code != 404){
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
            if((size = sendfile(_sockfd, urlfd, nullptr, _content_length)) < _content_length){
                LOG(ERROR, "sendfile() error.");
            }

            LOG(DEBUG, "send by sendfile, noncgi");
            LOG(INFO, "sendfile : " + _url);
            LOG(INFO, "sendfile size: " + std::to_string(size));
        }
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
        std::unique_ptr<HttpProtocol> _httpHandler(new HttpProtocol(sockfd));
        _httpHandler->handleRequest();
        _httpHandler->buildResponse();
        _httpHandler->sendResponse();

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
