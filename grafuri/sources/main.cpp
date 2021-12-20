#include <utility>
#include <iostream>
#include <cstdio>
#include "graf.h"
#include <map>
#include <random>
#include <unordered_set>
#include <utility>
#include <fstream>

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

Graf createRandomOrientedGraf(int n)
{

	//std::unordered_set<std::pair<int, int>, pair_hash> edges;

	std::mt19937 random{std::random_device()()};
	std::uniform_int_distribution dist(0, 19);
	std::uniform_int_distribution dist2(0, 250);
	std::uniform_int_distribution grad(0, 25);

	std::vector<std::pair<int, int>> edges;

	for (int i = 0; i < 12; i++)
	{
		int to = 10 + dist(random);

		if (to < n)
		{
			edges.emplace_back(0, to);
		}
	}

	for (int i = 1; i < n; i++)
	{
		
		auto grade = grad(random);
		std::unordered_set<int> toNodes;
		for(int j=0;j<grade; j++)
		{
			int to = ((i / 10) + 1) * 10 + dist2(random);

			if (to < n)
			{
				if (to < 0)
				{
					__debugbreak();
				}

				toNodes.insert(to);
			}
		}

		for (auto j : toNodes)
		{
			edges.emplace_back(i, j);
		}
	}


	Graf g;
	g.createFromPairsOfEdges(&edges[0].first, edges.size(), 0, 1, n);
	return g;

}


int main()
{

#ifdef PLATFORM_WIN32
#ifdef _MSC_VER 
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
	std::cout.sync_with_stdio();
#endif
#endif

	Graf g;

	int matrix[] ={
		0, 1, 1, 1, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 1, 0, 0,
		1, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 1, 1, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 0, 1, 1, 0,
		0, 0, 1, 0, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, };

	int edges[] = {
		1, 2,
		1, 3,
		1, 4,
		2, 5,
		2, 9,
		3, 5,
		3, 7,
		5, 7,
		6, 7,
		6, 8,
		4, 6,
	};

	int ctc[] = {
		1, 2,
		2, 6,
		6, 7,
		7, 6,
		3, 1,
		3, 4,
		2, 3,
		4, 5,
		5, 4,
		6, 5,
		5, 8,
		8, 7,
	};

	int diametru[] = {
		1, 2,
		1, 3,
		1, 4,
		2, 5,
		3, 6,
		4, 7,
		5, 8,
		5, 9,
		6, 10,
		10, 11,
	};

	int euler[] = {
		1, 2,
		1, 3,
		2, 2,
		2, 3,
		3, 4,
		3, 4,
	};

	g.createFromPairsOfEdges(euler, 6, 1, 0, 4);

	auto rez = g.getEulerianCycle(1);
	for (auto i : rez)
	{
		std::cout << i << ' ';
	}

	std::cout << "\n";

	//std::vector<std::vector<int>> listOfNeighbours;
	//listOfNeighbours.push_back({ });
	//listOfNeighbours.push_back({ 2, 3, 4 });
	//listOfNeighbours.push_back({ 1, 9, 5 });
	//listOfNeighbours.push_back({ 1, 5, 7 });
	//listOfNeighbours.push_back({ 1, 6 });
	//listOfNeighbours.push_back({ 2, 3, 7 });
	//listOfNeighbours.push_back({ 4, 8, 7 });
	//listOfNeighbours.push_back({ 3, 6, 5 });
	//listOfNeighbours.push_back({ 6 });
	//listOfNeighbours.push_back({ 2 });

	
	//g.createFromMatrix(matrix, 9);
	//g.createFromPairsOfEdges(edges, (sizeof(edges) / sizeof(int)) / 2, true, false);
	//g.createFromPairsOfEdges(diametru, (sizeof(diametru) / sizeof(int)) / 2, true, true);
	//g.createFromListOfNeighbours(listOfNeighbours, true);
	
	//std::cout << g.getDiameter() << "\n";


	//auto g2 = createRandomOrientedGraf(100000);
	////std::cout << g2.getStronglyConectedComponents(0).size() << "\n";
	////std::cout << g2.nodesCount << "\n";
	//auto rez = g2.getPairsOfEdges(1, true);
	//std::ofstream fout("drumminimdag.in");
	//fout << g2.nodesCount << " " << rez.size() << "\n";
	//std::mt19937 random{std::random_device()()};
	//std::uniform_int_distribution dist(1, 10);
	//
	//
	//for (int i = 0; i < rez.size(); i++)
	//{
	//	fout << rez[i].first << " " << rez[i].second << " " << dist(random) << "\n";
	//}
	//
	//fout.close();

	system("pause");
	return 0;
}