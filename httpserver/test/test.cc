#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;


bool processcgi(string bin, string _method, string _contentLength)
{
    // 0.将参数传递给子进程: get -> 环境变量, post -> 匿名管道
    std::string method = "REQUEST_METHOD=";
    method += _method;
    std::string contentLength = "CONTENT_LENGTH=";
    contentLength += _contentLength;
    if(putenv((char*)method.c_str()) != 0 || putenv((char*)contentLength.c_str()) != 0){
        perror("putenv error");
    }

    // 1.建立管道
    int input[2] = {0}, output[2] = {0};    // 父进程视角
    if(pipe(input) != 0 || pipe(output) != 0){
        perror("pipe create failed");
        return false;
    }
    // 2.创建子进程
    pid_t id = fork();
    if(id < 0){
        perror("fork failed");
        return false;
    }
    else if(id == 0){ // child
        // input[1] 写, output[0] 读
        close(input[0]);
        close(output[1]);

        // 3.fd重定向 + 进程替换 
        dup2(input[1], STDOUT_FILENO);
        dup2(output[0], STDIN_FILENO);
        execl(bin.c_str(), bin.c_str(), nullptr);  //cgi程序处理数据
        printf("\n-----------------execl failed---------------------\n");
        exit(4);
    }
    else{ // father
        // input[0] 读, output[1] 写
        close(input[1]);
        close(output[0]);  

        // 传递数据给cgi
        string requestBody;
        if(!requestBody.empty()){
            const char* start = requestBody.c_str();
            ssize_t size = 0, total = 0;
            while(total < requestBody.size() && (size = write(output[1], start + total, requestBody.size() - total)) > 0){
                total += size;
            }
            if(total != requestBody.size()){
                perror("write to child process failed");
                return false;
            }
        }

        // 从cgi读取结果,存入response body
        string responseBody;
        responseBody = "<!DOCTYPE html> <head> <meta charset=\"utf-8\"> </head>";
        responseBody += "<body>";
        char buff[1024] = {0};
        ssize_t s = 0;
        while((s = read(input[0], buff, sizeof(buff) - 1)) > 0){
            buff[s] = 0;
            responseBody += buff;
            memset(buff, 0, s);
        }
        responseBody += "</body>";
        responseBody += "</html>";

        int status = 0;
        if(waitpid(id, &status, 0) != id){
            perror("wait child process failed");
            return false;
        }
        if(WIFEXITED(status)){
            cout<<"body: "<<responseBody;
            return true;
        }
        else {
            responseBody = "<!DOCTYPE html>\n <head> <meta charset=\"utf-8\"> </head>\n";
            responseBody += "<body>\n";
            responseBody += "Server Error\n";
            responseBody += "</body>\n";
            responseBody += "</html>\n";
            
            cout<<"body: "<<responseBody;
            return false;
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        cout<<"usage: "<<argv[0]<<" filepath"<<endl;
        return -1;
    }

    // string filepath = argv[1];
    // int fd = open(filepath.c_str(), O_RDONLY);
    // if(fd < 0){
    //     perror("open file error.");
    // }

    // struct stat st;
    // int ret = stat(filepath.c_str(), &st);
    // if(ret != 0){
    //     perror("stat file error.");
    //     return 1;
    // }
    // if(st.st_nlink != 0){
    //     cout<<"文件存在"<<endl;
    //     cout<<"文件大小: "<<st.st_size<<endl;
    // }

    // string a = "123abc";
    // cout<<a.back()<<endl;

    processcgi(argv[1], "GET", "1256");
    cout<<"run over."<<endl;
    
    return 0;
}