#include <locale.h>
#include <stdio.h>
#include <vector>

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

int wmain()
{
	timeBeginPeriod(1);

	{
		//PRO_RESET();

		for (int i = 0; i < 100; ++i)
		{
			TestFunc1(i, i + 1);
			TestFunc3();
		}

		PRO_BEGIN(L"test");

		PRO_DATAOUTTEXT(L"testProfile.txt");
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