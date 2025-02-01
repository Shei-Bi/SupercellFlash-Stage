#ifndef SCWWEND_H
#define SCWWEND_H

#include "file/Writer.hpp"
#include "scw/Property.hpp"

namespace SCW
{

class Wend : public Property
{
  public:
    const char *Tag();
    void Encode(Conv3d::Writer *);
};

} // namespace SCW

#endif // SCWWEND_H