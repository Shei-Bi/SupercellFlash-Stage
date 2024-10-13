#pragma once
#include <flash/DisplayObject.h>
#include <flash/display_object/ShapeOriginal.h>
#include <flash/display_object/ShapeDrawBitmapCommand.h>
#include <flash/types/SWFContainer.hpp>
// #include <glad/glad.h>

class Shape :public DisplayObject
{
public:
    sc::flash::SWFVector<sc::flash::ShapeDrawBitmapCommand>* commands = nullptr;

    /* data */
public:
    static Shape* createShape(sc::flash::ShapeOriginal* original);
    bool render(Matrix2x3* mat, ColorTransform* c, float);
};