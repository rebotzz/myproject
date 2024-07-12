#include <iostream>
#include <string>
#include <cstdlib>
#include "./mysql/include/mysql.h"
#include "common.hpp"
using namespace std;

bool mysqlQuery(const string& sql_query)
{
    // 0.连接数据库
    MYSQL* conn = mysql_init(nullptr);
    mysql_set_character_set(conn, "utf8"); //utf8mb4
    if(nullptr == mysql_real_connect(conn, "127.0.0.1", "http_test", "********","http_test",8436, nullptr, 0))
    {
        cerr<<"mysql connect failed."<<endl;
        exit(2);
    }
    cerr<<"query: "<<sql_query<<endl;
    // 1.切换数据库
    mysql_query(conn, "use http_test;");
    // 2.执行sql语句
    int ret = mysql_query(conn, sql_query.c_str());
    cerr<<"sql result: "<<ret<<endl;
    // 3.断开连接数据库
    mysql_close(conn);
    if(ret == 0)
        return true;
    else 
        return false;
}

int main()
{
    cerr<<"test mysql"<<endl;
    cerr<<mysql_get_client_version()<<endl;
    // // 1.读取参数
    string argument;
    CgiArgument cgi;
    cgi.readArgument(argument);
    // for debug
    // cout<<"input sql: ";
    // cin>>argument;

    // 注册功能
    string name_kv, passwd_kv;
    string name_k, name_v, passwd_k, passwd_v;
    if(!cutString(argument,"&",name_kv, passwd_kv)
    || !cutString(name_kv, "=", name_k, name_v) 
    || !cutString(passwd_kv, "=",passwd_k, passwd_v))
    {
        cerr<<"mysql query string is wrong."<<endl;
        return 1;
    }
    // 2.执行sql语句
    string sql_query, ret;
    //"insert into user (name, passwd) values ('李四', md5('jffsd78'));";
    sql_query += "insert into user (name, passwd) values ";
    sql_query += "(\'" + name_v + "\', " + "md5(\'" + passwd_v + "\'));"; 
    if(mysqlQuery(sql_query) == false){
        cerr<<"execute mysql query failed."<<endl;
        ret = "注册失败!";
    }
    else{
        ret = "注册成功!";
    }
    
    // 3.输出结果
    string output;
    output += "<html>\n<head> <meta charset=\"utf-8\"> </head>";
    output += "<body>\n<h1>";
    output += ret;
    output +="</h1>\n</body>\n</html>";
    cout<<output<<endl;

    return 0;
}
