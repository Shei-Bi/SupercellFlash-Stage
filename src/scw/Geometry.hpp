#ifndef SCWGEO_H
#define SCWGEO_H

#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include "scw/Property.hpp"
#include "util/String.hpp"
#include <cstdint>
#include <vector>

namespace SCW
{

using Conv3d::String;

class Matrix4x4
{
  private:
    float elems[4][4] = {};
    typedef float __MatrixT;

  public:
    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);

    void Transpose();

    float &operator()(int x, int y);
};

class SourceArray
{
  public:
    String Name;
    uint8_t Index;
    uint8_t SourceIndex;           // uh, it is used for TEXTCOORD I think?
    uint8_t Stride;                // stride(Color) / element size
    float Scale;                   // this can always be 0?? (not saying it is)
    std::vector<double> Data = {}; // vertex/coordinate Data?

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

class IndexArray
{
  public:
    String Name = {};
    uint8_t IndexBufferSize = 0;
    std::vector<uint32_t> IndexBuffer = {};
    uint32_t TrianglesCount = 0;
    uint8_t InputsCount = 0;

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

class Weight
{
  public:
    uint8_t Joints[4];
    uint16_t Weights[4];

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

class Geometry : public Property
{
  public:
    String Name;
    String Group;
    // // this was used in older versions of scw format
    // Matrix4x4 IgnoredMatrix;
    std::vector<SourceArray> Vertices = {};
    bool HasBindMatrix;
    Matrix4x4 BindMatrix;
    struct
    {
        std::vector<String> Joints = {};
        std::vector<Matrix4x4> InverseBindMatrices = {};
    } Skins;

    std::vector<Weight> SkinWeights = {};
    std::vector<IndexArray> Materials = {};

    const char *Tag();
    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

} // namespace SCW

#endif // SCWGEO_H