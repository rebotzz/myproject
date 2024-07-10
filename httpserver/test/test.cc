// #include <iostream>
// #include <pthread.h>
// #include <memory>
// #include "threadPool.hpp"
// #include "tcpserver.hpp"

// using std::cout;
// using std::endl;
// class A
// {
// public:
//     void operator()()
//     {
//         cout<<"A"<<endl;
//     }
// };

// int main()
// {
//     // ThreadPool<int>* ptr = ThreadPool<int>::getInstance(3);
//     // ptr->pushTask();
//     // ptr->~ThreadPool(); 
//     // delete ptr;


//     // {
//     //     cout<<"begin"<<endl;
//     //     std::unique_ptr<ThreadPool<A>> pt(ThreadPool<A>::getInstance(3), std::default_delete<ThreadPool<A>::Delete()>());
//     //     cout<<"end"<<endl;
//     // }

//     // ThreadPool<A>* pt = ThreadPool<A>::getInstance(2);
//     // printf("%p\n", pt);

//     // // pt->~ThreadPool();
//     // // pt->destroy();
//     // delete pt;
//     // printf("%p\n", pt);

//     using namespace std;
//     // unique_ptr<Tcpserver> tcp(Tcpserver::getInstance());
//     Tcpserver* tcp = Tcpserver::getInstance();
//     // delete tcp;  
//     // tcp->~Tcpserver();

//     // delete在在释放空间之前会先调用析构函数, 所以不用处理

//     return 0;
// }


#include <iostream>
#include <string>
#include "../include/tcpserver.hpp"
#include <signal.h>
using namespace std;


int sockfd = -1;
void sigalrm_hander(int sig)
{
    close(sockfd);
    sockfd = -1;
    cout<<"alarm, and close fd."<<endl;
}
void sigpipe_hander(int sig)
{
    cout<<"get a signal pipe"<<endl;
}

int main()
{
    // 建立连接
    auto* tcp = Tcpserver::getInstance();
    string peer;
    sockfd = tcp->Accepter(peer);
    signal(SIGPIPE, sigpipe_hander);

    // 定时器,关闭文件
    signal(SIGALRM, sigalrm_hander);
    alarm(10);
    cout<<"开始计时: 10s"<<endl;
    while(true)
    {
        char buff[128] = {0};
        ssize_t s = read(sockfd, buff, sizeof(buff) - 1);
        if(s > 0) buff[s] = 0;
        if(s <= 0) break;

        cout<<"read: "<<buff<<endl;
        alarm(10);
        cout<<"重新开始计时: 5s"<<endl;
    }

    cout<<"程序退出"<<endl;

    delete tcp;
    return 0;
}