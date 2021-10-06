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

	//g.createFromMatrix(matrix, 9);
	g.createFromPairsOfEdges(edges, (sizeof(edges) / sizeof(int)) / 2, true);


	g.printNeighbours(true);


	system("pause");
	return 0;
}