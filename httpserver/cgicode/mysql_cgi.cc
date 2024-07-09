#include <iostream>
#include "./mysql/include/mysql.h"
#include "common.hpp"
using namespace std;

int main()
{
    cout<<"test mysql"<<endl;
    cout<<mysql_get_client_version()<<endl;
    return 0;
}
