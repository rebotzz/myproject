#include <iostream>
#include <pthread.h>
#include <memory>
#include "threadPool.hpp"
#include "tcpserver.hpp"

using std::cout;
using std::endl;
class A
{
public:
    void operator()()
    {
        cout<<"A"<<endl;
    }
};

int main()
{
    // ThreadPool<int>* ptr = ThreadPool<int>::getInstance(3);
    // ptr->pushTask();
    // ptr->~ThreadPool(); 
    // delete ptr;


    // {
    //     cout<<"begin"<<endl;
    //     std::unique_ptr<ThreadPool<A>> pt(ThreadPool<A>::getInstance(3), std::default_delete<ThreadPool<A>::Delete()>());
    //     cout<<"end"<<endl;
    // }

    // ThreadPool<A>* pt = ThreadPool<A>::getInstance(2);
    // printf("%p\n", pt);

    // // pt->~ThreadPool();
    // // pt->destroy();
    // delete pt;
    // printf("%p\n", pt);

    using namespace std;
    // unique_ptr<Tcpserver> tcp(Tcpserver::getInstance());
    Tcpserver* tcp = Tcpserver::getInstance();
    // delete tcp;  
    // tcp->~Tcpserver();

    // delete在在释放空间之前会先调用析构函数, 所以不用处理

    return 0;
}