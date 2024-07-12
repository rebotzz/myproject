#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

int main()
{
    cerr<<"read_from_pipe begin."<<endl;
    char buff[64] = {0};
    ssize_t s = read(0, buff, sizeof(buff) - 1);
    buff[s] = 0;
    cout<<"read_from_pipe read: "<<buff<<endl;
    cout<<"read_from_pipe exit"<<endl;
    return 0;
}