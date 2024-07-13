#pragma once
#include <signal.h>
#include <list>
#include <unordered_map>
#include <atomic>
#include "protocol.hpp"
#include "tcpserver.hpp"
#include "threadPool.hpp"

// HttpServer 0.1版本,为每一个新链接创建一个新线程
#ifdef HTTP_SERVER_0_1
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


// HttpServer 0.2版本,线程池 
// 短链接:一旦处理完成立刻断开连接;优点:避免长时间占用.缺点:重复TCP连接建立,消耗资源
#ifdef HTTP_SERVER_0_2
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
    // 线程例程
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
#endif



// HttpServer 0.3版本,线程池+长链接: 建立TCP连接后维持连接,以便后续通讯. 缺点:长时间占用
#define HTTP_SERVER_0_3
#ifdef HTTP_SERVER_0_3
// HttpServer 1.1版本:线程池,生产者消费者模型
class HttpServer
{
    typedef HttpServer Self;
    class Task;
private:
    int _port;
    std::string _ip;
    bool _stop;                 // HttpServer退出标志位
    std::list<Task> _tasks;     // 任务列表
    std::unordered_map<std::string, int> _peers; // k-v: ip-port 用于记录已经建立TCP连接的对端,避免重复建立连接 

private:
    HttpServer(const HttpServer& self) = delete;

    class Task
    {
    public:
        std::string _ip;    // todo: 加锁
        int _sockfd = -1;
        // 为了指向同一份数据: 临界资源  ubantu20测试结果: atomic int/bool无锁的  
        std::shared_ptr<std::atomic_int> _timeoutPtr; // 60s连接对端没有发送信息,close(sockfd)
        std::shared_ptr<std::atomic_bool> _alivePtr;   // 用于连接延时管理, 不同线程判断任务是否存活

        Task() = default;
        Task(std::string ip, int sockfd, std::shared_ptr<std::atomic_int> timeoutPtr, std::shared_ptr<std::atomic_bool> alivePtr)
            :_ip(ip), _sockfd(sockfd), _timeoutPtr(timeoutPtr), _alivePtr(alivePtr)
        {}
        void operator()()
        {
            if(_sockfd < 0) return;
            routine(this);
        }
    };
    // 线程例程
    static void* routine(void* args)
    {
        Task* task = (Task*) args;
        int sockfd = task->_sockfd;

        // 处理http请求
        std::unique_ptr<EndPoint> _httpHandler(new EndPoint(sockfd, HTTP_VERSION_1_0));
        // http/1.1 维持长链接
        // while(_httpHandler->handleRequest())    // 阻塞式recv, 通过close(fd)控制连接结束
        _httpHandler->handleRequest();    // 长链接有bug, 退回短链接
        {
            *(task->_timeoutPtr) = 60;                // 重置连接结束倒计时; 临界资源
            // LOG(DEBUG, "sockfd: [" + std::to_string(task->_sockfd) + "] 重置timeout");
            if(!_httpHandler->isStop()){
                _httpHandler->buildResponse();
                _httpHandler->sendResponse();
            }
        }

        LOG(DEBUG, "sockfd: [" + std::to_string(task->_sockfd) + "] handle http finish, close link.");
        // 关闭链接, 任务结束
        if(*task->_alivePtr){
            close(sockfd);
            *(task->_alivePtr) = false; // 让linkManager结束管理
        }

        return nullptr;
    }
    // 长链接超时管理
    static void * linkManager(void *args)
    {
        pthread_detach(pthread_self());
        HttpServer* serv = (HttpServer*)args;
        auto& tasks = serv->_tasks;
        while(!serv->_stop)
        {
            sleep(1);
            while(!tasks.empty())
            {
                sleep(1);
                // 遍历所有连接,检查超时情况
                auto iter = tasks.begin();
                while(iter != tasks.end()){
                    // LOG(DEBUG, "sockfd: [" + std::to_string(iter->_sockfd) + "] timeout剩余时间: " + std::to_string(*iter->_timeoutPtr));
                    // 如果在timeout时间内没有通讯,断开连接,移除任务列表
                    if((*(iter->_timeoutPtr))-- < 0 || *iter->_alivePtr == false){ // 临界资源
                        if(iter->_sockfd > 0 && (*iter->_alivePtr)) {
                            close(iter->_sockfd);
                            *iter->_alivePtr = false;   // 避免别的线程重复close


                            // todo: 线程安全加锁
                            auto pos = serv->_peers.find(iter->_ip);
                            if(pos != serv->_peers.end())
                                serv->_peers.erase(pos);
                        }
                        LOG(DEBUG, "sockfd: [" + std::to_string(iter->_sockfd) + "] timeout! ");
                        iter = tasks.erase(iter);
                    }
                    else {
                        ++iter;
                    }
                }
            }
        }

        return nullptr;
    }
public:
    HttpServer(int port, const std::string& ip = "0.0.0.0")
        :_port(port),_ip(ip), _stop(false)
    {}

    // 循环
    void loop()
    {
        // 初始化:TCP连接, 线程池, 长链接管理
        std::unique_ptr<Tcpserver> tcp(Tcpserver::getInstance(_port, _ip));
        std::unique_ptr<ThreadPool<Task>> threadPool(ThreadPool<Task>::getInstance(10)); //线程数量
        pthread_t tid;
        // pthread_create(&tid, nullptr, linkManager, this);   

        // for test
        // std::atomic_int aa = 1;
        // std::atomic_bool bb = 1;
        // using std::cout;
        // using std::endl;
        // cout<<"atomic_int is lock free: "<<aa.is_lock_free()<<endl;
        // cout<<"atomic_bool is lock free: "<<aa.is_lock_free()<<endl;

        while(!_stop)
        {
            // 1.监听并建立链接
            std::string peerIP;
            int sockfd = tcp->Accepter(peerIP);
            if(sockfd < 0){
                continue;
            }
            // // 如果已经建立连接了,就拒绝同一个对端多个连接  不行,浏览器多线程
            // if(_peers.count(peerIP)){
            //     LOG(DEBUG, "相同peer,拒绝连接. " + peerIP);
            //     close(sockfd);
            //     continue;
            // }
            // _peers[peerIP] = sockfd;  // todo 断开连接时删除

            std::cout<<"\n\n---------begin-------------"<<std::endl;
            std::string peer = peerIP + ":" + std::to_string(sockfd);
            LOG(INFO, "accept a new link. " + peer);

            // 2.派发任务给线程
            Task newTask(peerIP, sockfd, std::make_shared<std::atomic_int>(60), std::make_shared<std::atomic_bool>(true));
            // _tasks.push_back(newTask);      
            threadPool->pushTask(newTask);  // 两份拷贝,不管拷贝几份,都指向同一shared_ptr
        }
    }

    ~HttpServer()
    {}
};
#endif
