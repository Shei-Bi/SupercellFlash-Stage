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

    int readPositiveInt(int bitLength);
    bool readBoolean();
    int readPositiveIntMax1();
    int readPositiveIntMax3();
    int readPositiveIntMax7();
    int readPositiveIntMax15();
    int readPositiveIntMax31();
    int readPositiveIntMax63();
    int readPositiveIntMax127();
    int readPositiveIntMax255();
    int readPositiveIntMax511();
    int readPositiveIntMax1023();
    int readPositiveIntMax2047();
    int readPositiveIntMax4095();
    int readPositiveIntMax8191();
    int readPositiveIntMax16383();
    int readPositiveIntMax32767();
    int readPositiveIntMax65535();
    int readPositiveIntMax131071();
    int readPositiveIntMax262143();
    int readPositiveIntMax524287();
    int readPositiveIntMax1048575();
    int readPositiveIntMax2097151();
    int readPositiveIntMax4194303();
    int readPositiveIntMax8388607();
    int readPositiveIntMax16777215();
    int readPositiveIntMax33554431();
    int readPositiveIntMax67108863();
    int readPositiveIntMax134217727();
    int readPositiveIntMax268435455();
    int readPositiveIntMax536870911();

    int readPositiveVInt(int bitLength);
    int readPositiveVIntOftenZero(int bitLength);
    int readPositiveVIntMax255();
    int readPositiveVIntMax255OftenZero();
    int readPositiveVIntMax65535();
    int readPositiveVIntMax65535OftenZero();
    int readPositiveVIntMax16777215();
    int readPositiveVIntMax16777215OftenZero();

    int readInt(int bitLength);
    int readIntMax1();
    int readIntMax3();
    int readIntMax7();
    int readIntMax15();
    int readIntMax31();
    int readIntMax63();
    int readIntMax127();
    int readIntMax255();
    int readIntMax511();
    int readIntMax1023();
    int readIntMax2047();
    int readIntMax4095();
    int readIntMax8191();
    int readIntMax16383();
    int readIntMax32767();
    int readIntMax65535();
    int readIntMax131071();
    int readIntMax262143();
    int readIntMax524287();
    int readIntMax1048575();
    int readIntMax2097151();
    int readIntMax4194303();
    int readIntMax8388607();
    int readIntMax16777215();
    int readIntMax33554431();
    int readIntMax67108863();
    int readIntMax134217727();
    int readIntMax268435455();
    int readIntMax536870911();

    void rewind(int bitLength);

    int debugGetZeroBitsLength();
};