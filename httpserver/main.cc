#include <iostream>
#include <string>
#include <memory>
#include "httpserver.hpp"// .hpp类的声明的定义可以放在一起

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
    signal(SIGPIPE, SIG_IGN);
    std::unique_ptr<HttpServer> httpserver(new HttpServer(port));
    httpserver->loop();

    return 0;
}
