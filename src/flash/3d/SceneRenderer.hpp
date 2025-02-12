#pragma once

#include "SceneCharacter.hpp"
#include "data/LogicCharacterData.h"
#include "data/LogicSkinData.h"
#include "flash/Sprite.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Impostor.h"

class SceneRenderer :public Sprite {
public:
    SceneCharacter* character;
    Rect bounds;
    glm::mat4 perspectiveProjection;
    glm::mat4 lookAt;
    glm::mat4 unknownScaling;
    glm::mat4 matrix;
    Impostor* characterImpostor;
    RenderTarget* renderTarget;
    void addCharacter(LogicCharacterData* character, LogicSkinData* skin) {
        if (!skin) skin = character->defaultSkin;
        this->character = new SceneCharacter(character, skin);
        renderTarget = new RenderTarget(4);
        characterImpostor = new Impostor(renderTarget);
    }
    bool render(Matrix2x3* mat, ColorTransform* c, int rc, float deltaTime) {
        Stage* Stage = Stage::getInstance();
        setScale(3.2512f);
        Matrix2x3* n = new Matrix2x3(Matrix, *mat);

        float top = n->applyY(bounds.left, bounds.top) - n->ty;
        float bottom = Stage->matrixY * Stage->pointSize - (mat->applyY(getX(), getY()) - mat->ty) - mat->ty;
        float mid = n->applyY(bounds.right, bounds.bottom) - n->ty;
        float X = mat->applyX(getX(), getY());
        float right = X + n->applyX(bounds.right, bounds.bottom) - n->tx;
        float left = X + n->applyX(bounds.left, bounds.top) - n->tx;
        glm::mat4 transformation = glm::mat4(1.0f);
        /*
         8 12 16 20
        24 28 32 36
        40 44 48 52
        56 60 64 68
        */
        transformation[0][0] = (right - left) / (Stage->right - Stage->left);
        transformation[1][1] = (mid - top) / (Stage->bottom - Stage->top);
        transformation[3][0] = ((right - left) - (Stage->right - Stage->left) + (left) * 2.0f) / (Stage->right - Stage->left);
        transformation[3][1] = ((mid - top) + (bottom - mid) * 2.0f - (Stage->bottom - Stage->top)) / (Stage->bottom - Stage->top);
        transformation *= perspectiveProjection;
        // 0.37669089436531067
        // 0.6696727275848389
        // 0.016308657824993134
        // -0.5951389670372009

        character->faceTexture->update(deltaTime);

        Stage->start3D();

        renderTarget->reshape(Stage->right, Stage->bottom);
        renderTarget->begin();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        character->sprite->render(
            transformation *
            (lookAt *
                (glm::rotate(glm::mat4(1.0f), glm::radians(character->yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    (glm::rotate(glm::mat4(1.0f), glm::radians(character->pitch), glm::vec3(1.0f, 0.0f, 0.0f)) * unknownScaling))));
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        renderTarget->end();
        characterImpostor->render();
        delete n;
        return false;
    }

    void update(float deltaTime) {
        character->update(deltaTime);
    }
};