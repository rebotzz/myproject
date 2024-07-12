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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include "utility.hpp"

#define LINE_END "\n"
#define WEB_ROOT "webroot"
#define HTTP_VERSION_1_0 "HTTP/1.0"
#define HTTP_VERSION_1_1 "HTTP/1.1"
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
    bool isNeedReadBody()
    {
        if(_method == "POST" && _contentLength != 0) return true;
        else return false;
    }

public:
    HttpRequest(int sockfd):_sockfd(sockfd),_contentLength(0)
    {}
    bool readHead()
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

    bool readBody()
    {
        if(isNeedReadBody())
        {
            // 读取正文
            char ch = 0;
            int total = 0;
            while(total < _contentLength && read(_sockfd, &ch, sizeof(ch)) > 0){
                total++;
                _requestBody.push_back(ch);
            }
            if(total != _contentLength) return false;
        }

        return true;
    }

    void parseRequest()
    {   
        // 请求行 "GET /a/b/c.html HTTP/1.0"
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
        // std::cout<<"method: "<<_method<<std::endl;
        // std::cout<<"url: "<<_url<<std::endl;
        // std::cout<<"content-length: "<<_contentLength<<std::endl;
        // std::cout<<"query: "<<_query<<std::endl;
        // for(auto &[k, v] : _headKV){
        //     std::cout<<k<<": "<<v<<std::endl;
        // }
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

    std::string _path;
    std::string _suffix;
    int _fd;
    int _statusCode;
    int _contentLength;
public:
    enum StatusCode
    {
        OK = 200,
        Moved_Permanently = 301,
        Found = 302,
        Forbidden = 403,
        Not_Find = 404,
        Internal_Server_Error = 500
    };
    
public:
    HttpResponse(int sockfd):_sockfd(sockfd),_statusCode(OK),_contentLength(0)
    {}

    std::string stcode2desc()
    {
        std::string ret;
        switch(_statusCode)
        {
        case OK: ret = "OK"; break;
        case Moved_Permanently: ret = "Moved Permanently"; break;
        case Found: ret = "Found"; break;
        case Forbidden: ret = "Forbidden"; break;
        case Not_Find: ret = "Not Find"; break;
        case Internal_Server_Error: ret = "Internal Server Error"; break;
        default: ret = "Not Find"; break;
        }
        return ret;
    }
    std::string suffix2desc(const std::string& fileSuffix)
    {
        std::string line;
        if(fileSuffix == "html") line = "text/html";
        else if(fileSuffix == "js") line = "application/x-javascript";
        else if(fileSuffix == "css") line = "text/css";
        else if(fileSuffix == "png") line = "image/png";
        else if(fileSuffix == "jpg") line = "image/jpeg";
        else if(fileSuffix == "ico") line = "image/x-icon";
        else line = "text/html";// 没有后缀默认html
        return line;
    }
};

class EndPoint
{
private:
    int _sockfd;
    HttpRequest _httpRequest;
    HttpResponse _httpResponse;
    std::string _http_version;
    bool _stop;
    bool _cgi;

private: 
    void buildResponseStatusLine()
    {
        // 构建状态行 http/1.0 200 OK
        _httpResponse._statusLine += _http_version;
        _httpResponse._statusLine += " ";
        _httpResponse._statusLine += std::to_string(_httpResponse._statusCode);
        _httpResponse._statusLine += " ";
        _httpResponse._statusLine += _httpResponse.stcode2desc();
        _httpResponse._statusLine += LINE_END;
    }

    void buildResponseHead()
    {
        std::string line;
        line += "Content-Length: ";
        line += std::to_string(_httpResponse._contentLength);
        line += LINE_END;
        _httpResponse._responseHead.push_back(line);

        line.clear();
        line += "Content-Type: ";
        line += _httpResponse.suffix2desc(_httpResponse._suffix);
        line += LINE_END;
        _httpResponse._responseHead.push_back(line);
    }

    bool processNoncgi()
    {   
        // 构建响应报头 Content-Length: 123\nContent-Type: text/html\n
        if(_httpResponse._statusCode == 404){
            handlerError();
            return false;
        }
        buildResponseHead();
        // 构建空行
        _httpResponse._blank = LINE_END;

        // 构建正文 避免来回拷贝,直接sendfile
        _httpResponse._fd = open(_httpResponse._path.c_str(), O_RDONLY);
        if(_httpResponse._fd < 0){
            LOG(ERROR, "open file error, file: " + _httpResponse._path);
            return false;
        }
        return true;
    }
    bool processcgi()
    {
        auto& bin = _httpResponse._path;
        // LOG(DEBUG, "processcgi begin.");
        // LOG(DEBUG, "bin: " + bin);
        // 0.将参数传递给子进程: get -> 环境变量, post -> 匿名管道
        std::string method = "REQUEST_METHOD=";
        method += _httpRequest._method;
        std::string contentLength = "CONTENT_LENGTH=";
        contentLength += std::to_string(_httpRequest._contentLength); // 请求正文的长度
        std::string query = "QUERY_STRING=";
        query += _httpRequest._query.size() != 0 ? _httpRequest._query : "NULL";
        if(putenv((char*)method.c_str()) != 0 
        || putenv((char*)contentLength.c_str()) != 0
        || putenv((char*)query.c_str()) != 0){
            LOG(ERROR, "putenv failed");
            _httpResponse._statusCode = HttpResponse::Internal_Server_Error;
            handlerError();
            return false;
        }

        // 1.建立管道,在fork之前
        int input[2] = {0}, output[2] = {0};    // 父进程视角
        if(pipe(input) != 0 || pipe(output) != 0){
            LOG(ERROR, "pipe create failed");
            _httpResponse._statusCode = HttpResponse::Internal_Server_Error;
            handlerError();
            return false;
        }
        // 2.创建子进程
        pid_t id = fork();
        if(id < 0){
            LOG(ERROR, "fork failed");
            _httpResponse._statusCode = HttpResponse::Internal_Server_Error;
            handlerError();
            return false;
        }
        else if(id == 0){ // child
            // input[1] 写, output[0] 读
            close(input[0]);
            close(output[1]);

            // 3.fd重定向 + 进程替换 
            dup2(output[0], STDIN_FILENO);
            dup2(input[1], STDOUT_FILENO);
            execl(bin.c_str(), bin.c_str(), nullptr);  //cgi程序处理数据
            std::cerr<<"\nexecl failed\n"<<std::endl;
            exit(4);    // 子进程退出,fd自动回收
        }
        else{ // father
            // input[0] 读, output[1] 写
            close(input[1]);
            close(output[0]);  

            // 传递数据给cgi
            auto& requestBody = _httpRequest._requestBody;
            if(!requestBody.empty()){
                const char* start = requestBody.c_str();
                ssize_t size = 0, total = 0;
                while(total < requestBody.size() && (size = write(output[1], start + total, requestBody.size() - total)) > 0){
                    total += size;
                }
                if(total != requestBody.size()){
                    LOG(ERROR, "write to child process failed");
                    _httpResponse._statusCode = HttpResponse::Internal_Server_Error;
                    handlerError();
                    return false;
                }
            }

            // 从cgi读取结果,存入response body
            auto& responseBody = _httpResponse._responseBody;
            responseBody = "<!DOCTYPE html>\n<head> <meta charset=\"utf-8\"> </head>\n";
            responseBody += "<body>\n";
            char buff[1024] = {0};
            ssize_t s = 0;
            while((s = read(input[0], buff, sizeof(buff) - 1)) > 0){
                buff[s] = 0;
                responseBody += buff;
                memset(buff, 0, s);
            }
            responseBody += "\n</body>\n";
            responseBody += "</html>";
            _httpResponse._contentLength = responseBody.size();
            // 判断子进程运行状况
            int status = 0;
            pid_t ret = waitpid(id, &status, 0);
            if(ret == id && WIFEXITED(status)){
                LOG(INFO, "child process exit normally.");
            }
            else {
                if(ret != id) LOG(ERROR, "wait child process failed");
                else if(WIFSIGNALED(status)) LOG(ERROR, "child process was killed by signal.");
                else LOG(ERROR, "other error for child process.");
                responseBody = "<!DOCTYPE html>\n <head> <meta charset=\"utf-8\"> </head>\n";
                responseBody += "<body>\n";
                responseBody += "<h1>Internal Server Error!</h1>";
                responseBody += "\n</body>\n";
                responseBody += "</html>";
                _httpResponse._contentLength = responseBody.size();
                _httpResponse._statusCode = HttpResponse::Internal_Server_Error;
            }
        }
        _httpResponse._suffix = "html";
        buildResponseHead();
        _httpResponse._blank = LINE_END;

        // 回收资源,关闭fd
        close(input[0]);
        close(output[1]);
        return true;
    }

    bool handlerOK()
    {
        return true;
    }

    bool handlerError()
    {
        if(404 == _httpResponse._statusCode)    // 404,返回404.html send file
        {
            _httpResponse._path = WEB_ROOT;
            _httpResponse._path += "/404.html";
            struct stat fileStatus;
            memset(&fileStatus, 0, sizeof(fileStatus));
            stat(_httpResponse._path.c_str(), &fileStatus);
            _httpResponse._contentLength = fileStatus.st_size;
            // 构建正文 避免来回拷贝,直接sendfile
            _httpResponse._fd = open(_httpResponse._path.c_str(), O_RDONLY);
            if(_httpResponse._fd < 0){
                LOG(ERROR, "open file error, file: " + _httpResponse._path);
                return false;
            }
        }
        else   // 其他错误,返回错误码描述   send body
        {
            auto& responseBody = _httpResponse._responseBody;
            responseBody = "<!DOCTYPE html>\n <head> <meta charset=\"utf-8\"> </head>\n";
            responseBody += "<body>\n";
            responseBody += "<h1>";
            responseBody += _httpResponse.stcode2desc();
            responseBody += "!</h1>";
            responseBody += "\n</body>\n";
            responseBody += "</html>";
            _httpResponse._contentLength = responseBody.size();
        }

        _httpResponse._suffix = "html";
        buildResponseHead();
        _httpResponse._blank = LINE_END;
        return true;
    }
public:
    EndPoint(int sockfd, std::string http_version = HTTP_VERSION_1_0)
    :_sockfd(sockfd),_httpRequest(sockfd),_httpResponse(sockfd)
    ,_stop(false),_cgi(false),_http_version(http_version)
    {}

    bool handleRequest()
    {
        if(!_httpRequest.readHead()) {
            _stop = true;
            return false;
        }
        _httpRequest.parseRequest();
        if(!_httpRequest.readBody()){
            _stop = true;
            return false;
        }
        // if(_httpRequest._method == "POST" || !_httpRequest._query.empty() || !_httpRequest._requestBody.empty()){
        //     _cgi = true;
        // }
        std::string filename;
        Utility::cutString(_httpRequest._url, ".", filename, _httpResponse._suffix);
        
        return true;
    }
    bool buildResponse()
    {
        _httpResponse._path = WEB_ROOT;
        _httpResponse._path += _httpRequest._url;
        LOG(INFO, "path: " + _httpResponse._path);
        if(_httpRequest._url.find("..") != std::string::npos){
            // 非法请求
            _httpResponse._statusCode = HttpResponse::Forbidden;
            buildResponseStatusLine();
            handlerError();
            return false;
        }

        // 查看url文件属性
        struct stat fileStatus;
        memset(&fileStatus, 0, sizeof(fileStatus));
        if(stat(_httpResponse._path.c_str(), &fileStatus) != 0){
            LOG(ERROR, "url request resource not exists: " + _httpResponse._path); // 404
            _httpResponse._statusCode = 404;
        }
        else{
            LOG(INFO, "url exists, file: " + _httpResponse._path);
            if(S_ISDIR(fileStatus.st_mode)){    // 如果是目录文件
                _httpResponse._path += "/"; 
                _httpResponse._path += HOME_PAGE; 
                if(stat(_httpResponse._path.c_str(), &fileStatus) != 0){
                    _httpResponse._statusCode = 404;
                } 
                else{
                    _httpResponse._statusCode = HttpResponse::Moved_Permanently;
                    _httpResponse._contentLength = fileStatus.st_size;
                }
            }
            else if(S_IXUSR & fileStatus.st_mode || S_IXGRP & fileStatus.st_mode || S_IXOTH & fileStatus.st_mode){
                _cgi = true;    // 如果是可执行文件
            }
            else{   // 如果是普通文件
                _httpResponse._statusCode = 200; // 200 OK
                _httpResponse._contentLength = fileStatus.st_size;
            }
        }

        // 构建状态行 http/1.0 200 OK
        buildResponseStatusLine();

        // 构建报头+空行+正文, cgi / Noncgi
        if(_cgi && _httpResponse._statusCode != 404){
            processcgi();
        }
        else{
            processNoncgi();
        }
        return true;
    }
    bool sendResponse()
    {
        auto& _statusLine = _httpResponse._statusLine;
        auto& _responseHead = _httpResponse._responseHead;
        auto& _responseBody = _httpResponse._responseBody;

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
        send(_sockfd, _httpResponse._blank.c_str(), _httpResponse._blank.size(), 0);
        // 正文: noncgi / cgi(通过body返回数据)
        if(true == _cgi){ 
            ssize_t size = 0, total = 0;
            const char* start = _responseBody.c_str();
            while(total < _responseBody.size() && (size = send(_sockfd, start + total, _responseBody.size() - total, 0)) > 0){
                total += size;
            }
            if(total != _responseBody.size()) {
                LOG(ERROR, "send _responseBody error");
                return false;
            }
            // LOG(DEBUG, "send body by cgi");
        }
        else{
            ssize_t size = 0;
            if((size = sendfile(_sockfd, _httpResponse._fd, nullptr, _httpResponse._contentLength)) < _httpResponse._contentLength){
                LOG(ERROR, "sendfile error.");
                return false;
            }
            close(_httpResponse._fd);
            // LOG(DEBUG, "sendfile by noncgi");
            LOG(INFO, "sendfile : " + _httpResponse._path + "size: " + std::to_string(size));
        }

        return true;
    }

    bool isStop()
    {
        return _stop;
    }

    ~EndPoint()
    {}
};



