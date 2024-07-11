#include <iostream>
#include <string>
#include <cstdlib>
#include "common.hpp"
using namespace std;

int main() {
    cerr<<"this is cgi"<<endl;

    // string method, contentLength, query;
    // readEnv(method, contentLength, query);

    // if(!method.empty())
    //     cout<< "REQUEST_METHOD getenv: "<< method<<endl;
    // if(!contentLength.empty())
    //     cout<< "CONTENT_LENGTH getenv: "<< contentLength<<endl;
    // if(!query.empty())
    //     cout<< "QUERY_STRING getenv: "<< query<<endl;
    // else{
    //     cerr<<"not find QUERY_STRING"<<endl;
    //     cout<<"not find QUERY_STRING"<<endl;
    // }
    string args;
    CgiArgument().readArgument(args);
    cerr<<"read arguments: "<<args<<endl;
        
    cerr<<"bye cgi." <<endl;

    return 0;
}
