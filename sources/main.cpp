#include <glad/glad.h>
#include <iostream>
#include <ctime>
#include "tools.h"
#include "profilerLib.h"
#include <random>
#include <vector>
#include <algorithm>

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


std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, 1000);

auto generateVectorWithData()
{
	const int SIZE = 1000;
	std::vector<int> v;
	v.reserve(1000);

	for (int i = 0; i < SIZE; i++)
	{
		v.push_back(dist(rng));
	}

	return std::move(v);
}

template <class T>
void doTest(std::string testName, T sortFunc)
{
	PL::AverageProfiler profiler;


	for (int i = 0; i < 5; i++)
	{
		auto vect = generateVectorWithData();

		profiler.start();
		sortFunc(vect.begin(), vect.end());
		profiler.end();
	}

	auto rez = profiler.getAverageAndResetData();


	printf("%s : time(s) %.6f, cpu clocks: %.10d\n", testName.c_str(), rez.timeSeconds, rez.cpuClocks);
		
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

	
	
	doTest("standard implementation", std::sort<std::vector<int>::iterator>);
	doTest("bouble sort            ", boubleSort<std::vector<int>::iterator>);
	doTest("quick sort             ", quickSort<std::vector<int>::iterator>);



	std::cin.clear();
	std::cin.get();
}