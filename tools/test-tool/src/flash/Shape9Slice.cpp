#pragma once
#include <flash/Shape9Slice.h>
#include <flash/Stage.h>

Shape9Slice* Shape9Slice::createShape(sc::flash::ShapeOriginal* original, Rect* rect) {
    Shape9Slice* shape9Slice = new Shape9Slice();
    shape9Slice->commands = &original->commands;
    shape9Slice->scalingGrid = rect;
    return shape9Slice;
}
void updateBound(Rect* r, float x, float y) {
    if (r->x > x) r->x = x;
    else if (r->x + r->width < x) r->width = x - r->x;
    if (r->y > y) r->y = y;
    else if (r->y + r->height < y) r->height = y - r->y;
}
bool Shape9Slice::render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
    Rect* pos = new Rect(100000, 100000, -100000, -100000);
    for (sc::flash::ShapeDrawBitmapCommand command : *commands) {
        for (int i = 0;i < command.vertices.size();i++) {
            updateBound(pos, command.vertices[i].x, command.vertices[i].y);
        }
    }
    Rect* bounds = new Rect(scalingGrid->x - Matrix.tx, scalingGrid->y - Matrix.ty, scalingGrid->width, scalingGrid->height);
    Stage* Stage = Stage::getInstance();
    Matrix2x3* n = new Matrix2x3(Matrix);
    n->multiply(mat);
    ColorTransform* cT = new ColorTransform(colorTransform);
    cT->multiplyy(c);

    float newWidth = 1 / sqrtf(powf(n->a, 2) + powf(n->b, 2));
    float newHeight = 1 / sqrtf(powf(n->c, 2) + powf(n->d, 2));

    for (sc::flash::ShapeDrawBitmapCommand command : *commands) {
        if (Stage->shapeStart(command.GLImage, rc)) {
            int triangleCount = command.vertices.size() - 2;
            Stage->addTriangles(triangleCount);
            sc::flash::SWFVector<float>* v = &Stage->currentBucket->vertices;
            int required = v->size() + command.vertices.size() * 11;
            if (v->capacity() < required) v->reserve(v->capacity() + 512 * 11 * 3);
            for (int i = 0;i < command.vertices.size();i++) {
                sc::flash::ShapeDrawBitmapCommandVertex* vertex = &command.vertices[i];
                float x = vertex->x;
                float y = vertex->y;
                if (x <= bounds->x) x = fmin(bounds->x + bounds->width / 2, pos->x + (x - pos->x) * newWidth);
                else if (x >= bounds->x + bounds->width) x = fmax(bounds->x + bounds->width / 2, pos->x + pos->width + (x - pos->x - pos->width) * newWidth);
                if (y <= bounds->y) y = fmin(bounds->y + bounds->height / 2, pos->y + (y - pos->y) * newHeight);
                else if (y >= bounds->y + bounds->height) y = fmax(bounds->y + bounds->height / 2, pos->y + pos->height + (y - pos->y - pos->height) * newHeight);
                v->push_back(n->applyX(x, y));
                v->push_back(n->applyY(x, y));
                v->push_back(vertex->u);
                v->push_back(vertex->v);
                v->push_back(cT->multiply.r / 255.0f);
                v->push_back(cT->multiply.g / 255.0f);
                v->push_back(cT->multiply.b / 255.0f);
                v->push_back(cT->alpha / 255.0f);
                v->push_back(cT->add.r / 255.0f);
                v->push_back(cT->add.g / 255.0f);
                v->push_back(cT->add.b / 255.0f);
            }
        }
    }
    return true;
}