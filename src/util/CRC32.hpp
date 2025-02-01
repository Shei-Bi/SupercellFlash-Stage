#ifndef CRC32_H
#define CRC32_H

// https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635

#include <stdint.h>

struct CRC32_s
{
    void generate_table(uint32_t (&table)[256]);

    uint32_t update(uint32_t (&table)[256], uint32_t initial, const void *buf,
                    size_t len);
};

class CRC32
{
  private:
    uint32_t table[256];
    CRC32_s crc32_s;
    uint32_t initial;

  public:
    CRC32();

    void Update(const unsigned __int8 *buf, size_t len);

    uint32_t GetValue() const;
};

#endif // CRC32_H