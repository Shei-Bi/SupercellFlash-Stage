#include <flash/TextField.h>
#include <flash/Stage.h>
#include "TextField.h"
#include "TextFieldOriginal.h"

bool TextField::render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
    Stage* Stage = Stage::getInstance();
    if (!Stage->isCalculatingBounds) return false;
    Matrix2x3* n = new Matrix2x3(Matrix, *mat);

    bool touchResult;
    Stage->shapeStart(n->applyX(bounds.left, bounds.top), n->applyY(bounds.left, bounds.top), n->applyX(bounds.right, bounds.bottom), n->applyY(bounds.right, bounds.bottom), nullptr, 0, &touchResult);
    delete n;
    return touchResult;
}
bool TextField::collisionRender(Matrix2x3* mat) {
    auto cT = new ColorTransform();
    auto result = render(mat, cT, 0, 0.0f);
    delete cT;
    return result;
}
TextField* TextField::createTextField(TextFieldOriginal* original) {
    TextField* textField = new TextField();
    textField->bounds = original->bounds;
    return textField;
}