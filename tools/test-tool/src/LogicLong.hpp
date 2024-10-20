#pragma once
#define HIDWORD(x) (*((int*)&(x)+1))
class LogicLong {
public:
    static int getHigherInt(long long value) {
        return HIDWORD(value);
    }
    static int getLowerInt(long long value) {
        if (value >= 0)
            return value & 0x7FFFFFFF;
        else
            return value | 0x80000000;
    }
    static long long toLong(int high, int low) {
        int v2 = low | 0x80000000;
        if (low >= 0)
            v2 = low;
        return v2 | ((long long)high << 32);
    }
};