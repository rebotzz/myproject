#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "utility.hpp"

// 线程池 生产者消费者模型
template<class Task>
class ThreadPool
{
    typedef ThreadPool<Task> Self;
private:
    int _threadNum;
    std::queue<Task> _taskQueue;
    std::vector<pthread_t> _threads;
    bool _stop = false; // 控制多个线程退出

    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    static pthread_mutex_t _singleInstanceMutex;
    static Self* _instance;
private:
    ThreadPool(int threadNum = 6):_threadNum(threadNum)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);

        for(int i = 0; i < _threadNum; ++i)
        {
            pthread_t tid;  // debug: 不是pid_t, 是pthread_t
            if(pthread_create(&tid, nullptr, routine, this) != 0){
                LOG(FATAL, std::to_string(i) + " thread create failed.");
                continue;
            }
            _threads.push_back(tid);
            // LOG(DEBUG, "create a new thread " + std::to_string(i));
        }
        LOG(INFO, "threads pool initialize seccess.");
    }
    ThreadPool(Self& _self) = delete;
    Self& operator=(Self& _self) = delete;

    static void* routine(void* args)
    {
        ThreadPool<Task>* tp = (ThreadPool<Task>*)args;
        // pthread_detach(pthread_self());

        while(!tp->_stop)
        {
            // 1.获取任务
            pthread_mutex_lock(&tp->_mutex);
            while(tp->_taskQueue.empty()){
                // LOG(DEBUG, "tash queue empty, thread wait.");
                pthread_cond_wait(&tp->_cond, &tp->_mutex);
                if(tp->_stop && tp->_taskQueue.empty()){  // 收到接收通知&&任务为空,退出线程
                    pthread_mutex_unlock(&tp->_mutex);
                    // LOG(DEBUG, "thread exit");
                    return nullptr;
                }
            }
            // LOG(DEBUG, "thread wake up");
            Task task;
            tp->popTask(task);
            pthread_mutex_unlock(&tp->_mutex);

            // 2.执行任务
            // LOG(DEBUG, "thread: " + std::to_string(pthread_self()) + ", sockfd: " + std::to_string(task._sockfd));
            task();
        }

        // LOG(DEBUG, "thread exit");
        return nullptr;
    }
    bool popTask(Task& task)
    {
        if(!_taskQueue.empty()){
            task = _taskQueue.front();
            _taskQueue.pop();
            return true;
        }
        return false;
    }
public:
    static Self* getInstance(int threadNum = 6)
    {
        if(nullptr == _instance)
        {
            pthread_mutex_lock(&_singleInstanceMutex);
            if(nullptr == _instance){
                _instance = new ThreadPool<Task>(threadNum);
            }
            pthread_mutex_unlock(&_singleInstanceMutex);
        }
        return _instance;
    }

    bool pushTask(const Task& task)
    {
        pthread_mutex_lock(&_mutex);
        _taskQueue.push(task);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_cond);   // debug:需要唤醒线程
        // LOG(DEBUG, "push task seccess.");
        return true;
    }

    ~ThreadPool()
    {
        pthread_mutex_lock(&_mutex);
        _stop = true;
        pthread_mutex_unlock(&_mutex);
        for(auto& tid : _threads){
            // pthread_cond_broadcast(&_cond); // 惊群,竞争锁
            pthread_cond_signal(&_cond);
        }
        for(auto& tid : _threads){
            pthread_join(tid, nullptr);
        }
        // 等所有子进程退出后在释放锁
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);

        // 不能delete instance; 会无限递归,一直调用析构; 即:delete this; 
        // 解决: 交给外部调用者delete _instance,析构不做处理
    }

};
template<class Task>
pthread_mutex_t ThreadPool<Task>::_singleInstanceMutex = PTHREAD_MUTEX_INITIALIZER;
template<class Task>
ThreadPool<Task>* ThreadPool<Task>::_instance = nullptr;

