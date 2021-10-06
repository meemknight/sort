#include "pairingHeap.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <functional>
#include <algorithm>

int main()
{
	constexpr int size = 200'000;
	constexpr int heapsSize = 10;

	std::vector<int> vec;
	vec.reserve(size);
	std::generate_n(std::back_inserter(vec), size, [v = 0]()mutable { return v++; });
	std::random_shuffle(vec.begin(), vec.end());

	PairingHeap heaps[heapsSize];
	int index = 0;

	for (int i = 0; i < vec.size(); i++)
	{
		heaps[index].insert(vec[i]);

		index++;
		index %= heapsSize;

	}
	
	PairingHeap heap;
	
	for(int i=0;i<heapsSize;i++)
	{
		heap.meld(heaps[i]);
	}

	
	std::cout << "\n";

	vec.clear();

	vec = heap.getSortedElements();

	std::cout << (std::is_sorted(vec.begin(), vec.end()) ? "vector is sorted properly" : "vector is not sorted properly") << "\n";


	system("pause");
}