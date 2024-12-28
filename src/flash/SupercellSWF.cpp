#include "SupercellSWF.h"
#include <stdio.h>
#include "Matrix2x3.h"
#include "MovieClipModifier.h"
#include "ShapeOriginal.h"
#include "MovieClipOriginal.h"
#include "TextFieldOriginal.h"
#include "SWFTexture.h"
#include "ScMatrixBank.hpp"
#include <cstring>
#pragma optimize( "t", on )
bool SupercellSWF::loadInternal(std::filesystem::path& path) {
    sc = fopen((char*)path.generic_string().c_str(), "rb");
    buffer = malloc(4);

    shapeSize = readShort();
    movieClipSize = readShort();
    textureSize = readShort();
    textFieldSize = readShort();
    matrixSize = readShort();
    colorTransformSize = readShort();
    skip(5);
    exportNameSize = readShort();
    exportNameIds = new unsigned short[exportNameSize];
    exportNameStrings = new char* [exportNameSize];
    readShortArray(exportNameSize, exportNameIds);
    for (int i = 0;i < exportNameSize;i++) {
        int kZero = 0;
        exportNameStrings[i] = readAscii(&kZero, NULL);
    }
    // for (int i = 0;i < exportNameSize;i++) {
    //     printf("%d: %s\n", exportNameIds[i], exportNameStrings[i]);
    // }
    shapes.resize(shapeSize);
    movieClips.resize(movieClipSize);
    textFields.resize(textFieldSize);
    textures.resize(textureSize);
    auto mb = new ScMatrixBank();
    mb->init(matrixSize, colorTransformSize);
    matrixBanks.push_back(mb);
    loadTags();

    fclose(sc);
    sc = NULL;
    free(buffer);
    buffer = NULL;
    return true;
}
unsigned short SupercellSWF::readShort() {
    fread(buffer, sizeof(unsigned short), 1, sc);
    return *(unsigned short*)buffer;
}
int SupercellSWF::readInt() {
    fread(buffer, sizeof(int), 1, sc);
    return *(int*)buffer;
}
unsigned char SupercellSWF::readUnsignedChar() {
    fread(buffer, sizeof(unsigned char), 1, sc);
    return *(unsigned char*)buffer;
}
void SupercellSWF::readShortArray(int size, unsigned short* out) {
    fread(out, sizeof(unsigned short), size, sc);
}
unsigned short* SupercellSWF::readShortArray(int size) {
    unsigned short* temp = (unsigned short*)malloc(size * sizeof(unsigned short));
    // auto temp = new unsigned short[size];
    readShortArray(size, temp);
    return temp;
}
void SupercellSWF::readByteArray(int size, unsigned char* out) {
    fread(out, sizeof(unsigned char), size, sc);
}
unsigned char* SupercellSWF::readByteArray(int size) {
    auto temp = new unsigned char[size];
    readByteArray(size, temp);
    return temp;
}
char* SupercellSWF::readAscii(int* sizeOut, char* out) {
    unsigned char size = readUnsignedChar();
    if (size == 255) {
        *sizeOut = -1;
        return NULL;
    }
    if (!out) out = new char[size + 1];
    readByteArray(size, (unsigned char*)out);
    out[size] = (char)0;
    return out;
}
void SupercellSWF::loadTags() {
    unsigned short shapesLoaded = 0;
    unsigned short movieClipsLoaded = 0;
    unsigned short textFieldsLoaded = 0;
    unsigned short texturesLoaded = 0;
    unsigned short movieClipModifiersLoaded = 0;
    unsigned short matrixBanksLoaded = 0;
    unsigned short matrixsLoaded = 0;
    unsigned short colorTransformsLoaded = 0;
    unsigned short matrixSize;
    unsigned short colorTransformSize;
    ScMatrixBank* mb;
    while (true) {
        unsigned char tag = readUnsignedChar();
        int length = readInt();
        // printf("%d\n", tag);
        // skip(length);
        switch (tag) {
        case 0:
            return;
        case 7:
            textFields[textFieldsLoaded].load(this);
            textFieldsLoaded++;
            break;
        case 8:
            readMatrix2x3(matrixBanks[matrixBanksLoaded]->matrixs[matrixsLoaded]);
            matrixsLoaded++;
            break;
        case 9:
            readColorTransform(matrixBanks[matrixBanksLoaded]->colorTransforms[colorTransformsLoaded]);
            colorTransformsLoaded++;
            break;
        case 12:
            movieClips[movieClipsLoaded].load(this);
            movieClipsLoaded++;
            break;
        case 36:
            readMatrix2x3_2(matrixBanks[matrixBanksLoaded]->matrixs[matrixsLoaded]);
            matrixsLoaded++;
            break;
        case 18:
            shapes[shapesLoaded].load(this);
            shapesLoaded++;
            break;
        case 45:
            textures[texturesLoaded].load(this);
            texturesLoaded++;
            break;
        case 37:
            movieClipModifierSize = readShort();
            movieClipModifiers.resize(movieClipModifierSize);
            break;
        case 38:
        case 39:
        case 40:
            movieClipModifiers[movieClipModifiersLoaded].load(this, tag);
            movieClipModifiersLoaded++;
            break;
        case 42:
            matrixSize = readShort();
            colorTransformSize = readShort();
            mb = new ScMatrixBank();
            mb->init(matrixSize, colorTransformSize);
            matrixBanks.push_back(mb);
            matrixsLoaded = 0;
            colorTransformsLoaded = 0;
            matrixBanksLoaded++;
            break;
        default:
            printf("unknwon tag: %d\n", tag);
            abort();
            break;
        }
    }
}
void SupercellSWF::skip(int offset) {
    fseek(sc, offset, SEEK_CUR);
}
float SupercellSWF::readTwip() {
    return readInt() * 0.05f;
}
bool SupercellSWF::readBool() {
    return readUnsignedChar() != 0;
}
void SupercellSWF::readMatrix2x3(Matrix2x3& out) {
    out.a = readInt() / 1024.0f;
    out.b = readInt() / 1024.0f;
    out.c = readInt() / 1024.0f;
    out.d = readInt() / 1024.0f;
    out.tx = readTwip();
    out.ty = readTwip();
}
void SupercellSWF::readMatrix2x3_2(Matrix2x3& out) {
    out.a = readInt() / 65535.0f;
    out.b = readInt() / 65535.0f;
    out.c = readInt() / 65535.0f;
    out.d = readInt() / 65535.0f;
    out.tx = readTwip();
    out.ty = readTwip();
}
void SupercellSWF::readColorTransform(ColorTransform& out) {
    static_assert(sizeof(ColorTransform) == 7);
    // readByteArray(sizeof(ColorTransform), &out.mulR);
    out.addR = readUnsignedChar();
    out.addG = readUnsignedChar();
    out.addB = readUnsignedChar();
    out.alpha = readUnsignedChar();
    out.mulR = readUnsignedChar();
    out.mulG = readUnsignedChar();
    out.mulB = readUnsignedChar();
}
MovieClipOriginal* SupercellSWF::getOriginalMovieClip(unsigned short id) {
    for (MovieClipOriginal& mc : movieClips)if (mc.id == id) return &mc;
    abort();
}
DisplayObjectOriginal* SupercellSWF::getOriginalDisplayObject(unsigned short id) {
    for (MovieClipOriginal& mc : movieClips)if (mc.id == id) return &mc;
    for (ShapeOriginal& s : shapes)if (s.id == id) return &s;
    for (TextFieldOriginal& tf : textFields)if (tf.id == id) return &tf;
    for (MovieClipModifier& mcm : movieClipModifiers)if (mcm.id == id) return &mcm;
    abort();
}
bool SupercellSWF::hasExportName(char* name) {
    for (unsigned short i = 0;i < exportNameSize;i++) if (strcmp(exportNameStrings[i], name) == 0) return true;
    return false;
}
#pragma optimize( "t", off )