#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>

#define REQUEST_METHOD "REQUEST_METHOD"
#define CONTENT_LENGTH "CONTENT_LENGTH"
#define QUERY_STRING "QUERY_STRING"

#define LOG(logLevel, message) Log(#logLevel, message, __FILE__, __LINE__)  // 宏函数
std::string gettime();
void Log(std::string loglevel, std::string msg, std::string file, int line)
{
    // [日志级别][时间][日志描述][文件][行号]

    std::string timestr = gettime();
    std::cout<<"["<<loglevel<<"]["<<timestr<<"]["<<msg<<"]["<<file<<"]["<<line<<"]"<<std::endl;
}
enum LOG_LEVEL
{
    DEBUG,
    INFO,
    WARING,
    ERROR,
    FATAL
};

std::string gettime()
{
    time_t timestamp = time(nullptr);
    struct tm* caltime = localtime(&timestamp);
    int year = caltime->tm_year + 1900, month = caltime->tm_mon + 1, day = caltime->tm_mday;
    int hour = caltime->tm_hour, minute = caltime->tm_min, second = caltime->tm_sec;
    char buff[32] = {0};
    snprintf(buff, sizeof(buff), "%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);

    return buff;
}

bool cutString(const std::string& target, const std::string& separator, std::string& out1, std::string& out2)
{   
    auto pos = target.find(separator);
    if(pos != std::string::npos){
        out1 = target.substr(0, pos);
        out2 = target.substr(pos + separator.size());
        return true;
    }
    return false;
}

class CgiClass
{
public:
    std::string _method, _contentLength, _queryString;
public:
    // 读取环境变量
    void readEnv()
    {
        char * envPtr = nullptr;
        _method = (envPtr = getenv(REQUEST_METHOD)) == nullptr ? "" : envPtr;
        _contentLength = (envPtr = getenv(CONTENT_LENGTH)) == nullptr ? "" : envPtr;
        _queryString = (envPtr = getenv(QUERY_STRING)) == nullptr ? "" : envPtr;
    }
public:
    // 读取参数: 包括正文(如果body存在)
    bool readArgument(std::string& argument_out)
    {
        readEnv();
        if(_method == "POST"){
            int length = atoi(_contentLength.c_str());
            argument_out.resize(length);
            int total = 0, size = 0;
            while(total < length && (size = read(0, &argument_out[0], length - total)) > 0){
                total += size;
            }
            if(total != length) {
                LOG(ERROR, "CGI Read Argument Error");
                return false;
            } 
        }
        else if(_method == "GET"){
            argument_out = _queryString;
        }
        else{
            // ...
        }
        return true;
    }

};

