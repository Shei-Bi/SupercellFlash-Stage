#pragma once
class LogicLong {
public:
    static int getHigherInt(long long value) {
        return value >> 32;
    }
    static int getLowerInt(long long value) {
        return (int)value;
    }
    static long long toLong(int high, int low) {
        return ((long long)high << 32) | (unsigned int)low;
    }
};