#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class Sock
{
public:
    int Socket()
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0)
        {
            
        }
    }
    int Listen()
    {}
    int Connect()
    {}
    int Accepter()
    {}
    int SetSockOpt(int sock)
    {}



};