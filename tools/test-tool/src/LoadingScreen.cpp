#include <LoadingScreen.h>
#include <ResourceManager.h>
#include <GameMain.h>
#include <flash/Stage.h>
#include <LogicVersion.h>
#include <GameStateManager.h>

void LoadingScreen::update(float sinceStart, float deltaTime) {
    updateLoadingProgress(deltaTime);
}
void LoadingScreen::enter() {
    loading_screen = ResourceManager::getMovieClip("sc/loading.sc", "loading_screen");
    showOnlyLocalizedLogo("CN", loading_screen);
    loading_screen->setChildVisible("icon_prc_age", LogicVersion::isChinaVersion());

    MovieClip* loading_bar = loading_screen->getMovieClipByName("loading_bar");
    progress_bar = loading_bar->getMovieClipByName("progress_bar");
    text = loading_bar->getTextFieldByName("text");

    loading_screen->setChildVisible("supercell_id", false);
    loading_screen->setChildVisible("buttons_tencent", false);
    loading_screen->setChildVisible("buttons_yoozoo", false);

    updateLoadingProgress(0.0);

    Stage* Stage = Stage::getInstance();
    loading_screen->setPixelSnappedXY(Stage->matrixX / 2, Stage->matrixY / 2);
    GameMain::getInstance()->loadingScreenSprite->addChild(loading_screen);

    loading_bar->setY(loading_bar->getY() + (Stage->matrixY - 576.0f) * 0.4);
}
void LoadingScreen::setAlpha(float a) {
    alpha = a;
}
void LoadingScreen::updateLoadingProgress(float deltaTime) {
    if (loading_screen) {
        loading_screen->setAlpha(alpha);
        GameStateManager* GameStateManager = GameStateManager::getInstance();
        if (GameStateManager->currentState) {
            float progress = fmax(0.0, fmin(1.0, GameStateManager->currentState->getLoadingProgress()));
            progress_bar->gotoAndStopFrameIndex((int)(0.5f + progress * (progress_bar->getTotalFrames() - 1)));
        }
    }
}
void LoadingScreen::showOnlyLocalizedLogo(char* languageCode, MovieClip* movieClip) {
    MovieClip* logo_EN = movieClip->getMovieClipByName("logo_EN");
    MovieClip* logo_KR = movieClip->getMovieClipByName("logo_KR");
    MovieClip* logo_JP = movieClip->getMovieClipByName("logo_JP");
    MovieClip* logo_CNT = movieClip->getMovieClipByName("logo_CNT");
    MovieClip* logo_CNS = movieClip->getMovieClipByName("logo_CNS");
    if (logo_KR) logo_KR->visible = false;
    if (logo_JP) logo_JP->visible = false;
    if (logo_CNT) logo_CNT->visible = false;
    if (logo_CNS) logo_CNS->visible = false;
}