#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "utility.hpp"


// http请求
class HttpRequest
{
private:
    int _sockfd;
    std::string _buff;
public:
    std::string _requestLine;
    std::string _requestHead;
    std::string _blank;
    std::string _requestBody;
    std::unordered_map<std::string, std::string> _requestHeadMap;

public:
    HttpRequest(int sockfd):_sockfd(sockfd)
    {}
    void readRequest()
    {
        // http/1.0 短链接, 超时处理    todo:http/1.1 长链接
    }
    void parseRequest()
    {}

};

// http响应
class HttpResponse
{
private:
    int _sockfd;
public:
    std::string _statusLine;
    std::string _responseHead;
    std::string _blank;
    std::string _responseBody;

public:
    HttpResponse(int sockfd):_sockfd(sockfd)
    {

    }

    void buildResponse()
    {}
    void sendResponse()
    {

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
        return true;
    }
    bool buildResponse()
    {
        _httpResponse->buildResponse();
        return true;
    }
    bool sendResponse()
    {
        _httpResponse->sendResponse();
        return true;
    }

    ~HttpProtocol()
    {

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
