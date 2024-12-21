#include "SWFTexture.h"
#include "SupercellSWF.h"
#include "assert.h"
void SWFTexture::load(SupercellSWF* sc) {
    int ktxLength = sc->readInt();
    pixelFormat = sc->readUnsignedChar();
    width = sc->readShort();
    height = sc->readShort();
    //ktx
    for (int i = 0;i < sizeof(ktxHeader);i++) {
        assert(sc->readUnsignedChar() == ktxHeader[i]);
    }
    sc->readInt();
    int glType = sc->readInt();
    int glTypeSize = sc->readInt();
    glFormat = sc->readInt();
    glInternalFormat = sc->readInt();
    int glInternalBasetype = sc->readInt();
    width = sc->readInt();
    height = sc->readInt();
    sc->readInt();
    sc->readInt();
    sc->readInt();
    levels = sc->readInt();
    sc->skip(sc->readInt());
    khronosDatas = new unsigned char* [levels];
    for (int i = 0;i < levels;i++) {
        khronosDataSize = sc->readInt();
        khronosDatas[i] = new unsigned char[khronosDataSize];
        sc->readByteArray(khronosDataSize, khronosDatas[i]);
    }
    glImage = new GLImage();
    glImage->createWithFormat(this);
}