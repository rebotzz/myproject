#ifndef _SINGLETON_H_	
#define _SINGLETON_H_

// ¿É¼Ì³Ðµ¥ÀýÄ£°å
template <typename T>
class Singleton
{
public:
	static T* instance()
	{
		if (_instance == nullptr)
			_instance = new T();
		return _instance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

private:
	static T* _instance;
};

template<typename T>
T* Singleton<T>::_instance = nullptr;

#endif // !_SINGLETON_H_
