#pragma once
#include "Scene.h"
#include "scw/File.hpp"

class Sprite3D {
public:
    Scene scene;
    glm::mat4 matrix;
    void render(const glm::mat4& transform);
    void renderScene(const glm::mat4& transform);
    void createFromFile(std::string& filename);
    void createFromFile(SCW::File* scwFile);

};