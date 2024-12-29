#pragma once
#include <flash/Stage.h>
#include <flash/Shape.h>
#include <flash/DisplayObject.h>
#include "ShapeOriginal.h"
#include "ShapeDrawBitmapCommand.h"
// #include <glad/glad.h>

Shape* Shape::createShape(ShapeOriginal* original) {
    Shape* shape = new Shape();
    shape->commands = original->commands.data();
    shape->commandSize = original->commandSize;
    return shape;
}
bool Shape::render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
    Stage* Stage = Stage::getInstance();
    Matrix2x3* n = new Matrix2x3(Matrix, *mat);
    ColorTransform* cT = new ColorTransform(colorTransform, *c);
    for (unsigned short i = 0;i < commandSize;i++) {
        commands[i].render(n, cT, rc);
    }
    delete n;
    delete cT;
    return true;
}
bool Shape::collisionRender(Matrix2x3* mat) {
    Matrix2x3* n = new Matrix2x3(Matrix, *mat);
    for (unsigned short i = 0;i < commandSize;i++) {
        if (commands[i].render(n, &colorTransform, 0)) {
            goto success;
        }
    }
    delete n;
    return false;
success:
    delete n;
    return true;
}