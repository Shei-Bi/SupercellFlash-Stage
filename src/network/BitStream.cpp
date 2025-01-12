#include "BitStream.h"
#include <algorithm>
#include "assert.h"

BitStream::BitStream(int initialCapacity) {
    this->buffer = new unsigned char[initialCapacity];
    this->capacity = initialCapacity;
    this->offset = 0;
    bitOffset = 0;
}

BitStream::BitStream(unsigned char* buffer, int length) {
    this->buffer = buffer;
    this->capacity = length;
    this->offset = 0;
    bitOffset = 0;
}

BitStream::~BitStream() {
    delete[] this->buffer;
}

unsigned char* BitStream::getByteArray() {
    return buffer;
}

int BitStream::getLength() {
    return this->offset + 1;
}

void BitStream::resetOffset() {
    this->offset = 0;
    this->bitOffset = 0;
}

void BitStream::ensureCapacity() {
    if (this->offset + 6 > this->capacity) {
        capacity += 105;
        unsigned char* tmpBuffer = new unsigned char[capacity];
        memcpy(tmpBuffer, this->buffer, getLength());
        delete[] this->buffer;
        this->buffer = tmpBuffer;
    }
}

void BitStream::writePositiveInt(int value, int bitLength) {
    int i = std::clamp(value, 0, (1 << bitLength) - 1);
    assert(i == value);
    value = i;
    ensureCapacity();
    if (!bitLength) return;
    if (!offset) buffer[0] = (unsigned char)0;
    for (int bit = 0;bit < bitLength;bit++)
    {
        buffer[offset] |= ((1 << bit) & value) >> bit << bitOffset;
        bitOffset++;
        if (bitOffset == 8) {
            bitOffset = 0;
            offset++;
            buffer[offset] = (unsigned char)0;
        }
    }
}

void BitStream::writeBoolean(bool value) {
    writePositiveInt(value ? 1 : 0, 1);
}

void BitStream::writePositiveIntMax1(int value) {
    writePositiveInt(value, 1);
}

void BitStream::writePositiveIntMax3(int value) {
    writePositiveInt(value, 2);
}

void BitStream::writePositiveIntMax7(int value) {
    writePositiveInt(value, 3);
}

void BitStream::writePositiveIntMax15(int value) {
    writePositiveInt(value, 4);
}
void BitStream::writePositiveIntMax31(int value) {
    writePositiveInt(value, 5);
}
void BitStream::writePositiveIntMax63(int value) {
    writePositiveInt(value, 6);
}
void BitStream::writePositiveIntMax127(int value) {
    writePositiveInt(value, 7);
}
void BitStream::writePositiveIntMax255(int value) {
    writePositiveInt(value, 8);
}
void BitStream::writePositiveIntMax511(int value) {
    writePositiveInt(value, 9);
}
void BitStream::writePositiveIntMax1023(int value) {
    writePositiveInt(value, 10);
}
void BitStream::writePositiveIntMax2047(int value) {
    writePositiveInt(value, 11);
}
void BitStream::writePositiveIntMax4095(int value) {
    writePositiveInt(value, 12);
}
void BitStream::writePositiveIntMax8191(int value) {
    writePositiveInt(value, 13);
}
void BitStream::writePositiveIntMax16383(int value) {
    writePositiveInt(value, 14);
}
void BitStream::writePositiveIntMax32767(int value) {
    writePositiveInt(value, 15);
}
void BitStream::writePositiveIntMax65535(int value) {
    writePositiveInt(value, 16);
}
void BitStream::writePositiveIntMax131071(int value) {
    writePositiveInt(value, 17);
}
void BitStream::writePositiveIntMax262143(int value) {
    writePositiveInt(value, 18);
}
void BitStream::writePositiveIntMax524287(int value) {
    writePositiveInt(value, 19);
}
void BitStream::writePositiveIntMax1048575(int value) {
    writePositiveInt(value, 20);
}
void BitStream::writePositiveIntMax2097151(int value) {
    writePositiveInt(value, 21);
}
void BitStream::writePositiveIntMax4194303(int value) {
    writePositiveInt(value, 22);
}
void BitStream::writePositiveIntMax8388607(int value) {
    writePositiveInt(value, 23);
}
void BitStream::writePositiveIntMax16777215(int value) {
    writePositiveInt(value, 24);
}
void BitStream::writePositiveIntMax33554431(int value) {
    writePositiveInt(value, 25);
}
void BitStream::writePositiveIntMax67108863(int value) {
    writePositiveInt(value, 26);
}
void BitStream::writePositiveIntMax134217727(int value) {
    writePositiveInt(value, 27);
}
void BitStream::writePositiveIntMax268435455(int value) {
    writePositiveInt(value, 28);
}
void BitStream::writePositiveIntMax536870911(int value) {
    writePositiveInt(value, 29);
}

int BitStream::readPositiveInt(int bitLength) {
    if (!bitLength) return 0;
    int result = 0;
    for (int bit = 0;bit < bitLength;bit++)
    {
        result |= ((buffer[offset] >> bitOffset) & 1) << bit;
        bitOffset++;
        if (bitOffset == 8) {
            bitOffset = 0;
            offset++;
        }
    }
    return result;
}

bool BitStream::readBoolean() {
    return readPositiveInt(1) == 1;
}

int BitStream::readPositiveIntMax1() {
    return readPositiveInt(1);
}

int BitStream::readPositiveIntMax3() {
    return readPositiveInt(2);
}

int BitStream::readPositiveIntMax7() {
    return readPositiveInt(3);
}

int BitStream::readPositiveIntMax15() {
    return readPositiveInt(4);
}
int BitStream::readPositiveIntMax31() {
    return readPositiveInt(5);
}
int BitStream::readPositiveIntMax63() {
    return readPositiveInt(6);
}
int BitStream::readPositiveIntMax127() {
    return readPositiveInt(7);
}
int BitStream::readPositiveIntMax255() {
    return readPositiveInt(8);
}
int BitStream::readPositiveIntMax511() {
    return readPositiveInt(9);
}
int BitStream::readPositiveIntMax1023() {
    return readPositiveInt(10);
}
int BitStream::readPositiveIntMax2047() {
    return readPositiveInt(11);
}
int BitStream::readPositiveIntMax4095() {
    return readPositiveInt(12);
}
int BitStream::readPositiveIntMax8191() {
    return readPositiveInt(13);
}
int BitStream::readPositiveIntMax16383() {
    return readPositiveInt(14);
}
int BitStream::readPositiveIntMax32767() {
    return readPositiveInt(15);
}
int BitStream::readPositiveIntMax65535() {
    return readPositiveInt(16);
}
int BitStream::readPositiveIntMax131071() {
    return readPositiveInt(17);
}
int BitStream::readPositiveIntMax262143() {
    return readPositiveInt(18);
}
int BitStream::readPositiveIntMax524287() {
    return readPositiveInt(19);
}
int BitStream::readPositiveIntMax1048575() {
    return readPositiveInt(20);
}
int BitStream::readPositiveIntMax2097151() {
    return readPositiveInt(21);
}
int BitStream::readPositiveIntMax4194303() {
    return readPositiveInt(22);
}
int BitStream::readPositiveIntMax8388607() {
    return readPositiveInt(23);
}
int BitStream::readPositiveIntMax16777215() {
    return readPositiveInt(24);
}
int BitStream::readPositiveIntMax33554431() {
    return readPositiveInt(25);
}
int BitStream::readPositiveIntMax67108863() {
    return readPositiveInt(26);
}
int BitStream::readPositiveIntMax134217727() {
    return readPositiveInt(27);
}
int BitStream::readPositiveIntMax268435455() {
    return readPositiveInt(28);
}
int BitStream::readPositiveIntMax536870911() {
    return readPositiveInt(29);
}

int BitStream::readPositiveVInt(int bitLength) {
    return readPositiveInt(readPositiveInt(bitLength));
}

int BitStream::readPositiveVIntOftenZero(int bitLength) {
    if (readPositiveIntMax1() == 1) return 0;
    return readPositiveInt(readPositiveInt(bitLength));
}
int BitStream::readPositiveVIntMax255() {
    return readPositiveVInt(3);
}
int BitStream::readPositiveVIntMax255OftenZero() {
    return readPositiveVIntOftenZero(3);
}
int BitStream::readPositiveVIntMax65535() {
    return readPositiveVInt(4);
}
int BitStream::readPositiveVIntMax65535OftenZero() {
    return readPositiveVIntOftenZero(4);
}
int BitStream::readPositiveVIntMax16777215() {
    return readPositiveVInt(5);
}
int BitStream::readPositiveVIntMax16777215OftenZero() {
    return readPositiveVIntOftenZero(5);
}

int BitStream::readInt(int bitLength) {
    int sign = readPositiveIntMax1();
    return (sign ? 1 : -1) * readPositiveInt(bitLength);
}

int BitStream::readIntMax1() {
    return readInt(1);
}

int BitStream::readIntMax3() {
    return readInt(2);
}

int BitStream::readIntMax7() {
    return readInt(3);
}

int BitStream::readIntMax15() {
    return readInt(4);
}
int BitStream::readIntMax31() {
    return readInt(5);
}
int BitStream::readIntMax63() {
    return readInt(6);
}
int BitStream::readIntMax127() {
    return readInt(7);
}
int BitStream::readIntMax255() {
    return readInt(8);
}
int BitStream::readIntMax511() {
    return readInt(9);
}
int BitStream::readIntMax1023() {
    return readInt(10);
}
int BitStream::readIntMax2047() {
    return readInt(11);
}
int BitStream::readIntMax4095() {
    return readInt(12);
}
int BitStream::readIntMax8191() {
    return readInt(13);
}
int BitStream::readIntMax16383() {
    return readInt(14);
}
int BitStream::readIntMax32767() {
    return readInt(15);
}
int BitStream::readIntMax65535() {
    return readInt(16);
}
int BitStream::readIntMax131071() {
    return readInt(17);
}
int BitStream::readIntMax262143() {
    return readInt(18);
}
int BitStream::readIntMax524287() {
    return readInt(19);
}
int BitStream::readIntMax1048575() {
    return readInt(20);
}
int BitStream::readIntMax2097151() {
    return readInt(21);
}
int BitStream::readIntMax4194303() {
    return readInt(22);
}
int BitStream::readIntMax8388607() {
    return readInt(23);
}
int BitStream::readIntMax16777215() {
    return readInt(24);
}
int BitStream::readIntMax33554431() {
    return readInt(25);
}
int BitStream::readIntMax67108863() {
    return readInt(26);
}
int BitStream::readIntMax134217727() {
    return readInt(27);
}
int BitStream::readIntMax268435455() {
    return readInt(28);
}
int BitStream::readIntMax536870911() {
    return readInt(29);
}

void BitStream::rewind(int bitLength) {
    while (bitLength) {
        if (bitOffset == 0) {
            offset--;
            bitOffset = 8;
        }
        bitOffset--;
        bitLength--;
    }
}

int BitStream::debugGetZeroBitsLength() {
    int result = 0;
    while (readIntMax1() == 0) result++;
    rewind(result + 1);
    return result;
}
