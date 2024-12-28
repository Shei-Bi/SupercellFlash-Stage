#pragma once
#include <flash/Shape9Slice.h>
#include <flash/Stage.h>
#include "ShapeOriginal.h"

Shape9Slice* Shape9Slice::createShape(ShapeOriginal* original, Rect* rect) {
    Shape9Slice* shape9Slice = new Shape9Slice();
    shape9Slice->commands = original->commands.data();
    shape9Slice->commandSize = original->commandSize;
    shape9Slice->scalingGrid = rect;
    return shape9Slice;
}
bool Shape9Slice::render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
    Stage* Stage = Stage::getInstance();
    Matrix2x3* n = new Matrix2x3(Matrix, *mat);
    ColorTransform* cT = new ColorTransform(colorTransform, *c);
    Rect* bounds = new Rect(20480, 20480, -20480, -20480);
    for (unsigned short i = 0;i < commandSize;i++) {
        auto& command = commands[i];
        for (int i = 0;i < command.vertexSize;i++) {
            Stage::updateBound(bounds, command.vertexs[i].x, command.vertexs[i].y);
        }
    }
    Rect* grid = new Rect(scalingGrid);
    grid->movePosition(-Matrix.tx, -Matrix.ty);
    float newWidth = Stage->pointSize / sqrtf(powf(n->a, 2) + powf(n->b, 2));
    float newHeight = Stage->pointSize / sqrtf(powf(n->c, 2) + powf(n->d, 2));
    for (unsigned short i = 0;i < commandSize;i++) {
        commands[i].render9Slice(n, cT, rc, grid, bounds, newWidth, newHeight);
    }
    delete n;
    delete cT;
    delete bounds;
    delete grid;

    // Rect* displayObjectBounds = new Rect();
    // for (ShapeDrawBitmapCommand& command : *commands) {
    //     for (int i = 0;i < command.vertexSize;i++) {
    //         ShapeDrawBitmapCommandVertex& vertex = command.vertexs[i];
    //         float x = vertex.x;
    //         float y = vertex.y;
    //         if (x <= bounds->left) x = fmin(bounds->left + bounds->getWidth() / 2, pos->left + (x - pos->left) * newWidth);
    //         else if (x >= bounds->right) x = fmax(bounds->left + bounds->getWidth() / 2, pos->left + pos->getWidth() + (x - pos->x - pos->width) * newWidth);
    //         if (y <= bounds->y) y = fmin(bounds->y + bounds->height / 2, pos->y + (y - pos->y) * newHeight);
    //         else if (y >= bounds->y + bounds->height) y = fmax(bounds->y + bounds->height / 2, pos->y + pos->height + (y - pos->y - pos->height) * newHeight);
    //         Stage::updateBound(displayObjectBounds, n->applyX(x, y), n->applyY(x, y));
    //     }
    //     if (Stage->shapeStart(displayObjectBounds->x, displayObjectBounds->y, displayObjectBounds->x + displayObjectBounds->width, displayObjectBounds->y + displayObjectBounds->height, command.GLImage, rc)) {
    //         int triangleCount = command.vertices.size() - 2;
    //         Stage->addTriangles(triangleCount);
    //         sc::flash::SWFVector<float>* v = &Stage->currentBucket->vertices;
    //         int required = v->size() + command.vertices.size() * 11;
    //         if (v->capacity() < required) v->reserve(v->capacity() + 512 * 11 * 3);
    //         for (int i = 0;i < command.vertices.size();i++) {
    //             ShapeDrawBitmapCommandVertex& vertex = command.vertexs[i];
    //             float x = vertex->x;
    //             float y = vertex->y;
    //             if (x <= bounds->x) x = fmin(bounds->x + bounds->width / 2, pos->x + (x - pos->x) * newWidth);
    //             else if (x >= bounds->x + bounds->width) x = fmax(bounds->x + bounds->width / 2, pos->x + pos->width + (x - pos->x - pos->width) * newWidth);
    //             if (y <= bounds->y) y = fmin(bounds->y + bounds->height / 2, pos->y + (y - pos->y) * newHeight);
    //             else if (y >= bounds->y + bounds->height) y = fmax(bounds->y + bounds->height / 2, pos->y + pos->height + (y - pos->y - pos->height) * newHeight);
    //             v->push_back(n->applyX(x, y));
    //             v->push_back(n->applyY(x, y));
    //             v->push_back(vertex->u);
    //             v->push_back(vertex->v);
    //             v->push_back(cT->multiply.r / 255.0f);
    //             v->push_back(cT->multiply.g / 255.0f);
    //             v->push_back(cT->multiply.b / 255.0f);
    //             v->push_back(cT->alpha / 255.0f);
    //             v->push_back(cT->add.r / 255.0f);
    //             v->push_back(cT->add.g / 255.0f);
    //             v->push_back(cT->add.b / 255.0f);
    //         }
    //     }
    // }
    return true;
}