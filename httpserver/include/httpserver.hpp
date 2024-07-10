#pragma once
#include <signal.h>
#include "protocol.hpp"
#include "tcpserver.hpp"
#include "threadPool.hpp"

// HttpServer 1.0版本,为每一个新链接创建一个新线程
#ifdef HTTP_SERVER_1_0
class HttpServer
{
    typedef HttpServer Self;
private:
    int _port;
    std::string _ip;
    bool _stop;

private:
    HttpServer(const HttpServer& self) = delete;

    // 线程
    static void* routine(void* args)
    {
        // 线程分离,回收资源
        pthread_detach(pthread_self());

        // 忽略管道信号,防止程序退出
        signal(SIGPIPE, SIG_IGN);
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
    HttpServer(int port, const std::string& ip = "0.0.0.0")
        :_port(port),_ip(ip), _stop(false)
    {}

    // 循环
    void loop()
    {
        std::unique_ptr<Tcpserver> tcp(Tcpserver::getInstance(_port, _ip));
        int listensock = tcp->listensockfd();
        while(!_stop)
        {
            // 1.监听并建立链接
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            memset(&peer, 0, sizeof(peer));
            int sockfd = accept(listensock, (struct sockaddr*)&peer, &len);
            if(sockfd < 0)
            {
                LOG(ERROR, "accept socket error!");
                continue;
            }
            std::cout<<"\n\n---------begin-------------"<<std::endl;
            LOG(INFO, "accept a new link, sockfd: " + std::to_string(sockfd));

            // 2.派发任务给新建线程
            pthread_t tid;
            pthread_create(&tid, nullptr, routine, (void*) new int(sockfd));
            // 这里每个线程使用的sockfd不同,send时不会相互影响
            // pthread_join(tid, nullptr);
        }
    }

    ~HttpServer()
    {}
};
#endif


// HttpServer 1.1版本:线程池,生产者消费者模型
class HttpServer
{
    typedef HttpServer Self;
private:
    int _port;
    std::string _ip;
    bool _stop;

private:
    HttpServer(const HttpServer& self) = delete;

    class Task
    {
    public:
        int _sockfd = -1;
        Task() = default;
        Task(int sockfd):_sockfd(sockfd)
        {}
        void operator()()
        {
            if(_sockfd < 0) return;
            routine(&_sockfd);
        }
    };
    // 线程
    static void* routine(void* args)
    {
        int sockfd = *(int*)args;

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
    HttpServer(int port, const std::string& ip = "0.0.0.0")
        :_port(port),_ip(ip), _stop(false)
    {}

    // 循环
    void loop()
    {
        std::unique_ptr<Tcpserver> tcp(Tcpserver::getInstance(_port, _ip));
        std::unique_ptr<ThreadPool<Task>> threadPool(ThreadPool<Task>::getInstance());
        while(!_stop)
        {
            // 1.监听并建立链接
            std::string peerIP;
            int sockfd = tcp->Accepter(peerIP);
            if(sockfd < 0){
                continue;
            }
            std::cout<<"\n\n---------begin-------------"<<std::endl;
            LOG(INFO, "accept a new link, ip: " + peerIP + ", sockfd: " + std::to_string(sockfd));

            // 2.派发任务给线程
            threadPool->pushTask(Task(sockfd));
        }
    }

    ~HttpServer()
    {}
};