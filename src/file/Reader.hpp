#ifndef READER_H
#define READER_H

#include "util/String.hpp"
#include <cstdint>

namespace Conv3d
{

  class Reader
  {
  private:
    uint8_t* data;
    int offset, skipBytes, size;

  public:
    Reader();
    Reader(uint8_t*, int);

    bool HasData(int);

    uint8_t* Read(int);

    String* ReadUTF();
    String* ReadUTFWithLength(uint16_t);

    uint32_t ReadU32LittleEndian();
    uint32_t ReadU32();

    int16_t ReadI16LittleEndian();
    uint16_t ReadU16LittleEndian();

    uint8_t ReadU8();
    bool ReadBool();

    float ReadFloatLittleEndian();

    void Seek(int);

    int SkipBytes();
    void SetSkipBytes(int);
  };

} // namespace Conv3d

#endif // READER_H