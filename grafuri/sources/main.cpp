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

	g.createFromMatrix(matrix, 9);
	
	g.printNeighbours(1);


	system("pause");
	return 0;
}