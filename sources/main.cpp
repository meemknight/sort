#include <glad/glad.h>
#include <iostream>
#include <ctime>
#include "tools.h"
#include "profilerLib.h"


#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#undef min
#undef max


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


	std::cout << "test";




	std::cin.clear();
	std::cin.get();
}