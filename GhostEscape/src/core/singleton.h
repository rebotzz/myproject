#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<class T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T instance;
        return instance;
    }
protected:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton<T>& operator=(const Singleton<T>&) = delete;
};

#endif //_SINGLETON_H_