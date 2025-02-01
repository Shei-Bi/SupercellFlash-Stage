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
    return false;
}
TextField* TextField::createTextField(TextFieldOriginal* original) {
    TextField* textField = new TextField();
    textField->bounds = original->bounds;
    return textField;
}