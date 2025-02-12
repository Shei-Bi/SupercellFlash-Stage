#pragma once

#include "Sprite3D.h"
#include "data/LogicCharacterData.h"
#include "data/LogicSkinData.h"
#include "CharacterMaterial.h"
#include "Animator.h"
#include "FaceTexture.h"
#include "data/LogicAnimationData.h"
#include "data/LogicFaceData.h"
#include "data/LogicDataTables.h"

class SceneCharacter {
public:
    Sprite3D* sprite;
    Material* skinMaterial;
    Animator* animator;
    FaceTexture* faceTexture;
    float yaw;
    float pitch;

    SceneCharacter(LogicCharacterData* character, LogicSkinData* skin) {
        yaw = 0.0f;
        pitch = 0.0f;
        int size = std::clamp((int)(256 * Stage::getInstance()->pointSize) & 0xFFFFFFFE, 4u, 512u);
        faceTexture = new FaceTexture(size, size);
        sprite = new Sprite3D();
        animator = new Animator();
        std::string e = skin->conf->getModel();
        // ResourceManager::getSC3D("character_materials.scw");
        sprite->createFromFile(e);
        skinMaterial = new CharacterMaterial();
        skinMaterial->shader = Stage::getInstance()->uber_shader;
        skinMaterial->setDiffuseTex(ResourceManager::getImage(skin->diffuseTexture));
        skinMaterial->setStencilTex(faceTexture->texture);
        overrideMaterials(skinMaterial);

        auto t = glm::mat4(34.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -34.4f, 0.0f, 0.0f, 34.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        for (int i = 0;i < sprite->scene.getMeshCount();i++) {
            sprite->scene.setTransformation(i, glm::scale(t, glm::vec3(2.6f)));
        }

        addAnimation(1, LogicDataTables::getNonConstAnimationByName(skin->conf->getIdleAnim()), LogicDataTables::getFaceByName(skin->conf->getIdleFace()));
        addAnimation(2, LogicDataTables::getNonConstAnimationByName(skin->conf->getHappyAnim()), LogicDataTables::getFaceByName(skin->conf->getHappyFace()));
        addAnimation(6, LogicDataTables::getNonConstAnimationByName(skin->conf->getLobbyAnim()), LogicDataTables::getFaceByName(skin->conf->getLobbyFace()));
        addAnimation(7, LogicDataTables::getNonConstAnimationByName(skin->conf->getLobbyLoopAnim()), LogicDataTables::getFaceByName(skin->conf->getLobbyLoopFace()));

        setDefaultAnimation(7, 0.3f);
    }
    void overrideMaterials(Material* material) {
        for (int i = 0; i < sprite->scene.getMeshCount(); i++) {
            sprite->scene.getMesh(i)->material = material;
        }
    }

    void update(float deltaTime) {
        animator->update(sprite->skeleton, deltaTime);
        auto mc = animator->getCurrentAnimation()->faceClip;
        if (mc != faceTexture->movieClip) {
            mc->gotoAbsoluteTimeRecursive(0.0f);
            faceTexture->setMovieClip(mc);
        }

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

    void setDefaultAnimation(int index, float skipTime) {
        animator->setDefaultAnimation(index);
        // faceTexture->setMovieClip(animator->getAnimation(index)->faceClip);
    }

    void addAnimation(int index, LogicAnimationData* animation, LogicFaceData* face) {
        Animation* anim = new Animation();
        anim->animationData = animation;
        anim->faceData = face;
        anim->index = index;
        anim->valueChanged();
        auto mc = ResourceManager::getMovieClip("sc/" + face->getFileName(), face->getExportName());
        mc->setScaleX(faceTexture->width / 512.0f);
        mc->setScaleY(faceTexture->height / 512.0f);
        mc->setY(faceTexture->height);
        anim->faceClip = mc;
        animator->addAnimation(anim);
    }

    void changeAnimationTo(int index) {
        animator->changeAnimationTo(index);
    }
};