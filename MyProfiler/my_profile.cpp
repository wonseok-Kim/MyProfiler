#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include "my_profile.h"

static constexpr size_t TAG_LENGTH = 64;

struct ProfileData
{
    bool bFlag;
    wchar_t tag[TAG_LENGTH + 1];

    LARGE_INTEGER startTime;

    INT64 totalTime;
    INT64 min[2];  // [0]: 가장 최소, [1]: 다음 최소
    INT64 max[2];  // [0]: 가장 최대, [1]: 다음 최대

    INT64 callCount;
};

class ProfileManager
{
public:
    ProfileManager()
    {
        QueryPerformanceFrequency(&m_Freq);

        for (int i = 0; i < MAX_DATA; ++i)
        {
            m_DataList[i].min[0] = MAXINT64;
            m_DataList[i].min[1] = MAXINT64;
        }
    }

    void Begin(const wchar_t* tagName)
    {
        ProfileData* p;

        int idx = FindIdx(tagName);
        if (idx < 0)
        {
            if (m_DataCount > MAX_DATA - 1)
            {
                // TODO: 에러 처리
                return;
            }
            p = &m_DataList[m_DataCount++];
            p->bFlag = true;
            wcscpy_s(p->tag, _countof(p->tag), tagName);
        }
        else
        {
            p = &m_DataList[idx];
        }

        QueryPerformanceCounter(&p->startTime);
    }

    void End(const wchar_t* tagName)
    {
        LARGE_INTEGER end;
        QueryPerformanceCounter(&end);

        int idx = FindIdx(tagName);
        if (idx < 0)
        {
            // TODO: 에러처리 
            return;
        }
        ProfileData* p = &m_DataList[idx];

        INT64 elapsedMicroseconds = (end.QuadPart - p->startTime.QuadPart) * 100'000'0;
        elapsedMicroseconds /= m_Freq.QuadPart;

        p->totalTime += elapsedMicroseconds;
        p->callCount++;

        if (p->min[1] > elapsedMicroseconds)
        {
            if (p->min[0] > elapsedMicroseconds)
                p->min[0] = elapsedMicroseconds;
            else
                p->min[1] = elapsedMicroseconds;
        }
        else if (p->max[1] < elapsedMicroseconds)
        {
            if (p->max[0] < elapsedMicroseconds)
                p->max[0] = elapsedMicroseconds;
            else
                p->max[1] = elapsedMicroseconds;
        }
    }

    void SaveDataToFile(const wchar_t* filename)
    {
        assert(filename);

        FILE* pFile;
        errno_t result = _wfopen_s(&pFile, filename, L"w");
        if (result != 0 || pFile == nullptr)
        {
            // TODO: 에러 처리
            return;
        }

        fwprintf(pFile, L"---------------------------------------------------------------------------------------\n");
        fwprintf(pFile, L"%19s\t| %10s\t| %10s\t| %10s\t| %9s\t|\n", L"Name", L"Average", L"Min", L"Max", L"Call");
        fwprintf(pFile, L"---------------------------------------------------------------------------------------\n");
        for (int i = 0; i < m_DataCount; ++i)
        {
            ProfileData* p = &m_DataList[i];

            double average;
            if (p->callCount > 4)
            {
                INT64 total = p->totalTime - (p->min[0] + p->min[1] + p->max[0] + p->max[1]);
                average = (double)total / (p->callCount - 4);
            }
            else
            {
                average = (double)p->totalTime / p->callCount;
            }

            fwprintf(pFile, L"%20s\t| %10.4lf\xE3\x8E\xB2\t| %10lld\xE3\x8E\xB2\t| %10lld\xE3\x8E\xB2\t| %10lld\n",
                p->tag,
                average, 
                p->min[0],
                p->max[0],
                p->callCount);
        }
        fwprintf(pFile, L"---------------------------------------------------------------------------------------\n");

        fclose(pFile);
    }

    void Reset()
    {
        ZeroMemory(m_DataList, sizeof(m_DataList));
        for (int i = 0; i < MAX_DATA; ++i)
        {
            m_DataList[i].min[0] = MAXINT64;
            m_DataList[i].min[1] = MAXINT64;
        }

        m_DataCount = 0;
    }

private:
    int FindIdx(const wchar_t* tagName)
    {
        for (int i = 0; i < m_DataCount; ++i)
        {
            if (m_DataList[i].bFlag &&
                wcscmp(m_DataList[i].tag, tagName) == 0)
            {
                return i;
            }
        }
        return -1;
    }

private:
    static constexpr int MAX_DATA = 50;

    ProfileData m_DataList[MAX_DATA];
    int m_DataCount = 0;

    LARGE_INTEGER m_Freq;
};

static ProfileManager s_Profiler;

void ProfileBegin(const wchar_t* tag)
{
    s_Profiler.Begin(tag);
}

void ProfileEnd(const wchar_t* tag)
{
    s_Profiler.End(tag);
}

void ProfileDataOutText(const wchar_t* fileName)
{
    s_Profiler.SaveDataToFile(fileName);
}

void ProfileReset(void)
{
    s_Profiler.Reset();
}
