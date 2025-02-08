#include "Sprite3D.h"
#include <fstream>
#include "flash/Stage.h"

void Sprite3D::render(const glm::mat4& transform) {
    Stage::getInstance()->start3D();
    renderScene(transform);
    // glm::mat4 projection = glm::perspective(20.0f, (float)800 / (float)600, 10.5f, 199.5f);
    // // glm::mat4 view = camera.GetViewMatrix();
    // glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 5.0f, 105.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // uber_shader->setMat4("u_projectionView", projection);
    // uber_shader->setMat4("u_view", view);
    // uber_shader->setFloat("a_texMul", 1.0f);
    // // render the loaded model
    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    // uber_shader->setMat4("a_model", model);
}

void Sprite3D::renderScene(const glm::mat4& transform) {
    for (int i = 0;i < scene.getMeshCount();i++) {
        scene.getMesh(i)->render(transform, getDynamicMesh(i));
    }
}

void Sprite3D::createFromFile(std::string& filename) {
    std::ifstream is("assets/sc3d/" + filename, std::ios_base::binary);
    is.seekg(0, std::ios::end);
    size_t length = is.tellg();
    is.seekg(0, std::ios::beg);

    unsigned char* buf = new unsigned char[length];
    is.read((char*)buf, length);
    is.close();

    SCW::File* scwFile = new SCW::File(buf, length);

    if (!scwFile->LoadSCglTF()) {
        delete scwFile;
        scwFile = new SCW::File(buf, length);
        scwFile->Load();
    }
    createFromFile(scwFile);
}

void Sprite3D::createFromFile(SCW::File* scwFile) {
    fileReference = scwFile;
    scene.createFromSCWFile(scwFile);

    skeleton = new Skeleton();
    skeleton->initialize(scwFile);
    createDynamicMeshes(skeleton);
}

void Sprite3D::createDynamicMeshes(Skeleton* skeleton) {
    for (int i = 0;i < scene.getMeshCount();i++) {
        dynamicMeshes.push_back(scene.getMesh(i)->createDynamicMesh(skeleton, scene.getMeshGeometry(i)));
    }
}

DynamicMesh* Sprite3D::getDynamicMesh(int index) {
    if (dynamicMeshes.size() <= index) return nullptr;
    return dynamicMeshes[index];
}
