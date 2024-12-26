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
    ColorTransform() {
        mulR = 255;
        mulG = 255;
        mulB = 255;
        alpha = 255;
        addR = 0;
        addG = 0;
        addB = 0;
    }
    ColorTransform(ColorTransform& one, ColorTransform& two) {
        mulR = one.mulR;
        mulG = one.mulG;
        mulB = one.mulB;
        alpha = one.alpha;
        addR = one.addR;
        addG = one.addG;
        addB = one.addB;
        multiply(&two);
    }
    void reset() {
        mulR = 255;
        mulG = 255;
        mulB = 255;
        alpha = 255;
        addR = 0;
        addG = 0;
        addB = 0;
    }
};
#endif