#ifndef COLORTRANSFORM_H
#define COLORTRANSFORM_H
#include <math.h>
class ColorTransform {
public:
    unsigned char mulR, mulG, mulB, alpha, addR, addG, addB;
    void multiply(ColorTransform* color) {
        mulR = fmin(mulR * color->mulR / 255.0f, 255.0f);
        mulG = fmin(mulG * color->mulG / 255.0f, 255.0f);
        mulB = fmin(mulB * color->mulB / 255.0f, 255.0f);
        alpha = fmin(alpha * color->alpha / 255.0f, 255.0f);
        addR = fmin(addR + color->addR, 255.0f);
        addG = fmin(addG + color->addG, 255.0f);
        addB = fmin(addB + color->addB, 255.0f);
    }
};
#endif