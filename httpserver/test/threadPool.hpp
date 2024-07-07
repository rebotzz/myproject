#pragma once

#include "lockGuard.hpp"
#include "logMessage.hpp"
#include "thread.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <pthread.h>

int g_threadNum = 3;

// 1.0 POSIX原生线程库
#define _POSIX_THRAED_POOL_
#ifdef _POSIX_THRAED_POOL_

// 线程池
// 本质上：生产者消费者模型
// 1.生产者(主线程) 任务队列 消费者(其余线程)
template <class T> // 任务类型：T
class ThreadPool
{
private:
    // 这里需要static吗? 类函数第一个默认参数this指针，线程函数只有一个参数，故需要
    // 线程函数是static，只能使用static成员;但是可以通过传递this指针访问成员函数/变量
    static void *routine(void *args)
    {
        // 线程执行流
        ThreadData *td = (ThreadData *)args;
        ThreadPool<T> *tp = (ThreadPool<T> *)td->_args;
        while (true)
        {
            T task;
            {
                // 加锁,代码块，结束自动析构
                LockGuard lockguard(tp->getMutex());
                // 判断条件
                while (tp->taskIsEmpty()) // if->while
                {
                    logMessage(DEBUG, "file: %s, line: %d, 任务队列为空,wait...", __FILE__, __LINE__);
                    tp->waitCond(); 
                }
                // 获取任务
                logMessage(DEBUG, "file: %s, line: %d, 获取任务", __FILE__, __LINE__);
                tp->popTask(task);
            } // 解锁

            // 执行任务
            logMessage(DEBUG, "file: %s, line: %d, %s 执行任务", __FILE__, __LINE__, td->_name.c_str());
            logMessage(NOTICE, "%s 执行任务",td->_name.c_str());
            task();
        }

        return nullptr;
    }

    bool taskIsEmpty()
    {
        return _qTask.empty();     // 单个队列
    }

    void waitCond()
    {
        pthread_cond_wait(&_cond, &_mtx);
    }

    void wakeCond()
    {
        pthread_cond_signal(&_cond);
    }

    pthread_mutex_t *getMutex()
    {
        return &_mtx;
    }

private:
    ThreadPool(const ThreadPool<T> &tp) = delete;
    ThreadPool<T> &operator=(const ThreadPool<T> &tp) = delete;

    // private构造函数，限制拷贝构造，复制重载  目的：单例 --只能实例化一个对象
    ThreadPool(int threadNum = g_threadNum)
        : _threadNum(threadNum), _thread(_threadNum) // 调用vector构造
    {
        // 初始化锁，条件变量
        // pthread_mutex_init(&_mtx, nullptr);  // --static
        pthread_cond_init(&_cond, nullptr);
        logMessage(NOTICE, "file: %s, line: %d, 初始化锁，条件变量", __FILE__, __LINE__);

        // new线程
        for (int i = 0; i < _threadNum; ++i)
        {
            _thread[i] = new Thread(i + 1, routine, this /*??*/); // 所以，这里传入this指针
            logMessage(DEBUG, "file: %s, line: %d, new了一个线程 %s", __FILE__, __LINE__, _thread[i]->name());
        }
    }

public:
    // 懒汉模式的单例获取
    static ThreadPool<T> *getInstance(int threadNum = g_threadNum)
    {
        // 双检查加锁
        // 第二次进入nullptr != _tp_ptr, 避免申请锁
        if (nullptr == _tp_ptr)
        {
            // 避免多个线程同时调用，加锁 --但是锁不是static
            LockGuard lockguard(&_mtx);
            if (nullptr == _tp_ptr)
            {
                _tp_ptr = new ThreadPool<T>(threadNum);
            }
        }

        return _tp_ptr;
    }

    ~ThreadPool()
    {
        // 回收线程资源
        for (const auto &it : _thread)
        {
            delete it; // 会自动调用析构吗？？会！
        }

        // pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_cond);
    }

    void run()
    {
        for (const auto &it : _thread)
        {
            it->start();
            //logMessage(DEBUG, "file: %s, line: %d, run thread", __FILE__, __LINE__);
            // 线程分离,线程结束自动回收资源
            pthread_detach(it->getTid());
        }
    }

    // 1.0 使用一个任务队列
    void pushTask(const T task)
    {
        LockGuard lockguard(&_mtx);
        _qTask.push(task);
        wakeCond();
    }

    void popTask(T &task)
    {
        // debug:获取任务在临界区，不用再加锁，避免自锁
        task = _qTask.front();
        _qTask.pop();
    }

private:
    // 线程类数组，任务队列，线程数量，线程数据
    int _threadNum;
    std::vector<Thread *> _thread;
    std::queue<T> _qTask;
    pthread_cond_t _cond;
    // pthread_mutex_t _mtx; // 锁任务队列  --非单例时

    // 单例
    static ThreadPool<T> *_tp_ptr;
    static pthread_mutex_t _mtx; // 锁任务队列+单例创建  --单例时
};
template <typename T>
ThreadPool<T> *ThreadPool<T>::_tp_ptr = nullptr;
template <typename T>
pthread_mutex_t ThreadPool<T>::_mtx = PTHREAD_MUTEX_INITIALIZER; // 自动销毁

#else

// 2.0 C++的线程库
#include <thread>
#include <mutex>
#include <condition_variable>
// 线程池
// 本质上：生产者消费者模型
// 1.生产者(主线程) 任务队列 消费者(其余线程)
int g_task_num = 5; // 任务推送执行最小个数

template <class T> // 任务类型：T
class ThreadPool
{
public:
    ~ThreadPool()
    {
        for (auto &t : _thd)
        {
            t.join();
        }
    }

    // 核心接口:run, push,pop
    // 开始运行所有线程
    void run()
    {
        for (auto &t : _thd)
        {
            t = std::thread(routine, this);
        }
    }

    // 生产者
    void pushTask(const T &task)
    {
        // 锁
        std::unique_lock<std::mutex> lock(_mtx);
        _pq.push(task);

        // 如果任务到一定数量，一次性推送
        if (_pq.size() >= g_task_num)
        {
            _pq.swap(_cq);
            _cv.notify_all(); // 唤醒任务执行线程
        }
    }

    // 消费者
    void popTask(T &task)
    {
        // 锁？  调用在锁内，不用
        task = _cq.front();
        _cq.pop();
    }

    // 单例 --基于懒汉模式
    static ThreadPool<T> *getInstance(int tNum = g_threadNum)
    {
        // c++11 之后写法
        static ThreadPool<T> tp(tNum);
        return &tp;
    }

private:
    ThreadPool(int tNum = g_threadNum)
        : _tNum(tNum), _thd(tNum) // vector开辟空间初始化
    {
    }
    ThreadPool(const ThreadPool<T> &tp) = delete;
    ThreadPool<T> &operator=(const ThreadPool<T> &tp) = delete;

    // 线程运行函数 参数：任务队列，锁，条件变量
    static void routine(ThreadPool<T> *tp)
    {
        while (true)
        {
            T task;
            {
                std::unique_lock<std::mutex> lock(tp->_mtx);
                while (tp->_cq.empty()) // 检测资源就绪情况
                {
                    tp->_cv.wait(lock);
                }

                // 获取任务
                tp->popTask(task); // 在锁内部
            }

            // 执行任务
            task();
        }
    }

private:
    // 线程个数，线程数组，任务队列，锁，条件变量
    int _tNum;
    std::vector<std::thread> _thd;
    std::queue<T> _cq;
    std::queue<T> _pq;
    std::mutex _mtx; // 自动初始化,销毁
    std::condition_variable _cv;

    // static ThreadPool<T>* _ptr;
};

// template<class T>
// ThreadPool<T>* ThreadPool<T>::_ptr = nullptr;

#endif