#pragma once
#include <glm/glm.hpp>
// #include <glad/glad.h>

class Renderer
{
public:
    static int left;
    static int top;
    static int right;
    static int bottom;
    static void setViewport(int, int, int, int);
    static glm::mat4 getPixelMatrix();
};
