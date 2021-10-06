#pragma once
#include <vector>
#include <utility>


struct Graf
{
#pragma region constructors
	Graf() = default;
	
	//data is a nodesCount * nodesCount matrix with 0 or one
	void createFromMatrix(const int *data, int nodesCount); 
#pragma endregion


#pragma region data

	int nodesCount = 0;		// this number of elements

	struct Entry
	{
		int pos = 0;
		int size = 0;
	};
	std::vector<Entry> entries;		//this will describe how data in neighbours is layed out
									//at entries[0] we have the data about the node number 0 
									//neighbours[entries[0].pos] will have all the neighbours of the node 0
									//and there are entries[0].size neighbours

	std::vector<int> neighbours;	//using a vector so I don't have to write all 
									//the constructors and things
									//neighbours will contain all neighbours packed together



#pragma endregion

#pragma region methods

	std::pair<std::vector<int>::iterator, std::vector<int>::iterator>
		getNeighbours(int element);

	void printNeighbours(bool startFromOne = 0); //prints all neighbours from nodes

#pragma endregion

};

inline void Graf::createFromMatrix(const int *data, int nodesCount)
{
	neighbours.clear();
	entries.clear();
	this->nodesCount = nodesCount;

	entries.reserve(nodesCount);
	neighbours.reserve(nodesCount * (nodesCount - 1)); //reserve the max possible size and shrink later

	auto getEntry = [&](int x, int y) 
	{
		return data[x + y * nodesCount];
	};

	//it is slower to read on y so read it first
	for (int y = 0; y < nodesCount; y++)
	{
		Entry entry;
		entry.pos = neighbours.size();
		int count = 0;

		for (int x = 0; x < nodesCount; x++)
		{
			if (x!=y && getEntry(x, y) != 0)
			{
				//we have a neighbour here to add to our node
				neighbours.push_back(x);
				count++;
			}
		}

		entry.size = count;
		entries.push_back(entry); //this reprezents node number y
	}



	neighbours.shrink_to_fit();
}

inline std::pair<std::vector<int>::iterator, std::vector<int>::iterator> Graf::getNeighbours(int element)
{
	if (element > nodesCount)
	{
		return {};
	}

	auto entry = entries[element];

	std::pair<std::vector<int>::iterator, std::vector<int>::iterator> ret;

	ret.first = neighbours.begin() + entry.pos;
	ret.second = neighbours.begin() + entry.pos + entry.size;

	return ret;
}

inline void Graf::printNeighbours(bool startFromOne)
{
	for (int i = 0; i < nodesCount; i++)
	{
		auto n = getNeighbours(i);

		std::cout << i + (int)startFromOne << ": ";

		for (auto j = n.first; j < n.second; j++)
		{
			std::cout << *j + (int)startFromOne << ", ";
		}

		std::cout << "\n";

	}

}
