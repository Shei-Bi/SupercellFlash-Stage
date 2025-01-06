#include "ByteStream.h"
#include <string.h>
#include <LogicLong.hpp>
#include "GlobalID.h"

ByteStream::ByteStream(int initialCapacity) {
    this->buffer = new char[initialCapacity];
    this->length = initialCapacity;
    this->offset = 0;
    this->bitOffset = 0;
}

ByteStream::~ByteStream() {
    delete[] this->buffer;
}

bool ByteStream::isAtEnd() {
    return this->offset >= this->length;
}

char* ByteStream::getByteArray() {
    return buffer;
}

void ByteStream::setByteArray(char* buffer, int length) {
    if (this->buffer) {
        delete[] this->buffer;
    }
    this->buffer = buffer;
    this->length = length;
    this->bitOffset = 0;
    this->offset = 0;
}

int ByteStream::getLength() {
    return this->length;
}

int ByteStream::getOffset() {
    return this->offset;
}

void ByteStream::setOffset(int o) {
    this->offset = o;
}

void ByteStream::resetOffset() {
    this->offset = 0;
}

char* ByteStream::getDataPointer() {
    return this->buffer + this->offset;
}

void ByteStream::ensureCapacity(int capacity) {
    int bufferLength = this->length;
    if (this->offset + capacity > this->length) {
        char* tmpBuffer = new char[bufferLength + capacity + 100];
        memcpy(tmpBuffer, this->buffer, this->length);
        delete[] this->buffer;
        this->buffer = tmpBuffer;
        this->length = bufferLength + capacity + 100;
    }
}

// Read Methods
char ByteStream::readByte() {
    this->bitOffset = 0;
    if (isAtEnd()) {
        return 0;
    }
    return buffer[offset++];
}

bool ByteStream::readBoolean() {
    if (this->bitOffset == 0) {
        if (isAtEnd()) {
            return false;
        }
        ++this->offset;
    }

    bool value = (this->buffer[this->offset - 1] & (1 << this->bitOffset)) != 0;
    this->bitOffset = (this->bitOffset + 1) & 7;
    return value;
}

short ByteStream::readShort() {
    this->bitOffset = 0;
    if (offset + 2 - 1 > length) {
        return 0;
    }

    short retval = (short)((unsigned char)(this->buffer[this->offset] << 8) |
        this->buffer[this->offset + 1] & 0xFF);
    this->offset += 2;
    return retval;
}

int ByteStream::readInt() {
    this->bitOffset = 0;
    if (offset + 4 - 1 > length) {
        return 0;
    }

    int retval = ((unsigned char)this->buffer[this->offset] << 24) |
        ((unsigned char)this->buffer[this->offset + 1] << 16) |
        ((unsigned char)this->buffer[this->offset + 2] << 8) |
        this->buffer[this->offset + 3] & 0xFF;
    this->offset += 4;
    return retval;
}

long long ByteStream::readLongLong() {
    int high = readInt();
    return LogicLong::toLong(high, readInt());
}

long long ByteStream::readVLong() {
    int high = readVInt();
    return LogicLong::toLong(high, readVInt());
}

int ByteStream::readVInt() {
    this->bitOffset = 0;
    int value = 0;

    if (offset > length) {
        return 0;
    }

    unsigned char byteValue = this->buffer[this->offset];
    this->offset++;

    if ((byteValue & 0x40) != 0)
    {
        value |= byteValue & 0x3F;

        if ((byteValue & 0x80) != 0)
        {
            if (offset > length) {
                return 0;
            }

            value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 6;
            this->offset++;

            if ((byteValue & 0x80) != 0)
            {
                if (offset > length) {
                    return 0;
                }

                value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 13;
                this->offset++;

                if ((byteValue & 0x80) != 0)
                {
                    if (offset > length) {
                        return 0;
                    }

                    value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 20;
                    this->offset++;

                    if ((byteValue & 0x80) != 0)
                    {
                        if (offset > length) {
                            return 0;
                        }

                        value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 27;
                        this->offset++;
                        return (int)(value | 0x80000000);
                    }

                    return (int)(value | 0xF8000000);
                }

                return (int)(value | 0xFFF00000);
            }

            return (int)(value | 0xFFFFE000);
        }

        return (int)(value | 0xFFFFFFC0);
    }

    value |= byteValue & 0x3F;

    if ((byteValue & 0x80) != 0)
    {
        if (offset > length) {
            return 0;
        }

        value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 6;
        this->offset++;

        if ((byteValue & 0x80) != 0)
        {
            if (offset > length) {
                return 0;
            }

            value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 13;
            this->offset++;

            if ((byteValue & 0x80) != 0)
            {
                if (offset > length) {
                    return 0;
                }

                value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 20;
                this->offset++;

                if ((byteValue & 0x80) != 0)
                {
                    if (offset > length) {
                        return 0;
                    }

                    value |= ((byteValue = this->buffer[this->offset]) & 0x7F) << 27;
                    this->offset++;
                }
            }
        }
    }

    return value;
}

void ByteStream::readBytes(char* output, int length) {
    this->bitOffset = 0;
    if (length <= 0) return;

    if (offset + length - 1 > this->length) {
        return;
    }

    if (length <= 900000) {
        memcpy(output, buffer + offset, length);
        this->offset += length;
    }
}

char* ByteStream::readBytes(int length, int maxCapacity) {
    this->bitOffset = 0;

    if (offset + length - 1 > this->length) {
        return NULL;
    }

    if (length <= -1) {
        return NULL;
    }
    else if (length <= maxCapacity) {
        char* result = new char[length];
        memcpy(result, this->buffer + this->offset, length);
        this->offset += length;
        return result;
    }
    else {
        // Debugger::warning("readBytes too long array, max " + maxCapacity);
        return NULL;
    }
}

std::string* ByteStream::readString() {
    int length = this->readInt();

    if (offset + length - 1 > this->length) {
        return NULL;
    }

    if (length < 0 || length > 900000) {
        return NULL;
    }
    else if (length == 0) {
        return new std::string("");
    }
    else {
        std::string* retval = new std::string(buffer + offset, length);
        this->offset += length;
        return retval;
    }
}

void ByteStream::readString(std::string* str) {
    int length = this->readInt();

    if (offset + length - 1 > this->length) {
        return;
    }

    if (length < 0 || length > 900000) {
        return;
    }
    else if (length == 0) {
        str->assign("");
    }
    else {
        str->assign(buffer + offset, length);
        this->offset += length;
    }
}

const char* ByteStream::readString(int maxLength) {
    int length = this->readInt();

    if (length > maxLength) abort();

    if (offset + length - 1 > this->length) {
        return nullptr;
    }

    if (length < 0 || length > 900000) {
        return nullptr;
    }

    if (length == 0) {
        const char* str = new char[1];
        memcpy((void*)str, "", 1);
        return str;
    }
    else {
        const char* str = new char[length + 1];
        memcpy((void*)str, buffer + offset, length);
        ((char*)str)[length] = (char)0;
        this->offset += length;
        return str;
    }
}

LogicData* ByteStream::readDataReference() {
    int classID = readVInt();
    if (classID == 0) return nullptr;
    if (classID == 1) abort();
#ifndef NDEBUG
    if (TABLES[classID] == nullptr) {
        printf("table #%d not initialized! (accessing %d)\n", classID, readVInt());
        return nullptr;
    }
#endif
    return LogicDataTables::getDataById(GlobalID::createGlobalID(classID, readVInt()));
}


// Write Methods
void ByteStream::writeByte(char value) {
    this->bitOffset = 0;
    this->ensureCapacity(1);

    this->buffer[this->offset++] = value;
}

void ByteStream::writeDataReference() {
    writeVInt(0);
}

void ByteStream::writeBoolean(bool value) {
    if (this->bitOffset == 0)
    {
        this->ensureCapacity(1);
        this->buffer[this->offset++] = 0;
    }

    if (value)
    {
        this->buffer[this->offset - 1] |= (char)(1 << this->bitOffset);
    }

    this->bitOffset = (this->bitOffset + 1) & 7;
}

void ByteStream::writeShort(short value) {
    this->bitOffset = 0;
    this->ensureCapacity(2);

    this->buffer[this->offset++] = (char)(value >> 8);
    this->buffer[this->offset++] = (char)value;
}


void ByteStream::writeInt(int value) {
    this->bitOffset = 0;
    this->ensureCapacity(4);

    this->buffer[this->offset++] = (char)(value >> 24);
    this->buffer[this->offset++] = (char)(value >> 16);
    this->buffer[this->offset++] = (char)(value >> 8);
    this->buffer[this->offset++] = (char)value;
}

void ByteStream::writeLongLong(long long value) {
    writeInt(LogicLong::getHigherInt(value));
    writeInt(LogicLong::getLowerInt(value));
}

void ByteStream::writeVInt(int value) {
    this->bitOffset = 0;
    this->ensureCapacity(5);

    if (value >= 0)
    {
        if (value >= 64)
        {
            if (value >= 0x2000)
            {
                if (value >= 0x100000)
                {
                    if (value >= 0x8000000)
                    {
                        this->buffer[this->offset++] = (char)((value & 0x3F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 6) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 13) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 20) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)((value >> 27) & 0xF);
                    }
                    else
                    {
                        this->buffer[this->offset++] = (char)((value & 0x3F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 6) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 13) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)((value >> 20) & 0x7F);
                    }
                }
                else
                {
                    this->buffer[this->offset++] = (char)((value & 0x3F) | 0x80);
                    this->buffer[this->offset++] = (char)(((value >> 6) & 0x7F) | 0x80);
                    this->buffer[this->offset++] = (char)((value >> 13) & 0x7F);
                }
            }
            else
            {
                this->buffer[this->offset++] = (char)((value & 0x3F) | 0x80);
                this->buffer[this->offset++] = (char)((value >> 6) & 0x7F);
            }
        }
        else
        {
            this->buffer[this->offset++] = (char)(value & 0x3F);
        }
    }
    else
    {
        if (value <= -0x40)
        {
            if (value <= -0x2000)
            {
                if (value <= -0x100000)
                {
                    if (value <= -0x8000000)
                    {
                        this->buffer[this->offset++] = (char)((value & 0x3F) | 0xC0);
                        this->buffer[this->offset++] = (char)(((value >> 6) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 13) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 20) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)((value >> 27) & 0xF);
                    }
                    else
                    {
                        this->buffer[this->offset++] = (char)((value & 0x3F) | 0xC0);
                        this->buffer[this->offset++] = (char)(((value >> 6) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)(((value >> 13) & 0x7F) | 0x80);
                        this->buffer[this->offset++] = (char)((value >> 20) & 0x7F);
                    }
                }
                else
                {
                    this->buffer[this->offset++] = (char)((value & 0x3F) | 0xC0);
                    this->buffer[this->offset++] = (char)(((value >> 6) & 0x7F) | 0x80);
                    this->buffer[this->offset++] = (char)((value >> 13) & 0x7F);
                }
            }
            else
            {
                this->buffer[this->offset++] = (char)((value & 0x3F) | 0xC0);
                this->buffer[this->offset++] = (char)((value >> 6) & 0x7F);
            }
        }
        else
        {
            this->buffer[this->offset++] = (char)((value & 0x3F) | 0x40);
        }
    }
}

void ByteStream::writeBytes(char* buffer, int length) {
    this->bitOffset = 0;

    this->writeInt(length);
    if (length <= 0 || length > 900000) return;

    this->ensureCapacity(length);
    memcpy(this->buffer + offset, buffer, length);
    this->offset += length;
}

void ByteStream::writeString(std::string* str) {
    this->bitOffset = 0;

    if (str == NULL) {
        this->writeInt(-1);
        return;
    }

    this->writeInt(str->length());

    this->ensureCapacity(str->length());
    memcpy(this->buffer + offset, str->c_str(), str->length()); // copying all, except final '\0'
    this->offset += str->length();
}

void ByteStream::writeString(const char* str) {
    this->bitOffset = 0;

    if (str == NULL) {
        this->writeInt(-1);
        return;
    }

    this->writeInt(strlen(str));

    this->ensureCapacity(strlen(str));
    memcpy(this->buffer + offset, str, strlen(str)); // copying all, except final '\0'
    this->offset += strlen(str);
}
