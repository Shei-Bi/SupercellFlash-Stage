#pragma once
#include <texture/texture.h>

namespace sc {
    namespace flash {
        class SWFTexture;
    }
}
class GLImage
{
private:
    unsigned int id;
public:
    GLImage();
    void bind();
    void unbind();
    void createWithFormat(sc::flash::SWFTexture texture);
    void createWithFormat(sc::texture::KhronosTexture1*);
};