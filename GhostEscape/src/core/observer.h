#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <vector>
#include <functional>
#include <unordered_set>
#include <algorithm>

// 观察者
class Observer
{
public:
	Observer() = default;
	virtual ~Observer() = default;
};

// 订阅主题
class Subject
{
public:
	Subject() = default;
	virtual ~Subject() = default;

	virtual void attachObserver(Observer* observer, const std::function<void()>& update_callback)
	{
		observers_callbacks_.insert({observer, update_callback});
	}

	virtual void detachObserver(Observer* observer)
	{
        remove_list_.push_back(observer);
	}

	virtual void notifyAllObserver()
	{
        removeInvalid();
		for (auto& [observer, callback] : observers_callbacks_)
		{
			if (callback) 
				callback();
		}
	}

    virtual void notifyOneObserver(Observer* observer)
	{
        removeInvalid();
        if(auto iter = observers_callbacks_.find(observer); iter != observers_callbacks_.end())
        {
            if(iter->second)
                iter->second();
        }
	}

private:
    void removeInvalid()
    {
        if(!remove_list_.empty())
        {
            for(auto observer : remove_list_)
                observers_callbacks_.erase(observer);
            remove_list_.clear();
        }
    }

private:
    std::vector<Observer*> remove_list_;     // 为了避免for(:)迭代器失效
	std::unordered_map<Observer*, std::function<void()>> observers_callbacks_;// 处理一个观察者订阅多个主题情况,可以为不同主体注册不同回调
};

#endif // _OBSERVER_H_