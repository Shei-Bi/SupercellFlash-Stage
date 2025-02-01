#ifndef SCWHEAD_H
#define SCWHEAD_H

#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include "scw/Property.hpp"
#include "util/String.hpp"
#include <cstdint>

namespace SCW
{

class Header : public Property
{
  public:
    uint16_t Version;
    uint16_t FrameRate;
    uint16_t FirstFrame;
    uint16_t LastFrame;
    Conv3d::String MaterialsFile = {};
    int Unknown;

    const char *Tag();
    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);

    // ~Header();
};

} // namespace SCW

#endif // SCWHEAD_H