#ifndef _SINGLETON_H_
#define _SINGLETON_H_

// 可继承单例模板
template<class T>
class Singleton
{
public:
    static T& getInstance()
    {
        // c++11 饿汉模式,线程安全
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton<T>& operator=(const Singleton<T>&) = delete;
};



#endif // _SINGLETON_H_