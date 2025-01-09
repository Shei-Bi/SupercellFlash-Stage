#pragma once
#include <string>
#include "data/LogicDataTables.h"

class BitStream {
private:
    unsigned char* buffer;
    int offset;
    int bitOffset;
    int capacity;
public:
    BitStream(int initialCapacity);
    BitStream(unsigned char* buffer, int length);
    ~BitStream();
    unsigned char* getByteArray();
    int getLength();
    void ensureCapacity();
    void resetOffset();

    void writePositiveInt(int value, int bitLength);
    void writeBoolean(bool value);
    void writePositiveIntMax1(int value);
    void writePositiveIntMax3(int value);
    void writePositiveIntMax7(int value);
    void writePositiveIntMax15(int value);
    void writePositiveIntMax31(int value);
    void writePositiveIntMax63(int value);
    void writePositiveIntMax127(int value);
    void writePositiveIntMax255(int value);
    void writePositiveIntMax511(int value);
    void writePositiveIntMax1023(int value);
    void writePositiveIntMax2047(int value);
    void writePositiveIntMax4095(int value);
    void writePositiveIntMax8191(int value);
    void writePositiveIntMax16383(int value);
    void writePositiveIntMax32767(int value);
    void writePositiveIntMax65535(int value);
    void writePositiveIntMax131071(int value);
    void writePositiveIntMax262143(int value);
    void writePositiveIntMax524287(int value);
    void writePositiveIntMax1048575(int value);
    void writePositiveIntMax2097151(int value);
    void writePositiveIntMax4194303(int value);
    void writePositiveIntMax8388607(int value);
    void writePositiveIntMax16777215(int value);
    void writePositiveIntMax33554431(int value);
    void writePositiveIntMax67108863(int value);
    void writePositiveIntMax134217727(int value);
    void writePositiveIntMax268435455(int value);
    void writePositiveIntMax536870911(int value);
};