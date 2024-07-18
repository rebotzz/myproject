#pragma once
#include "comm.h"

// 1.�����ڴ��
template<typename T>
class FixedMemoryPool 
{
private:
	char* _memory = nullptr;	// �ڴ��
	void* _freeList = nullptr;// ��������,���������Ŀռ�; ������,ǰ4/8�ֽ�ָ����һ��ռ�
	int _leftBytes = 0; // ʣ��ռ��С,�ֽ�
	int _unitSize;		// ��λ��С,�ֽ�
public:
	~FixedMemoryPool()
	{
		// ����,�������������, ���̽����Զ��ͷ�
	}

	//T* New(const T& val = T())
	T* New()
	{
		// 1.�ȿ�freeList����û��,û���ٵ��ڴ�ػ�ȡ
		T* ret = nullptr;
		if (_freeList != nullptr)
		{
			auto next = *((void**)_freeList);	// ��һ��ռ��ַ
			ret = (T*)_freeList;
			_freeList = next;
		}
		// 2. ����ڴ�ؿռ仹��,ʹ���ڴ��
		else if(_leftBytes >= sizeof(T))
		{
			ret = (T*)_memory;
			_memory += _unitSize;	// ����Ҫ�ܴ�ָ����һ���ռ��ָ��
			_leftBytes -= _unitSize;
		}
		// 3.���ε���,��ʼ���ڴ��
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
		// 4. ����ڴ�ؿռ䲻��? realloc, ��ôfreeList��ô����? ����realloc,���ȥ�Ŀռ����û�л���
		// ����,����newһ������, Ȼ����һ���������
		else
		{
			throw std::bad_alloc();
		}

		// ��λnew ��ʼ��
		new(ret) T;

		return ret;
	}

	void Delete(T* obj)
	{
		// ��������
		obj->~T();

		// ͷ�嵽freeList
		// С�Ŀ�ָ��: ��������ܱ���_freeList��ʼnullptr������
		*((void**)obj) = _freeList; // ͷ4/8�ֽ�ָ��next	// *obj = next;	 error,���ʿռ��С��sizeof(T),����
		_freeList = obj;
	}
};