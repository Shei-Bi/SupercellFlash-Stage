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