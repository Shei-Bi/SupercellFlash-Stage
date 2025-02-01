#ifndef PROPERTY_H
#define PROPERTY_H

#include "file/Writer.hpp"
namespace SCW
{

class Property
{
  public:
    virtual const char *Tag() = 0;
    virtual void Encode(Conv3d::Writer *) = 0;
};

} // namespace SCW

#endif // PROPERTY_H