#include "file/Reader.hpp"
#include "util/bitcast.hpp"
#include <cstdint>
#include <stdexcept>

using namespace Conv3d;

Reader::Reader()
{
    this->data = nullptr;
    this->offset = 0;
    this->size = 0;
    this->skipBytes = -1;
}

Reader::Reader(uint8_t* data, int size)
{
    this->data = data;
    this->offset = 0;
    this->size = size;
    this->skipBytes = -1;
}

bool Reader::HasData(int n) { return this->offset + n <= this->size; }

uint8_t* Reader::Read(int n)
{
    this->skipBytes -= n;
    if (!this->HasData(n))
    {
        throw std::runtime_error("buffer short");
    }
    this->offset += n;
    return this->data + this->offset - n;
}

String* Reader::ReadUTF()
{
    uint16_t length = this->ReadU16LittleEndian();
    return this->ReadUTFWithLength(length);
}

String* Reader::ReadUTFWithLength(uint16_t length)
{
    if (length == 0)
        return new String();
    uint8_t* data = this->Read((int)length);
    return new String((char*)data, (int)length);
}

uint32_t Reader::ReadU32LittleEndian()
{
    uint8_t* data = this->Read(4);
    return (uint32_t)data[3] | (uint32_t)data[2] << 8 |
        (uint32_t)data[1] << 16 | (uint32_t)data[0] << 24;
}

uint32_t Reader::ReadU32()
{
    uint8_t* data = this->Read(4);
    return (uint32_t)data[0] | (uint32_t)data[1] << 8 |
        (uint32_t)data[2] << 16 | (uint32_t)data[3] << 24;
}
uint16_t Reader::ReadU16LittleEndian()
{
    uint8_t* data = this->Read(2);
    return (uint32_t)data[1] | (uint32_t)data[0] << 8;
}

int16_t Reader::ReadI16LittleEndian() { return (int16_t)this->ReadU16LittleEndian(); }

uint8_t Reader::ReadU8() { return *this->Read(1); }

bool Reader::ReadBool() { return this->ReadU8() != 0; }

void Reader::Seek(int n)
{
    this->offset += n;
    this->skipBytes -= n;
}

float Reader::ReadFloatLittleEndian() { return Util::bit_cast<float>(this->ReadU32LittleEndian()); }

int Reader::SkipBytes() { return this->skipBytes; }

void Reader::SetSkipBytes(int n) { this->skipBytes = n; }