#ifndef SCMATRIXBANK_H
#define SCMATRIXBANK_H
#include <vector>
#include "Matrix2x3.h"
#include "ColorTransform.h"
class ScMatrixBank {
public:
    unsigned short matrixSize;
    unsigned short colorTransformSize;
    std::vector<Matrix2x3> matrixs;
    std::vector<ColorTransform> colorTransforms;
    void init(int matrixSize, int colorTransformSize) {
        this->matrixSize = matrixSize;
        this->colorTransformSize = colorTransformSize;
        matrixs.resize(matrixSize);
        colorTransforms.resize(colorTransformSize);
    }
};
#endif