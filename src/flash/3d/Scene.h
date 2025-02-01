#pragma once
#include "scw/File.hpp"
#include "flash/3d/Mesh.h"

class Scene {
public:
    std::vector<Mesh*> meshes;
    void createFromSCWFile(SCW::File* scwFile);
    int getMeshCount();
    Mesh* getMesh(int index);
    void setMeshMaterial(int index, SCW::Material* material);
    void setTransformation(int index, const glm::mat4& matrix);
};