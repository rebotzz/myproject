#include <iostream>
#include <string>
#include <memory>
#include "./include/httpserver.hpp"// .hpp类的声明的定义可以放在一起
#include "./include/myDaemom.hpp"
// http服务端程序

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout<<"Usage: \n\t"<<argv[0]<<" port"<<std::endl;
        exit(1);
    }
    int port = atoi(argv[1]);

    LOG(INFO, "start httpserver.");
    daemon(1, 0);   // 守护进程
    // MyDaemon();

    signal(SIGPIPE, SIG_IGN); // 忽略管道信号,防止程序退出
    std::unique_ptr<HttpServer> httpserver(new HttpServer(port));
    httpserver->loop();

    return 0;
}
