#pragma once
#include "ScrollArea.hpp"
#include "flash/Stage.h"

class ListContainer {
public:
    MovieClip* clip;
    ScrollArea* scrollArea;
    std::vector<DisplayObject*> entries;

    ListContainer(MovieClip* movieClip) :clip(movieClip) {
        TextField* tf = movieClip->getTextFieldByName("scroll_area");
        if (!tf) tf = movieClip->getTextFieldByName("list");
        Rect scrollAreaBounds = tf->bounds;
        Rect globalBounds;
        tf->getGlobalBounds(&globalBounds, false);

        Stage* Stage = Stage::getInstance();
        {
            scrollAreaBounds.bottom += Stage->matrixY - globalBounds.bottom;
        }
        {
            scrollAreaBounds.left -= Stage->matrixX / 2.0f;
            scrollAreaBounds.right += Stage->matrixX / 2.0f;
        }
        auto list_bg = movieClip->getMovieClipByName("list_bg");
        if (list_bg) {
            {
                // list_bg.seth
            }
        }

        scrollArea = new ScrollArea(scrollAreaBounds.getWidth(), scrollAreaBounds.getHeight(), 1);
        movieClip->changeTimelineChild(tf, scrollArea);
        scrollArea->setPixelSnappedXY(scrollAreaBounds.left + tf->getX(), scrollAreaBounds.top + tf->getY());
        delete tf;
    }
    void addEntry(DisplayObject* entry) {
        entries.push_back(entry);
    }
    void refreshEntryPositions(int itemsPerRow, float startingHeight, float spacing, float startingWidth) {
        scrollArea->removeAllContent();
        int rowSize = 0;
        for (int i = 0;i < entries.size();i++) {
            auto entry = entries[i];
            float width = entry->getWidth();
            float height = entry->getHeight();
            entry->setPixelSnappedXY(
                scrollArea->width * 0.5f + width * (rowSize + 0.5f - itemsPerRow * 0.5f),
                startingHeight + height * 0.5f);
            scrollArea->addContentDontUpdateBounds(entry);
            rowSize++;
            if (rowSize == itemsPerRow) {
                rowSize = 0;
                startingHeight += height;
                startingWidth += width;
            }
        }
    }
    void clearEntries() {
        scrollArea->removeAllContent();
        entries.clear();
    }
};