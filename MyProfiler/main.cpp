#include <locale.h>
#include <stdio.h>
#include <vector>
#include <io.h>
#include <fcntl.h>

#include <Windows.h>
#include <timeapi.h>

#pragma comment(lib, "winmm.lib")

#include "my_profile.h"

void TestFunc2(int i2)
{
	Profile profile(L"TestFunc2");

	DWORD begin = timeGetTime();

	while (true)
	{
		if (timeGetTime() - begin >= 50)
			break;
	}
}

void TestFunc1(int i1, int i2)
{
	Profile profile(L"TestFunc1");

	DWORD begin = timeGetTime();

	while (true)
	{
		if (timeGetTime() - begin >= 50)
			break;
	}
	TestFunc2(i2);
}

void TestFunc3()
{
	Profile profile(L"TestFunc3");

	std::vector<int> vec;

	for (int i = 0; i < 100; ++i)
		vec.push_back(i);

	PRO_BEGIN(L"dsfsdklj");
}

void Test1()
{
	constexpr int length = 10;
	WCHAR* szTest1 = new WCHAR[length];
	wmemset(szTest1, L' ', length);
	szTest1[length - 1] = L'\0';

	CHAR* szText2 = new CHAR[length];
	memset(szText2, ' ', length);
	szText2[length - 1] = '\0';

	for (int i = 0; i < 100; ++i)
	{
		PRO_BEGIN(L"C wprintf");
		wprintf(szTest1);
		PRO_END(L"C wprintf");
	}

	//for (int i = 0; i < 100; ++i)
	//{
	//	PRO_BEGIN(L"C printf");
	//	printf(szText2);
	//	PRO_END(L"C printf");
	//}

	const WCHAR* beforeLocale = _wsetlocale(LC_ALL, nullptr);
	const WCHAR* locale = _wsetlocale(LC_ALL, L"");
	//	int ignore = _setmode(_fileno(stdout), _O_U16TEXT);

	UINT CP = GetConsoleOutputCP();

	for (int i = 0; i < 100; ++i)
	{
		PRO_BEGIN(L"16TEXT wprintf");
		wprintf(szTest1);
		PRO_END(L"16TEXT wprintf");
	}

	//for (int i = 0; i < 100; ++i)
	//{
	//	PRO_BEGIN(L"16TEXT printf");
	//	printf(szText2);
	//	PRO_END(L"16TEXT printf");
	//}

	PRO_DATAOUTTEXT(L"printf_profile.txt");
}

int wmain()
{
	timeBeginPeriod(1);

	{
		Test1();
	}

	//{
	//	DWORD start = timeGetTime();
	//	DWORD64 startClock = __rdtsc();

	//	while (true)
	//	{
	//		DWORD now = timeGetTime();
	//		if (now - start >= 1000)
	//		{
	//			start = now;
	//			DWORD64 nowClock = __rdtsc();

	//			double ghz = (nowClock - startClock) / 1'000'000'000.0;
	//			printf("%lfghz\n", ghz);
	//			startClock = nowClock;
	//		}
	//	}
	//}

	timeEndPeriod(1);
	return 0;
}