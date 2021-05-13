#include "pairingHeap.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>
#include <algorithm>

int main()
{

	
	std::vector<int> vec;
	std::generate_n(std::back_inserter(vec), 10, [v = 0]()mutable { return v++; });
	std::random_shuffle(vec.begin(), vec.end());

	std::vector<int> vec2;
	std::generate_n(std::back_inserter(vec), 10, [v = 10]()mutable { return v++; });
	std::random_shuffle(vec.begin(), vec.end());

	for (auto &i : vec)
	{
		std::cout << i << " ";
	}

	for (auto &i : vec2)
	{
		std::cout << i << " ";
	}

	PairingHeap heap1;
	PairingHeap heap2;

	heap1.build(vec);
	heap2.build(vec2);

	heap1.meld(heap2);

	std::cout << "\n";

	vec.clear();

	vec = heap1.getSortedElements();

	for (auto &i : vec)
	{
		std::cout << i << " ";
	}

	std::cout << "\n";

	system("pause");
}