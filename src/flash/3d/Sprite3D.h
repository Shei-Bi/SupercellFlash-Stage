#pragma once
#include "Scene.h"
#include "scw/File.hpp"
#include "Skeleton.h"

class Sprite3D {
public:
    Scene scene;
    Skeleton* skeleton;
    std::vector<DynamicMesh*> dynamicMeshes;
    glm::mat4 matrix;
    SCW::File* fileReference;
    void render(const glm::mat4& transform);
    void renderScene(const glm::mat4& transform);
    void createFromFile(std::string& filename);
    void createFromFile(SCW::File* scwFile);
    void createDynamicMeshes(Skeleton* skeleton);
    DynamicMesh* getDynamicMesh(int index);
};