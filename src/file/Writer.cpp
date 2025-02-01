#include "file/Writer.hpp"
#include "util/bitcast.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>

using namespace Conv3d;

void Writer::EnsureCapacity(int size)
{
    if (this->offset + size >= this->size)
    {
        int newSize = std::max(this->size * 2, this->size + size);
        uint8_t *newBuffer = new uint8_t[newSize];
        memcpy(newBuffer, this->buffer, this->offset);
        delete this->buffer;
        this->buffer = newBuffer;
        this->size = newSize;
    }
}

Writer::~Writer()
{
    if (this->buffer != nullptr)
        delete this->buffer;
}

void Writer::WriteU8(uint8_t byte)
{
    this->EnsureCapacity(1);
    buffer[offset++] = byte;
}

void Writer::WriteU16(uint16_t value)
{
    this->WriteU8(value >> 8);
    this->WriteU8(value & 0xFF);
}

void Writer::WriteU32(uint32_t value)
{
    this->WriteU8(value >> 24);
    this->WriteU8(value >> 16);
    this->WriteU8(value >> 8);
    this->WriteU8(value & 0xFF);
}

bool Writer::WriteBool(bool b)
{
    this->WriteU8(b ? 1 : 0);
    return b;
}

void Writer::WriteI16(int16_t value) { this->WriteU16((uint16_t)value); }

void Writer::WriteFloat(float value)
{
    this->WriteU32(Util::bit_cast<uint32_t>(value));
}

void Writer::WriteStringUTF(String *s) { this->WriteStringUTF(*s); }

void Writer::WriteStringUTF(String &s)
{
    this->WriteU16(s.length());
    this->WriteStringChars(s);
}

void Writer::WriteStringChars(String *s) { this->WriteStringChars(*s); }

void Writer::WriteStringChars(String &s)
{
    this->WriteStringChars(s.data(), s.length());
}

void Writer::WriteStringChars(char *chars, int size)
{
    this->WriteBytes((uint8_t *)chars, size);
}

void Writer::WriteStringChars(const char *chars, int size)
{
    this->WriteBytes((uint8_t *)chars, size);
}

void Writer::WriteBytes(uint8_t *bytes, int size)
{
    for (int i = 0; i < size; i++)
        this->WriteU8(bytes[i]);
}

uint8_t *Writer::Bytes() { return buffer; }

int Writer::Size() { return offset; }