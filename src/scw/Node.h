#ifndef SCWNODE_H
#define SCWNODE_H

#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include "scw/Property.hpp"
#include "util/String.hpp"
#include <cstdint>
#include <vector>

namespace SCW
{

using Conv3d::String;

class InstanceMaterial
{
  public:
    String Name = {}, Target = {};

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

class NodeInstance
{
  public:
    String Type = {}, Target = {};
    String CameraTarget = {};
    std::vector<InstanceMaterial> Materials;

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

class Vector3
{
  public:
    float X, Y, Z;

    bool Equals(Vector3 &);
};

// Quaternion q= w + xi + yj + zk
//
// w is the scalar component
//
// x, y and z are the vector components
class Quaternion : public Vector3
{
  public:
    float W;

    bool Equals(Quaternion &);
};

class KeyFrame
{
  public:
    uint16_t ID;
    Quaternion Rotation;
    Vector3 Translation, Scale;

    void Decode(Conv3d::Reader *, uint8_t, uint16_t, std::vector<KeyFrame> &);
    void Encode(Conv3d::Writer *, uint8_t, uint16_t);
};

class Node
{
  public:
    String Name = {}, ParentName = {};
    std::vector<NodeInstance> Instances = {};
    std::vector<KeyFrame> Frames;
    uint8_t FrameFlags = 0;

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

} // namespace SCW

#endif // SCWNODE_H