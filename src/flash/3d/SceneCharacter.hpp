#include "Sprite3D.h"
#include "data/LogicCharacterData.h"
#include "data/LogicSkinData.h"
#include "CharacterMaterial.h"

class SceneCharacter {
public:
    Sprite3D* sprite;
    Material* skinMaterial;
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
    }
    void overrideMaterials(Material* material) {
        for (int i = 0; i < sprite->scene.getMeshCount(); i++) {
            sprite->scene.getMesh(i)->material = material;
        }
    }
};