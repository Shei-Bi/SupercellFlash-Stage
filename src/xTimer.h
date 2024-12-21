#pragma once

class xTimer
{
public:
    static unsigned long long getNativeTime();
    static float getPassedTimeMs(unsigned long long, unsigned long long);
};
