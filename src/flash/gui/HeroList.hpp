#pragma once
#include "DropGUIContainer.hpp"
#include "ListContainer.hpp"
#include "HeroItem.hpp"

class HeroList :public DropGUIContainer {
public:
    ListContainer* listContainer;
    DropGUIContainer* hero_list_header_element;
    std::vector<HeroItem*>* heroItems;
    HeroList() :DropGUIContainer() {
        heroItems = nullptr;
        setMovieClip(ResourceManager::getMovieClip("sc/ui.sc", "hero_list"));
        listContainer = new ListContainer(getMovieClip());

        hero_list_header_element = new DropGUIContainer();
        hero_list_header_element->setMovieClip(ResourceManager::getMovieClip("sc/ui.sc", "hero_list_header_element"));
        getMovieClip()->addChild(hero_list_header_element);

        createItems();
    }
    void refresh() {
        ;
    }
    void createItems() {
        if (heroItems) {
            listContainer->clearEntries();
            for (auto item : *heroItems) {
                delete item;
            }
            delete heroItems;
        }
        heroItems = new std::vector<HeroItem*>();
        createHeroItems();
    }
    void createHeroItems() {
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->addEntry(new HeroItem());
        listContainer->refreshEntryPositions(3, 0.0f, 0.0f, 0.0f);
    }
};