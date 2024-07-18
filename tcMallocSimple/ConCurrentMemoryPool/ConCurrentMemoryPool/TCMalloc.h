#pragma once
#include "comm.h"

// 1.定长内存池
template<typename T>
class FixedMemoryPool 
{
private:
	char* _memory = nullptr;	// 内存池
	void* _freeList = nullptr;// 自由链表,管理还回来的空间; 单链表,前4/8字节指向下一块空间
	int _leftBytes = 0; // 剩余空间大小,字节
	int _unitSize;		// 单位大小,字节
public:
	~FixedMemoryPool()
	{
		// 或许,生命周期随进程, 进程结束自动释放
	}

	//T* New(const T& val = T())
	T* New()
	{
		// 1.先看freeList中有没有,没有再到内存池获取
		T* ret = nullptr;
		if (_freeList != nullptr)
		{
			auto next = *((void**)_freeList);	// 下一块空间地址
			ret = (T*)_freeList;
			_freeList = next;
		}
		// 2. 如果内存池空间还有,使用内存池
		else if(_leftBytes >= sizeof(T))
		{
			ret = (T*)_memory;
			_memory += _unitSize;	// 至少要能存指向下一个空间的指针
			_leftBytes -= _unitSize;
		}
		// 3.初次调用,初始化内存池
		else if(_memory == nullptr)
		{
			_leftBytes = 128 * sizeof(T);
			_memory = (char*) malloc(_leftBytes);
			if(_memory == nullptr) 
				throw std::bad_alloc();
			memset(_memory, 0, _leftBytes);
			_unitSize = sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*);

			ret = (T*)_memory;
			_memory += _unitSize;
			_leftBytes -= _unitSize;
		}
		// 4. 如果内存池空间不足? realloc, 那么freeList怎么处理? 不能realloc,借出去的空间可能没有还来
		// 或则,单独new一个对象, 然后建立一个链表管理
		else
		{
			throw std::bad_alloc();
		}

		// 定位new 初始化
		new(ret) T;

		return ret;
	}

	void Delete(T* obj)
	{
		// 调用析构
		obj->~T();

		// 头插到freeList
		// 小心空指针: 这个处理能避免_freeList初始nullptr的问题
		*((void**)obj) = _freeList; // 头4/8字节指向next	// *obj = next;	 error,访问空间大小是sizeof(T),不对
		_freeList = obj;
	}
};