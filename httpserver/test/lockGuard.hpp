#pragma once 
#include <pthread.h>
#include <iostream>
#include "logMessage.hpp"
// RAII风格锁
class Mutex
{
public:
    Mutex(pthread_mutex_t * mtx):_mtx(mtx)
    {}

    void lock()
    {
        pthread_mutex_lock(_mtx);
    }

    void unlock()
    {
        pthread_mutex_unlock(_mtx);
    }
private:
    pthread_mutex_t *_mtx;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t *_pmtx):_mutex(_pmtx)
    {
        logMessage(DEBUG,"file: %s, line: %d, lock", __FILE__, __LINE__);
        _mutex.lock();
    }

    ~LockGuard()
    {
        logMessage(DEBUG,"file: %s, line: %d, unlock", __FILE__, __LINE__);
        _mutex.unlock();
    }
private:
    Mutex _mutex;
};
