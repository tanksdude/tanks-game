//code from: https://github.com/TurtleMan64/usleep-windows
//slightly modified: moved the init out of the main function
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <Windows.h>
#include <winnt.h>

// High resolution timer only available on Windows 10 version 1803 or later.
#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define TIMER_CREATE_FLAGS 2
#else
#define TIMER_CREATE_FLAGS CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#endif

void usleep_windows_init()
{
    const HINSTANCE ntdll = LoadLibrary("ntdll.dll");
    if (ntdll != NULL)
    {
        typedef long(NTAPI* pNtQueryTimerResolution)(unsigned long* MinimumResolution, unsigned long* MaximumResolution, unsigned long* CurrentResolution);
        typedef long(NTAPI* pNtSetTimerResolution)(unsigned long RequestedResolution, char SetResolution, unsigned long* ActualResolution);

        pNtQueryTimerResolution NtQueryTimerResolution = (pNtQueryTimerResolution)GetProcAddress(ntdll, "NtQueryTimerResolution");
        pNtSetTimerResolution   NtSetTimerResolution   = (pNtSetTimerResolution)  GetProcAddress(ntdll, "NtSetTimerResolution");
        if (NtQueryTimerResolution != NULL &&
            NtSetTimerResolution   != NULL)
        {
            // Query for the highest accuracy timer resolution.
            unsigned long minimum, maximum, current;
            NtQueryTimerResolution(&minimum, &maximum, &current);
            
            // Set the timer resolution to the highest.
            NtSetTimerResolution(maximum, (char)1, &current);
        }

        // We can decrement the internal reference count by one
        // and NTDLL.DLL still remains loaded in the process.
        FreeLibrary(ntdll);
    }
}

void usleep_windows(long long microseconds)
{
    // Convert from microseconds to 100 of ns, and negative for relative time.
    LARGE_INTEGER sleepPeriod;
    sleepPeriod.QuadPart = -(10*microseconds);

    // Create the timer, sleep until time has passed, and clean up.
    HANDLE timer = CreateWaitableTimerEx(NULL, NULL, TIMER_CREATE_FLAGS, TIMER_ALL_ACCESS);
    SetWaitableTimer(timer, &sleepPeriod, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

#endif //_WIN32

#ifdef __cplusplus
}
#endif
