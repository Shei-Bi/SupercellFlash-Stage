#pragma once
#include "DropGUIContainer.hpp"
#include "HomePage.h"
#include "flash/3d/SceneRenderer.hpp"

SceneRenderer* createHeroSprite(float width, float height, LogicCharacterData* character, LogicSkinData* skin) {
    auto Stage = Stage::getInstance();
    SceneRenderer* renderer = new SceneRenderer();
    renderer->bounds = Rect(width * -0.5, height * -0.5, width * 0.5, height * 0.5);
    renderer->addCharacter(character, skin);
    renderer->perspectiveProjection = glm::perspective(20.0f, width / height, 10.5f, 199.5f);
    // glm::mat4 view = camera.GetViewMatrix();
    renderer->lookAt = glm::lookAt(glm::vec3(0.0f, 5.0f, 105.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    renderer->unknownScaling = glm::inverse(glm::mat4(34.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -34.4f, 0.0f, 0.0f, 34.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
    /*34.4, 0.0, 0.0, 0.0,
     0.0, 0.0, -34.4, 0.0,
     0.0, 34.4, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0*/

    return renderer;
}
class HomePageTeamMember :public DropGUIContainer {
public:
    MovieClip* player_area;
    SceneRenderer* sceneRenderer;
    HomePage* homePage;
    HomePageTeamMember(HomePage* homePage, MovieClip* movieClip) :DropGUIContainer(), homePage(homePage) {
        player_area = movieClip->getMovieClipByName("area");
        player_area->addChildAt(this, 0);
        auto speechBubble = getSpeechBubbleClip();
        if (speechBubble) {
            speechBubble->visible = false;
            speechBubble->gotoAndStopFrameIndex(0);
        }
        auto playerStatus = getPlayerStatusClip();
        if (playerStatus) {
            playerStatus->visible = false;
        }
        refresh();
    }

    MovieClip* getSpeechBubbleClip() {
        if (player_area && player_area->parent && player_area->parent->isMovieClip()) {
            return ((MovieClip*)player_area->parent)->getMovieClipByName("bubble");
        }
    }

    MovieClip* getPlayerStatusClip() {
        if (player_area && player_area->parent && player_area->parent->isMovieClip()) {
            return ((MovieClip*)player_area->parent)->getMovieClipByName("player_status");
        }
    }

    void refresh() {
        auto clientHome = GameStateManager::getInstance()->home;

        sceneRenderer = createHeroSprite(player_area->getWidth(), player_area->getWidth(), (LogicCharacterData*)clientHome->characters[0], nullptr);
        addChildAt(sceneRenderer, 0);
    }

    void update(float deltaTime) {
        sceneRenderer->update(deltaTime);
    }
};