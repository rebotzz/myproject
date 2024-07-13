#include <iostream>
#include <ctime>
#include "tcmalloc.h"
using namespace std;

void test()
{
	int times = 1000000;

	auto begin = clock();
	for (int i = 0; i < times; ++i)
	{
		auto ptr = new int;
		delete ptr;
	}
	auto end = clock();
	cout << "new/delete: " << end - begin << endl;

	begin = clock();
	for (int i = 0; i < times; ++i)
	{
		auto ptr = (int*) malloc(sizeof(int));
		free(ptr);
	}
	end = clock();
	cout << "malloc/free: " << end - begin << endl;

	begin = clock();
	FixedMemoryPool<int> memoryPool;
	for (int i = 0; i < times; ++i)
	{
		auto ptr = memoryPool.New();
		memoryPool.Delete(ptr);
	}
	end = clock();
	cout << "FixedMemoryPool: " << end - begin << endl;

}

int main()
{
	try
	{
		cout << "test" << endl;
		test();
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}


	return 0;
}