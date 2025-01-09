#pragma once
#include <string>
#include "data/LogicDataTables.h"

class ByteStream {
private:
    char* buffer;
    int offset;
    int length;
    int bitOffset;
public:
    ByteStream(int initialCapacity);
    ByteStream(unsigned char* buffer, int length);
    ~ByteStream();
    char* getByteArray();
    void setByteArray(char* buffer, int length);
    int getLength();
    int getOffset();
    void setOffset(int);
    char* getDataPointer();
    void ensureCapacity(int capacity);
    bool isAtEnd();
    void resetOffset();

    char readByte();
    bool readBoolean();
    short readShort();
    int readInt();
    long long readLongLong();
    long long readVLong();
    int readVInt();
    void readBytes(char* output, int length);
    char* readBytes(int length, int maxCapacity);
    std::string* readString();
    void readString(std::string* str);
    const char* readString(int length);
    LogicData* readDataReference();

    void writeByte(char value);
    void writeBoolean(bool value);
    void writeShort(short value);
    void writeInt(int value);
    void writeLongLong(long long);
    void writeVInt(int value);
    void writeBytes(char* buffer, int length);
    void writeBytesWithoutLength(unsigned char* buffer, int length);
    void writeString(std::string* str);
    void writeString(const char* str);
    void writeDataReference(LogicData* logicData);
};