#ifndef SCWMAT_H
#define SCWMAT_H

#include "file/Reader.hpp"
#include "file/Writer.hpp"
#include "scw/Property.hpp"
#include "util/String.hpp"
#include <cstdint>
namespace SCW
{

using Conv3d::String;

class RGBA
{
  public:
    union
    {
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
        char bytes[4];
    } colors;

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

class Variable
{
  public:
    bool UseText2D;
    String Texture2D;
    RGBA Color;

    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);
};

class Material : public Property
{
  public:
    String Name;
    String ShaderFile;
    uint8_t BlendMode;
    class
    {
      public:
        Variable Ambient;
        Variable Diffuse;
        Variable Specular;
        String StencilTex2D;
        String NormalTex2D;
        Variable Colorize;
        Variable Emission;
        String OpacityTex2D;
        float Opacity, Unk;
        String LightmapTex2D, LightmapSpecularTex2D, Unk2;
    } Variables;
    uint32_t ShaderConfig;
    float StencilScaleOffset[4];

    const char *Tag();
    void Decode(Conv3d::Reader *);
    void Encode(Conv3d::Writer *);

    // ~Material();
};

} // namespace SCW

#endif // SCWMAT_H