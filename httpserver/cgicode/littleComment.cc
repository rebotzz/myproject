#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "common.hpp"

using namespace std;
#define FILE_PATH "./comment.txt"

int main()
{
    // 留言板功能cgi code...
    // 1.读取环境变量 2.读取参数
    string argument;
    CgiClass cgiArgs;
    cgiArgs.readArgument(argument); // 包括正文

    cerr<<"这里是: 留言板功能cgi"<<endl;
    cerr<<"read: "<<argument<<endl;

    int fd = open(FILE_PATH, O_CREAT | O_APPEND | O_WRONLY, 0666);  //debug: 666 -> 0666
    if(fd < 0) {
        LOG(ERROR, "留言板文件打开错误.");
        return 1;
    }

    // string out = "[" + gettime() + "] " + argument + "\n";
    write(fd, argument.c_str(), argument.size());
    write(fd, "\n", sizeof("\n"));
    close(fd);


    cerr<<"留言板功能cgi read: "<<argument<<endl;
    cerr<<"留言板功能cgi 结束, bye."<<endl;
    return 0;
}


