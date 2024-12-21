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