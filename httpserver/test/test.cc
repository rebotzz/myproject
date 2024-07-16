#include <iostream>




#if 0
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

int test_sInstance_delete()
{
    ThreadPool<int>* ptr = ThreadPool<int>::getInstance(3);
    ptr->pushTask();
    ptr->~ThreadPool(); 
    delete ptr;


    {
        cout<<"begin"<<endl;
        std::unique_ptr<ThreadPool<A>> pt(ThreadPool<A>::getInstance(3), std::default_delete<ThreadPool<A>::Delete()>());
        cout<<"end"<<endl;
    }

    ThreadPool<A>* pt = ThreadPool<A>::getInstance(2);
    printf("%p\n", pt);

    // pt->~ThreadPool();
    // pt->destroy();
    delete pt;
    printf("%p\n", pt);

    using namespace std;
    unique_ptr<Tcpserver> tcp(Tcpserver::getInstance());
    Tcpserver* tcp = Tcpserver::getInstance();
    delete tcp;  
    tcp->~Tcpserver();

    // delete在在释放空间之前会先调用析构函数, 所以不用处理

    return 0;
}


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

// int main()
// {
//     // 建立连接
//     auto* tcp = Tcpserver::getInstance();
//     string peer;
//     sockfd = tcp->Accepter(peer);
//     // signal(SIGPIPE, sigpipe_hander);
//     signal(SIGPIPE, SIG_IGN);   // 就算忽略SIGPIPE, 关闭fd后,read结束阻塞

//     // 定时器,关闭文件
//     signal(SIGALRM, sigalrm_hander);
//     alarm(10);
//     cout<<"开始计时: 10s"<<endl;
//     while(true)
//     {
//         char buff[128] = {0};
//         ssize_t s = read(sockfd, buff, sizeof(buff) - 1);
//         if(s > 0) buff[s] = 0;
//         if(s <= 0) break;

//         cout<<"read: "<<buff<<endl;
//         alarm(10);
//         cout<<"重新开始计时: 5s"<<endl;
//     }

//     cout<<"程序退出"<<endl;

//     delete tcp;
//     return 0;
// }

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
// 测试管道通讯

void test_pipe()
{
    int pipefd[2] = {0};
    pipe(pipefd);
    pid_t pid;
    pid = fork();
    if (pid == -1)
        perror("fork error");
    if (pid != 0) {
        cout<<"father"<<endl;
        close(pipefd[0]);
        write(pipefd[1], "hello", 5);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    }
    else{
        close(pipefd[1]);
        char buf[10] = {0};
        read(pipefd[0], buf, 10);
        printf("child buf=%s\n", buf);
    }
}


bool test_comment_cgi()
{
    std::string method = "REQUEST_METHOD=POST";
    std::string contentLength = "CONTENT_LENGTH=123";
    std::string query = "QUERY_STRING=NULL";
    if(putenv((char*)method.c_str()) != 0 
    || putenv((char*)contentLength.c_str()) != 0
    || putenv((char*)query.c_str()) != 0)
    {
        perror("putenv error.");
        return false;
    }
    cout<<"put env done!"<<endl;
    return true;
}

// int main()
// {
//     int fdpipe[2];
//     pipe(fdpipe);


//     // if(!test_comment_cgi()) return -1;
//     std::string method = "REQUEST_METHOD=POST";
//     std::string contentLength = "CONTENT_LENGTH=123";
//     std::string query = "QUERY_STRING=NULL";
//     if(putenv((char*)method.c_str()) != 0 
//     || putenv((char*)contentLength.c_str()) != 0
//     || putenv((char*)query.c_str()) != 0)
//     {
//         perror("putenv error.");
//         return false;
//     }
//     cout<<"put env done!"<<endl;

//     // string bin = "./read";
//     string bin = "./littleComment";

//     pid_t id = fork();  // debug: fork在管道建立之后
//     if(id < 0){
//         return -1;
//     }
//     if(id == 0){
//         // child read
//         // 是不是因为管道一个以只读方式打开,所以出错?
//         close(fdpipe[1]);


//         dup2(fdpipe[0], 0);
//         // execlp("ls", "ls", nullptr);
//         // sleep(1);
//         cerr<<"子进程替换"<<endl;
//         execl(bin.c_str(), bin.c_str(), nullptr);
//         cerr<<"进程替换失败"<<endl;

//         // char buff[32] = {0};
//         // ssize_t s = read(fdpipe[0], buff, sizeof(buff) - 1);
//         // if(s > 0) cout<<"child read: "<<buff<<endl;
//         // else cout<<"read failed"<<endl;
//         exit(2);
//     }

//     // parent
//     close(fdpipe[0]);
//     cout<<"父进程 do write"<<endl;
//     char str[] = "hello world.";
//     ssize_t s = write(fdpipe[1], str, sizeof(str));
//     if(s > 0) cout<<"write :"<<str<<endl;
//     else cout<<"write failed"<<endl;

//     waitpid(id, nullptr, 0);
//     cout<<"after wait, 父进程 exit"<<endl;
//     return 0;
// }

#endif

#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <atomic>
using namespace std;
#define FILE_PATH "./comment.txt"

int main()
{
    // 留言板功能cgi code...
    // 1.读取环境变量 2.读取参数

    // cerr<<"这里是: 留言板功能cgi"<<endl;

    // int fd = open(FILE_PATH, O_CREAT | O_APPEND | O_WRONLY, 0666);
    // if(fd < 0) {
    //     printf("留言板文件打开错误.");
    //     return 1;
    // }

    // string argument = "test comment write file.\n你好, 你能看见吗?\n";
    // write(fd, argument.c_str(), argument.size());
    // close(fd);


    // cerr<<"留言板功能cgi read: "<<argument<<endl;
    // cerr<<"留言板功能cgi 结束, bye."<<endl;

    
    
    return 0;
}


