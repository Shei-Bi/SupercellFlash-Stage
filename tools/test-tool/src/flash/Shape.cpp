#pragma once
#include <flash/Stage.h>
#include <flash/Shape.h>
#include <flash/DisplayObject.h>
#include <flash/display_object/ShapeOriginal.h>
#include <flash/display_object/ShapeDrawBitmapCommand.h>
#include <flash/types/SWFContainer.hpp>
// #include <glad/glad.h>

Shape* Shape::createShape(sc::flash::ShapeOriginal* original) {
    Shape* shape = new Shape();
    shape->commands = &original->commands;
    return shape;
}
bool Shape::render(Matrix2x3* mat, ColorTransform* c, float deltaTime) {
    Stage* Stage = Stage::getInstance();
    Matrix2x3* n = new Matrix2x3(Matrix);
    n->multiply(mat);
    ColorTransform* cT = new ColorTransform(colorTransform);
    cT->multiplyy(c);
    for (sc::flash::ShapeDrawBitmapCommand command : *commands) {
        if (Stage->shapeStart(command.GLImage)) {
            int triangleCount = command.vertices.size() - 2;
            Stage->addTriangles(triangleCount);
            sc::flash::SWFVector<float>* v = &Stage->currentBucket->vertices;
            int required = v->size() + command.vertices.size() * 11;
            if (v->capacity() < required) v->reserve(v->capacity() + 512 * 11 * 3);
            for (int i = 0;i < command.vertices.size();i++) {
                sc::flash::ShapeDrawBitmapCommandVertex vertex = command.vertices[i];
                v->push_back(n->applyX(vertex.x, vertex.y));
                v->push_back(n->applyY(vertex.x, vertex.y));
                v->push_back(vertex.u);
                v->push_back(vertex.v);
                v->push_back(cT->multiply.r / 255.0f);
                v->push_back(cT->multiply.g / 255.0f);
                v->push_back(cT->multiply.b / 255.0f);
                v->push_back(cT->alpha / 255.0f);
                v->push_back(cT->add.r / 255.0f);
                v->push_back(cT->add.g / 255.0f);
                v->push_back(cT->add.b / 255.0f);
            }
            // int triangleCount = 2;
            // Stage->addTriangles(triangleCount);
            // sc::flash::SWFVector<float>* v = &Stage->currentBucket->vertices;
            // int required = v->size() + 16;
            // if (v->capacity() < required) v->reserve(v->capacity() + 512 * 4 * 3);
            // v->push_back(n->applyX(-100, -100));
            // v->push_back(n->applyY(-100, -100));
            // v->push_back(-1.0);
            // v->push_back(-1.0);
            // v->push_back(n->applyX(-100, 100));
            // v->push_back(n->applyY(-100, 100));
            // v->push_back(-1.0);
            // v->push_back(1.0);
            // v->push_back(n->applyX(100, 100));
            // v->push_back(n->applyY(100, 100));
            // v->push_back(1.0);
            // v->push_back(1.0);
            // v->push_back(n->applyX(100, -100));
            // v->push_back(n->applyY(100, -100));
            // v->push_back(1.0);
            // v->push_back(-1.0);
            // Stage->abort = true;
        }
    }
    return true;
}