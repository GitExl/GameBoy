#include "renderlib.h"
#include "windows.h"
#include "timer.h"


static double        timerFrequency;
static LARGE_INTEGER  timerCount;

static uint32_t minMS;
static uint32_t delayFudge = 0;


// Return the number of microseconds elapsed since system start
// Might not be actually microsecond-accurate, but more accurate than SDL_GetTicks or GetTickCount.
//
inline uint64_t timerTicks()
{
    QueryPerformanceCounter(&timerCount);
    return (uint64_t)timerCount.QuadPart / timerFrequency;
}

// Initialize timer frequency value
//
uint32_t timerInit()
{
    LARGE_INTEGER Frequency;
    TIMECAPS Caps;


    // Get minimum millisecond resolution of system timer
    timeGetDevCaps(&Caps, sizeof(TIMECAPS));
    minMS = Caps.wPeriodMin;

    // Retrieve resolution of performance timer
    QueryPerformanceFrequency(&Frequency);
    if (Frequency.QuadPart == 0)
        return 0;

    //timerFrequency = (Frequency.QuadPart / 1000000);
    timerFrequency = (double)Frequency.QuadPart / 1000000;

    return 1;
}

// Delay for a certain amount of time
//
void timerDelay(int64_t Delay)
{
    uint64_t Timing;
    int64_t delayCount;
    uint64_t delayStart;
    MSG curMsg;


    // Do not wait if the delay is negative (ie infinite)
    if (Delay <= 0)
        return;

    // Suspend the process for a number of milliseconds
    // The Sleep function will always resume the thread after _more_ than the specified number of ms.
    // To counteract this, we measure how long the wait actually took. If it took longer than the
    // total delay, we wait a ms less next time. The actual delay is decreased by the actual time we
    // waited during thread suspension, not what we requested.
    delayCount = Delay / 1000 - delayFudge;
    if (delayCount >= minMS)
    {
        delayStart = timerTicks();
        Sleep(delayCount);
        Delay -= timerTicks() - delayStart;

        if (Delay <= 0)
        {
            delayFudge++;
            return;
        }
    }

    // Keep processing messages while waiting the remaining time
    Timing = timerTicks();
    while(timerTicks() - Timing < Delay)
    {
        if(PeekMessage(&curMsg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&curMsg);
            DispatchMessage(&curMsg);
        }
    }
}
