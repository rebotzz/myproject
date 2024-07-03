#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

int main(int argc, char* argv[])
{
    // if(argc != 2){
    //     cout<<"usage: "<<argv[0]<<" filepath"<<endl;
    //     return -1;
    // }

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

    string a = "123abc";
    cout<<a.back()<<endl;
    return 0;
}