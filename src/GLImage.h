#pragma once
#include "TextureData_generated.h"
#include "MipMapData_generated.h"

class SWFTexture;

class GLImage
{
private:
    unsigned int id;
public:
    GLImage();
    void bind();
    void unbind();
    void createWithFormat(SWFTexture* texture);
    void createWithFormat(const sc::texture::SCTX::TextureData* texture, std::vector<const sc::texture::SCTX::MipMap*>& mipmaps, unsigned char* levels);
};