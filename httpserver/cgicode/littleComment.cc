#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>
#include "common.hpp"

using namespace std;

int main()
{
    // 留言板功能cgi code...
    // 1.读取环境变量 2.读取参数
    string argument;
    CgiClass cgiArgs;
    cgiArgs.readArgument(argument); // 包括正文
    int contentLen = atoi(cgiArgs._contentLength.c_str());

    cout<<"这里是: 留言板功能cgi"<<endl;
    cout<<"read: "<<argument<<endl;

    cerr<<"留言板功能cgi read: "<<argument<<endl;
    cerr<<"留言板功能cgi 结束, bye."<<endl;
    return 0;
}


