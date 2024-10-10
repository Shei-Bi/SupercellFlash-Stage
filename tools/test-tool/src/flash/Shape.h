#pragma once
#include <flash/DisplayObject.h>
#include <flash/display_object/ShapeOriginal.h>
#include <flash/display_object/ShapeDrawBitmapCommand.h>
#include <flash/types/SWFContainer.hpp>
#include <glad/glad.h>

class Shape :public DisplayObject
{
private:
    sc::flash::SWFVector<sc::flash::ShapeDrawBitmapCommand>* commands = nullptr;

    /* data */
public:
    static Shape* createShape(sc::flash::ShapeOriginal* original) {
        Shape* shape = new Shape();
        shape->commands = &original->commands;
        return shape;
    }
    bool render() {
        sc::flash::ShapeDrawBitmapCommand command = (*commands)[0];
        int triangleCount = command.vertices.size() - 2;
        Stage::getInstance()->addTriangles(triangleCount);
        sc::flash::SWFVector<float>* v = &Stage::getInstance()->currentBucket->vertices;
        int required = v->size() + command.vertices.size() * 4;
        if (v->capacity() < required) v->resize(required);
        for (int i = 0;i < command.vertices.size();i++) {
            sc::flash::ShapeDrawBitmapCommandVertex vertex = command.vertices[i];
            v->push_back(vertex.x / 25);
            v->push_back(vertex.y / 25);
            v->push_back(vertex.u);
            v->push_back(vertex.v);
        }
        return true;
    }
};