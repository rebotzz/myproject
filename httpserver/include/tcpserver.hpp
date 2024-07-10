#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include "utility.hpp"

class Tcpserver
{
    #define PORT 8081
    #define BACKLOG 5
    typedef Tcpserver Self;
private:
    int _listensock;
    int _port;
    std::string _ip;

    static pthread_mutex_t _singleInstanceMutex;
    static Self* _instance;
private:
    // 网络通讯准备工作
    Tcpserver(int port, const std::string& ip):_listensock(-1),_port(port),_ip(ip)
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
        if(_ip == "0.0.0.0")
            local.sin_addr.s_addr = INADDR_ANY; 
        else
            inet_aton(_ip.c_str(), &local.sin_addr);
        if(bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0){
            LOG(FATAL, "bind socket error!");
            exit(3);
        }
        if(listen(_listensock, BACKLOG) < 0){
            LOG(FATAL, "listen socket error!");
            exit(4);
        }
        LOG(INFO, "socket initialized seccess.");
    }
public:
    static Self* getInstance(int port = PORT, const std::string& ip = "0.0.0.0")   // 单例
    {
        if(_instance == nullptr)
        {
            pthread_mutex_lock(&_singleInstanceMutex);
            if(_instance == nullptr)
            {
                _instance = new Self(port, ip);
            }
            pthread_mutex_unlock(&_singleInstanceMutex);
        }
        return _instance;
    }

    int listensockfd()
    {
        return _listensock;
    }

    int Accepter(std::string& ip)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        memset(&peer, 0, sizeof(peer));
        int sockfd = accept(_listensock, (struct sockaddr*)&peer, &len);
        if(sockfd < 0){
            LOG(ERROR, "accept socket error!");
            return -1;
        }
        char buff[32] = {0}; 
        inet_ntop(AF_INET, reinterpret_cast<void *>(&peer.sin_addr.s_addr), buff, sizeof(buff));
        ip = buff;
        return sockfd;
    }

    ~Tcpserver()
    {
        if(_listensock > 0){
            close(_listensock);
        }
    }

};
pthread_mutex_t Tcpserver::_singleInstanceMutex = PTHREAD_MUTEX_INITIALIZER;
Tcpserver* Tcpserver::_instance = nullptr;