#include <iostream>
#include <cstdio>
#include "graf.h"

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif


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

	std::vector<std::vector<int>> listOfNeighbours;

	listOfNeighbours.push_back({ });
	listOfNeighbours.push_back({ 2, 3, 4 });
	listOfNeighbours.push_back({ 1, 9, 5 });
	listOfNeighbours.push_back({ 1, 5, 7 });
	listOfNeighbours.push_back({ 1, 6 });
	listOfNeighbours.push_back({ 2, 3, 7 });
	listOfNeighbours.push_back({ 4, 8, 7 });
	listOfNeighbours.push_back({ 3, 6, 5 });
	listOfNeighbours.push_back({ 6 });
	listOfNeighbours.push_back({ 2 });

	
	//g.createFromMatrix(matrix, 9);
	g.createFromPairsOfEdges(edges, (sizeof(edges) / sizeof(int)) / 2, true, false);
	//g.createFromListOfNeighbours(listOfNeighbours, true);
	

	//auto v = g.getShortestPath(1, {8, 9}, true);

	//for (auto i : v)
	//{
	//	std::cout << i << " ";
	//}

	//g.printMatrix();
	//g.printListOfNeighbours(true);
	g.printPairsOfEdges(true);


	system("pause");
	return 0;
}