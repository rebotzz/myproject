#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

#define REQUEST_METHOD "REQUEST_METHOD"
#define CONTENT_LENGTH "CONTENT_LENGTH"
#define QUERY_STRING "QUERY_STRING"

int main() {
    cerr<<"this is cgi"<<endl;
    // cerr<<"test path env: "<<getenv("PATH")<<endl;

    string method, contentLength, query;
    method = getenv(REQUEST_METHOD) == nullptr ? "" : getenv(REQUEST_METHOD);
    contentLength = getenv(CONTENT_LENGTH) == nullptr ? "" : getenv(CONTENT_LENGTH);
    query = getenv(QUERY_STRING) == nullptr ? "" : getenv(QUERY_STRING);
    if(!method.empty())
        cout<< "REQUEST_METHOD getenv: "<< method<<endl;
    if(!contentLength.empty())
        cout<< "CONTENT_LENGTH getenv: "<< contentLength<<endl;
    if(!query.empty())
        cout<< "QUERY_STRING getenv: "<< query<<endl;
        
    cerr<<"bye cgi." <<endl;

    return 0;
}
