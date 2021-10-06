#include <iostream>
#include <cstdio>

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

	
	std::cout << "hello world?\n";

	system("pause");
	return 0;
}