#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include "common.hpp"
using namespace std;

int main() {
    cerr<<"start cal cgi"<<endl;
    // 1.读取环境变量 2.读取参数
    string argument;
    CgiArgument cgiArgs;
    cgiArgs.readArgument(argument);

    // 3.处理数据
    string left, right;
    int x = 0, y = 0, ret = 0;
    char op = 0;
    bool valid = true;
    if(cutString(argument, "+", left, right)) op = '+';
    else if(cutString(argument, "-", left, right)) op = '-';
    else if(cutString(argument, "*", left, right)) op = '*';
    else if(cutString(argument, "/", left, right)) op = '/';
    else if(cutString(argument, "%", left, right)) op = '%';
    else valid = false;
    if(valid)
    {
        x = atoi(left.c_str());
        y = atoi(right.c_str());
        
        switch(op){
        case '+': ret = x + y; break;
        case '-': ret = x - y; break;
        case '*': ret = x * y; break;
        case '/': ret = x / y; break;
        case '%': ret = x % y; break;
        default: cerr<<"op error."<<endl; break;
        }        
    }

    // 4.输出结果
    if(valid)
    {
        argument += " = ";
        argument += to_string(ret);
        write(1, argument.c_str(), argument.size());
    }
    else{
        cerr<<"equal expressment error."<<endl;
        argument = "expressment error!";
        write(1, argument.c_str(), argument.size());
    }
   
    cerr<<"finish cal cgi." <<endl;
    return 0;
}
