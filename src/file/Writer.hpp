#ifndef WRITER_H
#define WRITER_H

#include "util/String.hpp"
#include <cstddef>
#include <cstdint>

namespace Conv3d
{

class Writer
{
  private:
    uint8_t *buffer = nullptr;
    int size = 0;
    int offset = 0;

  public:
    void EnsureCapacity(int);

    void WriteU8(uint8_t);
    void WriteU16(uint16_t);
    void WriteU32(uint32_t);
    bool WriteBool(bool);
    void WriteI16(int16_t);
    void WriteFloat(float);
    void WriteStringUTF(String *);
    void WriteStringUTF(String &);
    void WriteStringChars(String *);
    void WriteStringChars(String &);
    void WriteStringChars(char *, int);
    void WriteStringChars(const char *, int);
    void WriteBytes(uint8_t *, int);
    int Size();

    uint8_t *Bytes();

    ~Writer();
};

} // namespace Conv3d

#endif // WRITER_H