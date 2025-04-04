//code from: https://github.com/aprilyxx/common/blob/master/time_util.cc
//slightly modified: uint32 -> long; removed errno check
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#include <time.h>

void SleepInMs_posix(long ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = ms % 1000 * 1000000;

    nanosleep(&ts, &ts);
}

void SleepInUs_posix(long us) {
    struct timespec ts;
    ts.tv_sec = us / 1000000;
    ts.tv_nsec = us % 1000000 * 1000;

    nanosleep(&ts, &ts);
}

#endif //_WIN32

#ifdef __cplusplus
}
#endif
