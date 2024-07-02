#include <iostream>
#include <string>
#include <memory>
#include "protocol.hpp" // .hpp类的声明的定义可以放在一起
#include "utility.hpp"

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
    std::unique_ptr<HttpServer> httpserver(HttpServer::getInstance(port));
    httpserver->loop();

    return 0;
}
