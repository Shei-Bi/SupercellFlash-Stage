#include <flash/objects/GLImage.h>
#include <stdio.h>
#include <flash/objects/SWFTexture.h>
#include <glad/glad.h>
GLImage::GLImage() {
    ;
}
void GLImage::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
void GLImage::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
void GLImage::createWithFormat(sc::flash::SWFTexture texture) {
    glGenTextures(1, &id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLint format = GL_RGBA;
    GLenum pixelType = GL_UNSIGNED_BYTE;
    switch ((uint8_t)texture.pixel_format())
    {
    case 2:
    case 8:
        pixelType = GL_UNSIGNED_SHORT_4_4_4_4;
        break;
    case 3:
        pixelType = GL_UNSIGNED_SHORT_5_5_5_1;
        break;
    case 4:
        pixelType = GL_UNSIGNED_SHORT_5_6_5;
        format = GL_RGB;
        break;
    case 6:
        format = GL_LUMINANCE_ALPHA;
        break;
    case 10:
        format = GL_LUMINANCE;
        break;
    }
    printf("width: %d\nheight: %d\n(uint8_t)texture.pixel_format(): %d\n", texture.image()->width(), texture.image()->height(), (uint8_t)texture.pixel_format());

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture.image()->width(), texture.image()->height(), 0, format, pixelType, texture.image()->data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}