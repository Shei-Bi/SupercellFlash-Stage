#pragma once
#include "xTimer.h"
#include <string>

#ifndef abcdefg
#define TRACE(func) func
#else
#define TRACE(func) Trace::start(#func);\
func;\
Trace::end();
#endif
class Trace {
    static inline char func_name[1024];
    static inline unsigned long long start_time = 0;
public:
    static void start(const char* name) {
        start_time = xTimer::getNativeTime();
        strcpy(func_name, name);
    }
    static void end() {
        printf("%s: %d ms\n", func_name, (int)xTimer::getPassedTimeMs(start_time, xTimer::getNativeTime()));
    }
};