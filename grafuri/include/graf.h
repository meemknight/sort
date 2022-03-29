//////////////////////////////////////////////////////////////////////////
//Luta Vlad (c)
//https://github.com/meemknight/sort/blob/master/grafuri/include/graf.h
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <queue>
#include <limits.h>
#include <iostream>

#ifdef _MSC_VER
#include <signal.h>
#endif

#undef max
#undef min

struct Graf
{
	Graf() = default;

	//todo add some templates

	//data is a nodesCount * nodesCount matrix with 0 or one
	void createFromMatrix(const int* data, int nodesCount);

	//data is a pair of 2 numbers defining an edge (this supports oriented grafs)
	void createFromPairsOfEdges(const int* data, int edgesCount, bool startFromOne, bool oriented, int optionalNodes = -1);

	//for every node(first vector) we have some neighbours(second vector)
	void createFromListOfNeighbours(const std::vector<std::vector<int>>& data, bool startFromOne);


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



	std::pair<std::vector<int>::iterator, std::vector<int>::iterator>
		getNeighbours(int element);


	Graf crateTranspose(bool oriented);

	//"from" should not be equal to "to"
	std::vector<int> getShortestPath(int from, std::vector<int> to, bool startFromOne);
	std::vector<int> getPathLength(int from, std::vector<int> to, bool startFromOne);
	std::vector<int> getTopologicSort(bool startFromOne);
	std::vector<std::pair<int, int>> criticalConnections(bool startFromOne);
	std::vector<int> getEulerianCycle(bool startFromOne);

	//doar arbori
	int getDiameter();
	int getDistanceToFarthestNode(int node);

	std::vector<int> getMatrix(int* nodesCount);
	std::vector<std::pair<int, int>> getPairsOfEdges(bool startFromOne, bool oriented);
	std::vector<std::vector<int>> getListOfNeighbours(bool startFromOne);
	int countConexComponents();
	std::vector<std::vector<int>> getStronglyConectedComponents(bool startFromOne);


	void printMatrix();
	void printListOfNeighbours(bool startFromOne); //prints all neighbours from nodes
	void printPairsOfEdges(bool startFromOne, bool oriented);

};

inline bool havelHakimi(std::vector<int> grade)
{

	// 5 4 4 3 3 2 2 1
	// - 3 3 2 2 1 2 1
	// - 3 3 2 2 2 1 1
	// - - 2 1 1 2 1 1
	// - - 2 2 1 1 1 1 -
	// - - - 1 0 1 1 1
	// - - - 1 1 1 1 -


	// 5 4 4 3 3 2 2 1

	//         |
	//         V
	// 1 2 3 4 5 6 7 8 -> nr de elemente
	// 1 2 2 2 1
	// 2 3 2 0 0 
	// 4 2 0 0 0 - 

	while (true)
	{
		std::sort(grade.begin(), grade.end(), std::greater<int>());
		while (!grade.empty() && grade.back() == 0)
		{
			grade.erase(grade.begin() + grade.size() - 1);
		}

		if (grade.size() > 1)
		{
			int g = grade[0];

			for(int i=1; g>0; i++, g--)
			{
				if (i >= grade.size())
				{
					return 0;
				}
				else
				{
					grade[i]--;
				}
			}

			grade.erase(grade.begin());
		}
		else
		{
			break;
		}
	}

	if (grade.empty())
	{
		return true;
	}
	else
	{
		return false;
	}

}

// leftmost element which is smaller than current index
inline int LowerBound(int lf, int rg, int val, std::vector<int>& grade, int n)
{
	if (lf > rg) return n;

	int mid = lf + (rg - lf) / 2;

	if (grade[mid] <= val)
		return std::min(mid, LowerBound(lf, mid - 1, val, grade, n));
	else
		return LowerBound(mid + 1, rg, val, grade, n);
}

inline bool ErdosGaillai(std::vector<int> &grade)
{
	int n = grade.size();
	
	if (n < 1) { return true; }
	
	std::sort(grade.begin(), grade.end());
	std::vector<int> s;
	s.resize(n);

	s[0] = grade[0];
	for (int i = 1; i < n; i++)
	{
		s[i] = s[i - 1] + grade[i];
	}

	if (s[n-1] % 2)
		return false;
	

	for (int i = 0; i < n; i++)
	{
		int pos = LowerBound(i, n, i, grade, n);
		long long val;

		if (pos <= n)
			val = 1LL * i * (i - 1) + s[n] - s[pos - 1] + i * (pos - i - 1);
		else
			val = 1LL * i * (i - 1) + i * (n - i);

		if (s[i] > val)
			return false;
	}

	return true;
}



inline std::vector<std::vector<int>> royFloyd(std::vector<std::vector<int>> in)
{
	int n = in.size();
	
	for (int k = 0; k < n; k++)
		for (int i = 1; i < n; i++)
			for (int j = 1; j < n; j++)
				if (in[i][k] && in[k][j] && (in[i][j] > in[i][k] + in[k][j] || !in[i][j]) && i != j)
				{
					in[i][j] = in[i][k] + in[k][j];
				}

	return in;
};


inline void Graf::createFromMatrix(const int* data, int nodesCount)
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
			if (x != y && getEntry(x, y) != 0)
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

inline void Graf::createFromPairsOfEdges(const int* data, int edgesCount, bool startFromOne,
	bool oriented, int optionalNodes)
{
	neighbours.clear();
	entries.clear();
	this->nodesCount = 0;

	if (edgesCount == 0)
	{
		return;
	}

	if (optionalNodes > 0)
	{
		this->nodesCount = optionalNodes;
		entries.resize(this->nodesCount);
	}

	std::vector<std::pair<int, int>> sortedData;
	sortedData.reserve(edgesCount);
	int biggestEl = 0;

	for (int i = 0; i < edgesCount; i++)
	{
		int a = data[i * 2 + 0] - (int)startFromOne;
		int b = data[i * 2 + 1] - (int)startFromOne;

		if (a > biggestEl) { biggestEl = a; }
		if (b > biggestEl) { biggestEl = b; }

	#ifdef _MSC_VER
		if (a < 0 || b < 0 ||
			((optionalNodes > 0)&&(a>= optionalNodes +startFromOne || b>= optionalNodes +startFromOne))
			)
		{
			__debugbreak();
			raise(SIGABRT);
		}
	#endif

		sortedData.emplace_back(a, b);

		if (!oriented && a != b)
		{
			sortedData.emplace_back(b, a);
		}
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
		this->nodesCount = biggestEl + 1;
	}

	entries.resize(this->nodesCount);
	neighbours.reserve(this->nodesCount); //we can't reserve the max possible value, it is too big

	int lastNode = -1;
	for (auto& i : sortedData)
	{
		//if (i.first == i.second) { continue; }

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

inline void Graf::createFromListOfNeighbours(const std::vector<std::vector<int>>& data, bool startFromOne)
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

inline Graf Graf::crateTranspose(bool oriented)
{
	auto initialGraf = this->getPairsOfEdges(0, oriented);

	for (auto &i : initialGraf) 
	{
		std::swap(i.first, i.second);
	}

	Graf g;
	g.createFromPairsOfEdges(&initialGraf[0].first, initialGraf.size(), 0, true, this->nodesCount);

	return g;
}

inline std::vector<int> Graf::getShortestPath(int from, std::vector<int> to, bool startFromOne)
{
	from -= (int)startFromOne;

	std::vector<int> shortestPath;
	shortestPath.resize(nodesCount);

	std::vector<int> returnPath; //this will trace any node back to "from" using the shortest path
	returnPath.resize(nodesCount, -1);

	std::queue<std::pair<int, int>> nodesToVisit; //current node, shortest path
	nodesToVisit.push({from, 0});

	while (!nodesToVisit.empty())
	{
		auto node = nodesToVisit.front();
		nodesToVisit.pop();

		auto n = getNeighbours(node.first);

		for (auto i = n.first; i < n.second; i++)
		{
			if (*i != from) //not visit the first node
			{
				if (shortestPath[*i] == 0) //node not visited
				{
					shortestPath[*i] = node.second + 1;
					nodesToVisit.push({*i, node.second + 1});
					returnPath[*i] = node.first;
				}
			}
		}
	}

	int shortestIndex = -1;
	int currentShortest = INT_MAX;
	for (int i = 0; i < to.size(); i++)
	{
		auto n = to[i] - (int)startFromOne;
		if (shortestPath[n] < currentShortest)
		{
			currentShortest = shortestPath[n];
			shortestIndex = n;
		}
	}

	std::vector<int> returnVector;
	returnVector.reserve(currentShortest + 1);
	{
		int el = returnPath[shortestIndex];
		returnVector.push_back(shortestIndex + (int)startFromOne);
		while (el >= 0)
		{
			returnVector.push_back(el + (int)startFromOne);
			el = returnPath[el];
		}
	}

	std::reverse(returnVector.begin(), returnVector.end());
	return returnVector;
}


inline std::vector<int> Graf::getPathLength(int from, std::vector<int> to, bool startFromOne)
{
	from -= (int)startFromOne;

	std::vector<int> shortestPath;
	shortestPath.resize(nodesCount, -1);

	//std::vector<int> returnPath; //this will trace any node back to "from" using the shortest path
	//returnPath.resize(nodesCount, -1);

	std::queue<std::pair<int, int>> nodesToVisit; //current node, shortest path
	nodesToVisit.push({from, 0});

	while (!nodesToVisit.empty())
	{
		auto node = nodesToVisit.front();
		nodesToVisit.pop();

		auto n = getNeighbours(node.first);

		for (auto i = n.first; i < n.second; i++)
		{
			if (*i != from) //not visit the first node
			{
				if (shortestPath[*i] == -1) //node not visited
				{
					shortestPath[*i] = node.second + 1;
					nodesToVisit.push({*i, node.second + 1});
					//returnPath[*i] = node.first;
				}
			}
		}
	}

	std::vector<int> returnVector;
	returnVector.reserve(nodesCount);
	for (int i=0;i<nodesCount;i++)
	{
		returnVector.push_back(shortestPath[i]);
	}
	
	return returnVector;

	//if we ever want to specify which values to return
	//std::vector<int> returnVector;
	//returnVector.reserve(to.size());
	//for (auto i : to)
	//{
	//	returnVector.push_back(shortestPath[i - (int)startFromOne]);
	//}
	//
	//return returnVector;
}

inline std::vector<int> Graf::getTopologicSort(bool startFromOne)
{
	std::queue<int> noduri;

	std::vector<int> gradeInterioare;
	gradeInterioare.resize(nodesCount);

	for (int i = 0; i < nodesCount; i++)
	{
		auto v = getNeighbours(i);

		for (auto j = v.first; j < v.second; j++)
		{
			gradeInterioare[*j]++;
		}
	}

	for (int i=0; i< gradeInterioare.size(); i++)
	{
		if (gradeInterioare[i] == 0)
		{
			noduri.push(i);
		}
	}

	std::vector<int> ret;
	ret.reserve(nodesCount);

	while (!noduri.empty())
	{
		auto t = noduri.front();
		ret.push_back(t + (int)startFromOne);

		auto vecini = getNeighbours(t);
		
		for (auto j = vecini.first; j < vecini.second; j++)
		{
			gradeInterioare[*j]--;
			if (gradeInterioare[*j] == 0)
			{
				noduri.push(*j);
			}
		}

		noduri.pop();
	}

	return ret;
}


//todo
void dfsCriticalConnections(Graf& g, std::vector<unsigned char>& visited, int index, std::vector<std::vector<int>> &adiacenta,
	std::vector<std::vector<int>> intoarcere)
{
	if (visited[index] == 0)
	{
		visited[index] = 1;

		auto vecini = g.getNeighbours(index);

		for (auto j = vecini.first; j < vecini.second; j++)
		{
			if (visited[*j] == 0)
			{
				adiacenta[index].push_back(*j);
			}

			//dfsCriticalConnections(g, visited, *j, adiacenta);
		}
	}
}

//todo
inline std::vector<std::pair<int, int>> Graf::criticalConnections(bool startFromOne)
{


	return std::vector<std::pair<int, int>>();
}

//https://infoarena.ro/problema/ciclueuler
//euler(nod v)
//	cat timp(v are vecini)
//		w = un vecin aleator al lui v
//		sterge_muchie(v, w)
//		euler(w)
//	sfarsit cat timp
//	adauga v la ciclu
void euler(int nod, std::vector<std::vector<int>> &vecini, std::vector<int> &ciclu)
{
	while (!vecini[nod].empty())
	{
		int vecin = vecini[nod].back();
		vecini[nod].pop_back();
		if(nod != vecin)
		{ 
			auto f = std::find(vecini[vecin].begin(), vecini[vecin].end(), nod);
			if (f != vecini[vecin].end())
			{
				*f = vecini[vecin].back();
				vecini[vecin].pop_back();
			}
		}
		euler(vecin, vecini, ciclu);
	}
	ciclu.push_back(nod);
}

inline std::vector<int> Graf::getEulerianCycle(bool startFromOne)
{
	auto vecini = this->getListOfNeighbours(0);
	std::vector<int> ciclu;
	ciclu.reserve(nodesCount);
	euler(0, vecini, ciclu);

	for (auto i : vecini)
	{
		if (!i.empty())
		{
			return {};
		}
	}

	if (startFromOne)
	{
		for (auto& i : ciclu) { i++; }
	}
	
	return ciclu;
}

//doar pt arbore
inline int Graf::getDiameter()
{
	int rootNode = 0;
	//first find the root node
	{
		std::vector<int> grade;
		grade.resize(nodesCount);

		for (int i = 0; i < nodesCount; i++)
		{
			auto vecini = getNeighbours(i);
			for (auto j = vecini.first; j < vecini.second; j++)
			{
				int g = *j;
				grade[g]++;
			}
		}

		for (int i = 0; i < grade.size(); i++)
		{
			if (grade[i] == 0) { rootNode = i; break; }
		}
	}
	
	int firstEnd = 0;
	//find the farthest node.
	{
		std::queue<int> queue;
		queue.push(rootNode);

		while (!queue.empty())
		{
			int nod = queue.front();
			firstEnd = nod;
			queue.pop();

			auto vecini = getNeighbours(nod);

			for (auto i = vecini.first; i < vecini.second; i++)
			{
				queue.push(*i);
			}
		}
	}

	{
		//find the farthest node.
		
		//we first make the not oriented graph
		auto v = getPairsOfEdges(0, 1);

		Graf g;
		g.createFromPairsOfEdges((int*)v.data(), v.size(), 0, 0, nodesCount);
		return g.getDistanceToFarthestNode(firstEnd);

	}

}

//doar arbori
inline int Graf::getDistanceToFarthestNode(int node)
{
	std::queue<std::pair<int, int>> queue;
	queue.push({node, 0});
	int biggestDistance = 0;
	std::vector<char> visited;
	visited.resize(nodesCount);
	visited[node] = true;

	while (!queue.empty())
	{
		auto nod = queue.front();
		queue.pop();

		if (nod.second > biggestDistance)
		{
			biggestDistance = nod.second;
		}

		auto vecini = getNeighbours(nod.first);

		for (auto i = vecini.first; i < vecini.second; i++)
		{
			if (!visited[*i])
			{
				visited[*i] = true;
				queue.push({*i, nod.second + 1});
			}
		}
	}

	return biggestDistance+1;
}


inline std::vector<int> Graf::getMatrix(int* nodesCount)
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

inline std::vector<std::pair<int, int>> Graf::getPairsOfEdges(bool startFromOne, bool oriented)
{

	if (!oriented)
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
				edges.emplace_back(a+(int)startFromOne, b + (int)startFromOne);
			}
		}

		std::set<std::pair<int, int>> s(edges.begin(), edges.end()); //todo replace with unordered_set
		edges.assign(s.begin(), s.end());

		return edges;
	}
	else
	{
		std::vector<std::pair<int, int>> edges;
		edges.reserve(neighbours.size() + 2);

		for (int i = 0; i < nodesCount; i++)
		{
			auto n = getNeighbours(i);
			for (auto j = n.first; j < n.second; j++)
			{
				edges.emplace_back(i + (int)startFromOne, *j + (int)startFromOne);
			}
		}

		//std::set<std::pair<int, int>> s(edges.begin(), edges.end()); //todo replace with unordered_set
		//edges.assign(s.begin(), s.end());

		return edges;
	}
	
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


void dfs(Graf& g, std::vector<unsigned char>& visited, int index)
{
	if (visited[index] == 0)
	{
		visited[index] = 1;

		auto vecini = g.getNeighbours(index);

		for (auto j = vecini.first; j < vecini.second; j++)
		{
			dfs(g, visited, *j);
		}

	}
};

inline int Graf::countConexComponents()
{
	std::vector<unsigned char> visited;
	visited.resize(nodesCount);

	int componenteConexe = 0;

	for (int i = 0; i < nodesCount; i++)
	{
		if (visited[i] == 0)
		{
			componenteConexe++;
			dfs(*this, visited, i);
		}
	}

	return componenteConexe;
}

void dfsForStronglyConected(Graf& g, std::vector<std::pair<char, char>> &plusMinus, int index, bool minus)
{
	if (plusMinus[index].first != 2)
	{
		char* v = 0;
		if (minus)
		{
			v = &plusMinus[index].second;
		}
		else
		{
			v = &plusMinus[index].first;
		}

		if (*v == 0)
		{
			*v = 1;

			auto vecini = g.getNeighbours(index);

			for (auto j = vecini.first; j < vecini.second; j++)
			{
				dfsForStronglyConected(g, plusMinus, *j, minus);
			}

		}
	
	}
};

inline std::vector<std::vector<int>> Graf::getStronglyConectedComponents(bool startFromOne)
{
	if (nodesCount == 0) { return {}; }

	std::vector<std::vector<int>> returnVector;
	returnVector.reserve(100);

	std::vector<std::pair<char,char>> plusMinus;
	plusMinus.resize(nodesCount, {});


	auto transpusGraf = crateTranspose(true);

	//we will mark plusMinus.first = 2 for nodes that are eliminated
	while (true)
	{
		int index = -1;

		for (int i = 0; i<plusMinus.size(); i++)
		{
			if (plusMinus[i].first != 2)
			{
				index = i;
				break;
			}
		}

		if (index < 0)
		{
			//found all the strongly conex grafs
			break;
		}

		dfsForStronglyConected(*this, plusMinus, index, 0);
		dfsForStronglyConected(transpusGraf, plusMinus, index, 1);
		
		returnVector.push_back({});
		returnVector.back().reserve(100);
		for (int i = 0; i < plusMinus.size(); i++)
		{
			if (plusMinus[i].first == 1 && plusMinus[i].second == 1)
			{
				plusMinus[i].first = 2;
				returnVector.back().push_back(i + (int)startFromOne);
			}
			else
			{
				if (plusMinus[i].first != 2)
				{
					plusMinus[i] = {};
				}
			}
		}

	}


	return returnVector;
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

inline void Graf::printPairsOfEdges(bool startFromOne, bool oriented)
{
	auto edges = getPairsOfEdges(startFromOne, oriented);

	for (auto& i : edges)
	{
		std::cout << i.first + (int)startFromOne << " " << i.second + (int)startFromOne << "\n";
	}
}

