#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <set>
#undef max
#undef min

struct Graf
{
#pragma region constructors
	Graf() = default;
	
	//todo add some templates

	//data is a nodesCount * nodesCount matrix with 0 or one
	void createFromMatrix(const int *data, int nodesCount); 

	//data is a pair of 2 numbers defining an edge (1-2 is the same as 2-1 and edges like 1-1 are ignored)
	void createFromPairsOfEdges(const int *data, int edgesCount, bool startFromOne, int optionalNodes = -1);

	//for every node(first vector) we have some neighbours(second vector)
	void createFromListOfNeighbours(const std::vector<std::vector<int>> &data, bool startFromOne);

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

	std::vector<int> getMatrix(int *nodesCount);
	std::vector<std::pair<int, int>> getPairsOfEdges(bool startFromOne);
	std::vector<std::vector<int>> getListOfNeighbours(bool startFromOne);

	void printMatrix();
	void printListOfNeighbours(bool startFromOne); //prints all neighbours from nodes
	void printPairsOfEdges(bool startFromOne);

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

inline void Graf::createFromPairsOfEdges(const int *data, int edgesCount, bool startFromOne, int optionalNodes)
{
	neighbours.clear();
	entries.clear();
	this->nodesCount = 0;

	if (edgesCount == 0) { return; }
	if (optionalNodes == 0) { return; }

	std::vector<std::pair<int, int>> sortedData;
	sortedData.reserve(edgesCount*2);
	
	for (int i = 0; i < edgesCount; i++)
	{
		sortedData.emplace_back(data[i * 2 + 0] - (int)startFromOne, data[i * 2 + 1] - (int)startFromOne);
		sortedData.emplace_back(data[i * 2 + 1] - (int)startFromOne, data[i * 2 + 0] - (int)startFromOne);
	}

	//todo remove duplicates if they exist

	std::sort(sortedData.begin(), sortedData.end(), []
	(std::pair<int, int> a, std::pair<int, int> b) 
	{
		if (a.first == b.first)
		{
			return a.second < b.second;
		}
		else
		{
			return a.first < b.first;
		}

	});

	if (optionalNodes >= 0)
	{
		this->nodesCount = optionalNodes;
	}
	else
	{
		this->nodesCount = sortedData.back().first + 1;
	}


	entries.resize(this->nodesCount);
	neighbours.reserve(this->nodesCount * (this->nodesCount - 1)); //reserve the max possible size and shrink later

	int lastNode = -1;
	for (auto &i :sortedData)
	{
		if (i.first == i.second) { continue; }

		if (lastNode != i.first)
		{
			entries[i.first].pos = neighbours.size();
		}

		lastNode = i.first;

		neighbours.push_back(i.second);
		entries[i.first].size++;

	}

	neighbours.shrink_to_fit();
}

inline void Graf::createFromListOfNeighbours(const std::vector<std::vector<int>> &data, bool startFromOne)
{
	neighbours.clear();
	entries.clear();
	this->nodesCount = std::max((unsigned int)data.size() - (unsigned int)startFromOne, (unsigned int)0);
	if (this->nodesCount == 0) { return; }

	entries.resize(this->nodesCount);
	neighbours.reserve(this->nodesCount * (this->nodesCount - 1)); //reserve the max possible size and shrink later
	
	for (int i = (int)startFromOne; i < data.size(); i++)
	{
		entries[i - (int)startFromOne].pos = neighbours.size();
		entries[i - (int)startFromOne].size = data[i].size();

		for (int j = 0; j < data[i].size(); j++)
		{
			neighbours.push_back(data[i][j] - (int)startFromOne);
		}
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

inline std::vector<int> Graf::getMatrix(int *nodesCount)
{
	if (nodesCount) { *nodesCount = this->nodesCount; }

	std::vector<int> ret;
	ret.reserve(this->nodesCount * this->nodesCount);

	for (int j = 0; j < this->nodesCount; j++)
	{
		auto n = getNeighbours(j);
		for (int i = 0; i < this->nodesCount; i++)
		{
			if (i == j)
			{
				ret.push_back(0);
			}
			else
			{
				if (std::find(n.first, n.second, i) != n.second)
				{
					ret.push_back(1);
				}
				else
				{
					ret.push_back(0);
				}
			}
		}
	}


	return ret;
}

inline std::vector<std::pair<int, int>> Graf::getPairsOfEdges(bool startFromOne)
{
	std::vector<std::pair<int, int>> edges;
	edges.reserve(neighbours.size() + 2);

	for (int i = 0; i < nodesCount; i++)
	{
		auto n = getNeighbours(i);
		for (auto j = n.first; j < n.second; j++)
		{
			auto a = i, b = *j;
			if (a > b)
			{
				std::swap(a, b);
			}
			edges.emplace_back(a, b);
		}
	}

	std::set<std::pair<int, int>> s(edges.begin(), edges.end()); //todo replace with unordered_set
	edges.assign(s.begin(), s.end());

	return edges;
}

inline std::vector<std::vector<int>> Graf::getListOfNeighbours(bool startFromOne)
{
	std::vector<std::vector<int>> ret;
	ret.reserve(nodesCount + (int)startFromOne);
	
	if (startFromOne)
	{
		ret.push_back({});
	}

	for (int i = 0; i < nodesCount; i++)
	{
		auto n = getNeighbours(i);

		ret.push_back({});
		ret.back().reserve(n.second - n.first + (int)startFromOne);

		for (auto j = n.first; j < n.second; j++)
		{
			ret.back().push_back(*j + (int)startFromOne);
		}
	}

	return ret;
}

inline void Graf::printMatrix()
{
	auto m = getMatrix(nullptr);

	for (int j = 0; j < nodesCount; j++)
	{
		for (int i = 0; i < nodesCount; i++)
		{
			if (i == j)
			{
				std::cout << '0';
			}
			else
			{
				std::cout << m[i + j * nodesCount];
			}
			std::cout << " ";
		}
		std::cout << "\n";
	}
}

inline void Graf::printListOfNeighbours(bool startFromOne)
{
	auto l = getListOfNeighbours(startFromOne);

	for (int i = (int)startFromOne; i < l.size(); i++)
	{
		std::cout << i << ": ";
		for (auto j : l[i])
		{
			std::cout << j << ", ";
		}
		std::cout << "\n";
	}

}

inline void Graf::printPairsOfEdges(bool startFromOne)
{
	auto edges = getPairsOfEdges(startFromOne);

	for(auto &i :edges)
	{
		std::cout << i.first + (int)startFromOne << " " << i.second + (int)startFromOne << "\n";
	}
}
