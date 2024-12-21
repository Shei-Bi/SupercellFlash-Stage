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
    ColorTransform* cT = new ColorTransform(colorTransform);
    cT->multiply(c);
    Rect* bounds = new Rect();
    for (unsigned short i = 0;i < commandSize;i++) {
        auto& command = commands[i];
        for (int i = 0;i < command.vertexSize;i++) {
            auto& vertex = command.vertexs[i];
            Stage::updateBound(bounds, n->applyX(vertex.x, vertex.y), n->applyY(vertex.x, vertex.y));
        }
        if (Stage->shapeStart(bounds->left, bounds->top, bounds->right, bounds->bottom, command.glImage, rc)) {
            int triangleCount = command.vertexSize - 2;
            std::vector<float>* v = &Stage->currentBucket->vertices;
            int required = v->size() + command.vertexSize * 11;
            if (required > 65535) {
                printf("Stage vertex overflow, required:%d", required);
                return true;
            }
            Stage->addTriangles(triangleCount);
            if (v->capacity() < required) v->reserve(v->capacity() + 512 * 11 * 3);
            for (int i = 0;i < command.vertexSize;i++) {
                auto& vertex = command.vertexs[i];
                v->push_back(n->applyX(vertex.x, vertex.y));
                v->push_back(n->applyY(vertex.x, vertex.y));
                v->push_back(vertex.u / 65536.0f);
                v->push_back(vertex.v / 65536.0f);
                v->push_back(cT->mulR / 255.0f);
                v->push_back(cT->mulG / 255.0f);
                v->push_back(cT->mulB / 255.0f);
                v->push_back(cT->alpha / 255.0f);
                v->push_back(cT->addR / 255.0f);
                v->push_back(cT->addG / 255.0f);
                v->push_back(cT->addB / 255.0f);
            }
        }
    }
    return true;
}