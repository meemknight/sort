#include <glad/glad.h>
#include <iostream>
#include <ctime>
#include "tools.h"
#include "profilerLib.h"
#include <random>
#include <vector>
#include <algorithm>
#include <future>
#include <atomic>
#include <execution>
#include <functional>
using namespace std::placeholders;

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#undef min
#undef max


template <typename T>
void boubleSort(T begin, T end)
{
	if (end - begin == 0)
	{
		return;
	}

	for (T i = begin; i < end; i++)
	{
		bool swapped = 0;
		for (T j = begin; j < end - 1; j++)
		{
			if (*j > *(j + 1))
			{
				std::swap(*j, *(j + 1));
				swapped = 1;
			}
		}
		if (!swapped)
		{
			break;
		}
	}

}

template <typename T>
void quickSort(T begin, T end)
{
	if (end - begin < 4)
	{
		boubleSort(begin, end);
		return;
	}

	T pivot = begin + ((end - begin) / 2);
	std::swap(*pivot, *(end - 1));

	T itemFromLeft = begin;
	T itemFromRight = end - 2;

	auto &pivotValue = *(end - 1);

	while (true)
	{

		for (; itemFromLeft < end - 1; itemFromLeft++)
		{
			if (*itemFromLeft > pivotValue) { break; }
		}

		for (; itemFromRight > begin; itemFromRight--)
		{
			if (*itemFromRight < pivotValue) { break; }
		}

		if (itemFromLeft > itemFromRight)
		{
			std::swap(*itemFromLeft, *(end - 1));
			break;
		}

		if (*itemFromLeft > pivotValue && *itemFromRight < pivotValue)
		{
			std::swap(*itemFromLeft, *itemFromRight);
			continue;
		}

		break;

	}

	int dif = begin - itemFromLeft;
	int dif2 = itemFromLeft - end;


	quickSort(begin, itemFromLeft);

	if (begin == itemFromLeft)
	{
		std::swap(*begin, *(end - 1));
	}

	quickSort(itemFromLeft, end);


	return;

}


template <typename T>
void threadedQuickSortNaive(T begin, T end)
{
	if (end - begin < 4)
	{
		boubleSort(begin, end);
		return;
	}

	T pivot = begin + ((end - begin) / 2);
	std::swap(*pivot, *(end - 1));

	T itemFromLeft = begin;
	T itemFromRight = end - 2;

	auto &pivotValue = *(end - 1);

	while (true)
	{

		for (; itemFromLeft < end - 1; itemFromLeft++)
		{
			if (*itemFromLeft > pivotValue) { break; }
		}

		for (; itemFromRight > begin; itemFromRight--)
		{
			if (*itemFromRight < pivotValue) { break; }
		}

		if (itemFromLeft > itemFromRight)
		{
			std::swap(*itemFromLeft, *(end - 1));
			break;
		}

		if (*itemFromLeft > pivotValue && *itemFromRight < pivotValue)
		{
			std::swap(*itemFromLeft, *itemFromRight);
			continue;
		}

		break;

	}

	int dif = begin - itemFromLeft;
	int dif2 = itemFromLeft - end;


	auto future1 = std::async(std::launch::async, 
		threadedQuickSortNaive<std::vector<int>::iterator>, begin, itemFromLeft);

	if (begin == itemFromLeft)
	{
		std::swap(*begin, *(end - 1));
	}

	auto future2 = std::async(std::launch::async, 
		threadedQuickSortNaive<std::vector<int>::iterator>, itemFromLeft, end);


	return;

}


std::atomic<int> atomicCounter = 0;

template <typename T>
void threadedQuickSort(T begin, T end)
{
	if (end - begin < 4)
	{
		boubleSort(begin, end);
		return;
	}

	T pivot = begin + ((end - begin) / 2);
	std::swap(*pivot, *(end - 1));

	T itemFromLeft = begin;
	T itemFromRight = end - 2;

	auto &pivotValue = *(end - 1);

	while (true)
	{

		for (; itemFromLeft < end - 1; itemFromLeft++)
		{
			if (*itemFromLeft > pivotValue) { break; }
		}

		for (; itemFromRight > begin; itemFromRight--)
		{
			if (*itemFromRight < pivotValue) { break; }
		}

		if (itemFromLeft > itemFromRight)
		{
			std::swap(*itemFromLeft, *(end - 1));
			break;
		}

		if (*itemFromLeft > pivotValue && *itemFromRight < pivotValue)
		{
			std::swap(*itemFromLeft, *itemFromRight);
			continue;
		}

		break;

	}

	int dif = begin - itemFromLeft;
	int dif2 = itemFromLeft - end;


	//try to create a thread
	if(std::atomic_fetch_add(&atomicCounter, 1) <= 3)
	{
		//use threaded version

		auto future1 = std::async(std::launch::async,
			threadedQuickSort<std::vector<int>::iterator>, begin, itemFromLeft);

		if (begin == itemFromLeft)
		{
			std::swap(*begin, *(end - 1));
		}


		threadedQuickSort(itemFromLeft, end);

		std::atomic_fetch_sub(&atomicCounter, 1);

	}else
	{
		//don't use threaded version,
		//decrement the counter since no thread will be generated
		std::atomic_fetch_sub(&atomicCounter, 1);


		threadedQuickSort(begin, itemFromLeft);

		if (begin == itemFromLeft)
		{
			std::swap(*begin, *(end - 1));
		}


		threadedQuickSort(itemFromLeft, end);

	}



	return;

}

template <typename T>
void mergeSort(T begin, T end)
{
	if (end - begin < 4)
	{
		boubleSort(begin, end);
		return;
	}

	//sort 2 halves
	T middle = begin + ((end - begin) / 2);
	
	mergeSort(begin, middle);
	mergeSort(middle, end);

	//now merge
	std::vector< std::remove_reference_t<decltype(*begin)>> v;
	v.reserve(end - begin + 1);

	auto it1 = begin;
	auto it2 = middle;

	while(true)
	{
		if(it1>=middle && it2>=end)
		{
			break;
		}

		if(it1>=middle)
		{
			v.push_back(*it2);
			it2++;
			continue;
		}

		if(it2>= end)
		{
			v.push_back(*it1);
			it1++;
			continue;
		}

		if(*it1 < *it2)
		{
			v.push_back(*it1);
			it1++;
			continue;
		}else
		{
			v.push_back(*it2);
			it2++;
			continue;
		}

	}

	std::copy(v.begin(), v.end(), begin);

}



std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 1000);

auto generateVectorWithData(int size)
{

	std::vector<int> v;
	v.reserve(1000);

	for (int i = 0; i < size; i++)
	{
		v.push_back(dist(rng));
	}

	return std::move(v);
}

template <class T>
void doTest(std::string testName, T sortFunc, int size)
{
	PL::AverageProfiler profiler;


	int good = 1;
	for (int i = 0; i < 1; i++)
	{
		auto vect = generateVectorWithData(size);

		profiler.start();
		sortFunc(vect.begin(), vect.end());
		profiler.end();

		//if(!std::is_sorted(vect.begin(), vect.end()))
		//{
		//	good = 0;
		//}

	}

	auto rez = profiler.getAverageAndResetData();

	if(good)
	{
		printf("%s : time(s) %.6f, cpu clocks: %.10d, vector size: %d, tests: 1\n", testName.c_str(), rez.timeSeconds, rez.cpuClocks, size);
	}
	else
	{
		printf("%s : time(s) %.6f, cpu clocks: %.10d, vector size: %d, tests: 1, sort failed.\n", testName.c_str(), rez.timeSeconds, rez.cpuClocks, size);
	}
		
}


int main()
{

#ifdef PLATFORM_WIN32
#ifdef _MSC_VER 
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	std::cout.sync_with_stdio();
#endif
#endif


	
	auto paralelSortStandard = [](std::vector<int>::iterator b, std::vector<int>::iterator e)
	{
		std::sort(std::execution::par, b, e);
	};


	doTest("standard implementation          ", std::sort<std::vector<int>::iterator>, 1'00'000);
	doTest("standard implementation (paralel)", paralelSortStandard, 1'00'000);
	doTest("merge sort                       ", mergeSort<std::vector<int>::iterator>, 1'000'000);
	doTest("quick sort                       ", quickSort<std::vector<int>::iterator>, 1'000'000);
	//doTest("threaded quick sort (naive)      ", threadedQuickSortNaive<std::vector<int>::iterator>, 1'000'000);
	doTest("threaded quick sort              ", threadedQuickSort<std::vector<int>::iterator>, 1'000'000);
	doTest("bouble sort                      ", boubleSort<std::vector<int>::iterator>, 1'000'000);




	system("pause");
	system("pause");
	system("pause");

}