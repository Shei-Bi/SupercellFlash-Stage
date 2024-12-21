#pragma once
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
};