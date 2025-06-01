#pragma once

#include <vector>
#include <functional>
#include <unordered_set>

// 观察者
class Observer
{
public:
	Observer() = default;
	virtual ~Observer() = default;

	virtual void update() = 0;
};

// 订阅主题
class Subject
{
public:
	Subject() = default;
	virtual ~Subject() = default;

	virtual void attach_observer(Observer* observer)
	{
		observer_list.push_back(observer);
	}

	virtual void attach_observer(Observer* observer, const std::function<void()>& update_callback)
	{
		observer_list.push_back(observer);
		observer_callback_list[observer] = update_callback;
	}

	virtual void detach_observer(Observer* observer)
	{
		observer_list.erase(std::remove(observer_list.begin(), observer_list.end(), observer), observer_list.end());
		observer_callback_list.erase(observer);
	}

	virtual void notify_observer()
	{
		for (Observer* observer : observer_list)
		{
			// 优先调用单独注册的更新回调
			if (observer_callback_list.count(observer) && observer_callback_list[observer]) 
				observer_callback_list[observer]();
			else
				observer->update();
		}
	}

private:
	std::vector<Observer*> observer_list;
	std::unordered_map<Observer*, std::function<void()>> observer_callback_list;	// 处理一个观察者订阅多个主题情况
};

