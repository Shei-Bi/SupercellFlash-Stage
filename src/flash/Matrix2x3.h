#ifndef MATRIX2X3_H
#define MATRIX2X3_H
class Matrix2x3 {
public:
    float a, b, c, d, tx, ty;
    float applyX(float x, float y) {
        return x * a + y * c + tx;
    }

    float applyY(float x, float y) {
        return y * d + x * b + ty;
    }
    void multiply(Matrix2x3* matrix) {
        float scaleX = (a * matrix->a) + (b * matrix->c);
        float shearX = (a * matrix->b) + (b * matrix->d);
        float scaleY = (d * matrix->d) + (c * matrix->b);
        float shearY = (d * matrix->c) + (c * matrix->a);
        float x = matrix->applyX(tx, ty);
        float y = matrix->applyY(tx, ty);

        a = scaleX;
        b = shearX;
        d = scaleY;
        c = shearY;
        tx = x;
        ty = y;
    }
    Matrix2x3() {
        a = 1.0f;
        b = 0.0f;
        c = 0.0f;
        d = 1.0f;
        tx = 0.0f;
        ty = 0.0f;
    }
    Matrix2x3(Matrix2x3& one, Matrix2x3& two) {
        a = one.a;
        b = one.b;
        c = one.c;
        d = one.d;
        tx = one.tx;
        ty = one.ty;
        multiply(&two);
    }
};
#endif