#ifndef SUPERCELLSWF_H
#define SUPERCELLSWF_H
#include <iostream>
#include <filesystem>
#include <vector>
#include "ShapeOriginal.h"
#include "MovieClipOriginal.h"
#include "TextFieldOriginal.h"
#include "SWFTexture.h"
#include "MovieClipModifier.h"
#include "ScMatrixBank.hpp"
#include "Matrix2x3.h"
#include "ColorTransform.h"
class SupercellSWF {
public:
    FILE* sc;
    void* buffer;
    unsigned short shapeSize;
    unsigned short movieClipSize;
    unsigned short textFieldSize;
    unsigned short textureSize;
    unsigned short matrixSize;
    unsigned short colorTransformSize;
    unsigned short exportNameSize;
    unsigned short movieClipModifierSize;
    unsigned short* exportNameIds;
    char** exportNameStrings;
    std::vector<ShapeOriginal> shapes;
    std::vector<MovieClipOriginal> movieClips;
    std::vector<TextFieldOriginal> textFields;
    std::vector<SWFTexture> textures;
    std::vector<MovieClipModifier> movieClipModifiers;
    std::vector<ScMatrixBank*> matrixBanks;
    bool loadInternal(std::filesystem::path& path);
    unsigned short readShort();
    int readInt();
    unsigned char readUnsignedChar();
    void readShortArray(int, unsigned short*);
    unsigned short* readShortArray(int);
    char* readAscii(int*, char*);
    void readByteArray(int, unsigned char*);
    unsigned char* readByteArray(int);
    void loadTags();
    void skip(int);
    float readTwip();
    bool readBool();
    void readMatrix2x3(Matrix2x3&);
    void readMatrix2x3_2(Matrix2x3&);
    void readColorTransform(ColorTransform&);
    MovieClipOriginal* getOriginalMovieClip(unsigned short);
    DisplayObjectOriginal* getOriginalDisplayObject(unsigned short);
    bool hasExportName(char*);
};
#endif