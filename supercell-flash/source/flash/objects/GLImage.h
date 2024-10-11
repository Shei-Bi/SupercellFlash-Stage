#pragma once
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
};