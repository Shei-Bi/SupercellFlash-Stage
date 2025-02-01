#include "GLImage.h"
#include <stdio.h>
#include "flash/SWFTexture.h"
#include <glad/glad.h>
#include "stdlib.h"

GLImage::GLImage() {
    ;
}
void GLImage::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
void GLImage::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
void GLImage::createWithFormat(SWFTexture* texture) {
    glGenTextures(1, &id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (texture->glFormat == 0) {
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, (GLenum)texture->glInternalFormat, texture->width, texture->height, 0, texture->khronosDataSize, texture->khronosDatas[0]);
    }
    else abort();
    printf("Error loading ktx: %d\n", glGetError());
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}

void GLImage::createWithFormat(const sc::texture::SCTX::TextureData* texture, std::vector<const sc::texture::SCTX::MipMap*>& mipmaps, unsigned char* levels) {
    // printf("%d %d %d %d %d %d\n", texture->pixel_type(), texture->width(), texture->height(), texture->levels_count(), texture->texture_length(), texture->flags());
    if (texture->variants()) abort();
    int glFormat = -1;
    switch (texture->pixel_type()) {
    case 204:
        glFormat = 37808;
        break;
    case 212:
        glFormat = 37815;
        break;
    default:
        abort();
        break;
    }
    glGenTextures(1, &id);
    bind();
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glCompressedTexImage2D(GL_TEXTURE_2D, 0, glFormat, texture->width(), texture->height(), 0, mipmaps[1]->offset(), levels);
    printf("Error: %d\n", glGetError());
    unbind();
}
