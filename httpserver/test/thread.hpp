#ifndef __THREAD_HPP_
#define __THREAD_HPP_
#include <pthread.h>
#include <string>
#include <iostream>
#include "logMessage.hpp"

typedef void*(*func_t)(void *);
class ThreadData
{
public:
    ThreadData() = default;
    ThreadData(const ThreadData& td):_name(td._name),_args(td._args)
    {}
public:
    std::string _name;
    void *_args;
};

// 封装原生线程库
class Thread
{
public:
    Thread(int nameNum, func_t fun, void *arg)
        :_fun(fun)
    {
        _tData._args = arg;
        _tData._name = "thread ";
        _tData._name += std::to_string(nameNum);
    }
    ~Thread()
    {
        // std::cout<<"~Thread()"<<std::endl;
        // pthread_join(_tid, nullptr);   //自己join自己，会不会很怪？不算自己join，不在线程调用函数内
    }

    void start()
    {
        pthread_create(&_tid, nullptr, _fun, (void*)&_tData);
        logMessage(DEBUG,"file: %s, line: %d, 创建了一个线程:%s, tid: %p",
        __FILE__, __LINE__, _tData._name.c_str(), _tid);
        logMessage(NOTICE,"%s init", _tData._name.c_str());
    }

    const char* name()
    {
        return _tData._name.c_str();
    }

    pthread_t getTid()  //pthread_t 本质:unsigned long int --也对应地址
    {
        return _tid;
    }
private:
    pthread_t _tid;
    func_t _fun;
    ThreadData _tData;      // 包括：_arg,_name
};

#endif 