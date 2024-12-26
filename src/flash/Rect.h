#ifndef RECT_H
#define RECT_H
class Rect {
public:
    float left, top, right, bottom;
    Rect() :left(0), top(0), right(0), bottom(0) {};
    Rect(float left, float top, float right, float bottom) :left(left), top(top), right(right), bottom(bottom) {
        ;
    }
    Rect(Rect* rect) :left(rect->left), top(rect->top), right(rect->right), bottom(rect->bottom) {
        ;
    }
    void movePosition(float x, float y) {
        left += x;
        right += x;
        top += y;
        bottom += y;
    }
    static Rect* LTWH(float left, float top, float width, float height) {
        return new Rect(left, top, left + width, top + height);
    }
    float getHeight() {
        return bottom - top;
    }
    float getWidth() {
        return right - left;
    }
    float getMidX() {
        return left + getWidth() / 2.0f;
    }
    float getMidY() {
        return top + getHeight() / 2.0f;
    }
};
#endif