#include <locale.h>
#include <stdio.h>
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
        if (timeGetTime() - begin >= 500)
            break;
    }
}

void TestFunc1(int i1, int i2)
{
    Profile profile(L"TestFunc1");
    
    DWORD begin = timeGetTime();

    while (true)
    {
        if (timeGetTime() - begin >= 500)
            break;
    }
    TestFunc2(i2);
}

int wmain()
{
    timeBeginPeriod(1);

    PRO_RESET();

    for (int i = 0; i < 100; ++i)
    {
        TestFunc1(i , i + 1);
    }

    PRO_DATAOUTTEXT(L"profile4.txt");

    timeEndPeriod(1);

    return 0;
}