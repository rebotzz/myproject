#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>

#define REQUEST_METHOD "REQUEST_METHOD"
#define CONTENT_LENGTH "CONTENT_LENGTH"
#define QUERY_STRING "QUERY_STRING"

#define LOG(logLevel, message) Log(#logLevel, message, __FILE__, __LINE__)  // 宏函数
void Log(std::string loglevel, std::string msg, std::string file, int line)
{
    // [日志级别][时间][日志描述][文件][行号]

    time_t timestamp = time(nullptr);
    struct tm* caltime = localtime(&timestamp);
    std::string timestr = std::to_string(caltime->tm_year + 1900) + "-";
    timestr += std::to_string(caltime->tm_mon + 1) + "-";
    timestr += std::to_string(caltime->tm_mday) + " ";
    timestr += std::to_string(caltime->tm_hour) + ":";
    timestr += std::to_string(caltime->tm_min) + ":";
    timestr += std::to_string(caltime->tm_sec);
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

class CgiArgument
{
private:
    std::string _method, _contentLength, _queryString;
private:
    // 读取环境变量
    void readEnv()
    {
        char * envPtr = nullptr;
        _method = (envPtr = getenv(REQUEST_METHOD)) == nullptr ? "" : envPtr;
        _contentLength = (envPtr = getenv(CONTENT_LENGTH)) == nullptr ? "" : envPtr;
        _queryString = (envPtr = getenv(QUERY_STRING)) == nullptr ? "" : envPtr;
    }
public:
    // 读取参数
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

