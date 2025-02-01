#ifndef SCWCAM_H
#define SCWCAM_H

#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include "scw/Property.hpp"
#include "util/String.hpp"

namespace SCW
{

class Camera3D : public Property
{
  public:
    Conv3d::String Name = {};
    float Yfox, Xfov;
    float AspectRatio;
    float ZNear, ZFar;

    const char *Tag();
    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

} // namespace SCW

#endif // SCWCAM_H