#include "Sprite3D.h"
#include "data/LogicCharacterData.h"
#include "data/LogicSkinData.h"
#include "CharacterMaterial.h"
#include "Animator.h"

class SceneCharacter {
public:
    Sprite3D* sprite;
    Material* skinMaterial;
    Animator* animator;
    SceneCharacter(LogicCharacterData* character, LogicSkinData* skin) {
        sprite = new Sprite3D();
        std::string e = skin->conf->getModel();
        sprite->createFromFile(e);
        skinMaterial = new CharacterMaterial();
        skinMaterial->shader = Stage::getInstance()->uber_shader;
        skinMaterial->setDiffuseTex(ResourceManager::getImage(skin->diffuseTexture));
        overrideMaterials(skinMaterial);

        auto t = glm::mat4(34.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -34.4f, 0.0f, 0.0f, 34.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        for (int i = 0;i < sprite->scene.getMeshCount();i++) {
            sprite->scene.setTransformation(i, glm::scale(t, glm::vec3(2.6f)));
        }

        animator = new Animator();
        std::ifstream is("assets/sc3d/colette_win.scw", std::ios_base::binary);
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
        Animation* animation = new Animation();
        animation->initializeBones(scwFile);
        animator->currentAnimation = animation;
        // abort();
    }
    void overrideMaterials(Material* material) {
        for (int i = 0; i < sprite->scene.getMeshCount(); i++) {
            sprite->scene.getMesh(i)->material = material;
        }
    }

    void update(float deltaTime) {
        animator->update(sprite->skeleton, deltaTime);

        for (int i = 0;i < sprite->scene.getMeshCount();i++) {
            sprite->skeleton->beginUpdateMesh(sprite->scene.getMesh(i), sprite->getDynamicMesh(i));
        }

        // sprite->skeleton->printDebugInfo();

        for (int i = 0;i < sprite->scene.getMeshCount();i++) {
            sprite->skeleton->updateMesh(sprite->scene.getMesh(i), sprite->getDynamicMesh(i));
        }

        for (int i = 0;i < sprite->scene.getMeshCount();i++) {
            sprite->skeleton->finishUpdateMesh(sprite->scene.getMesh(i), sprite->getDynamicMesh(i));
        }
    }
};