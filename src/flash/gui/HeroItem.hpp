#pragma once
#include "GameButton.h"
#include "ResourceManager.h"

class HeroItem : public GameButton {
public:
    HeroItem() {
        setMovieClip(ResourceManager::getMovieClip("sc/ui.sc", "brawler_list"), true);
    }
};