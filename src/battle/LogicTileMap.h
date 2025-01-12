#pragma once
#include "data/LogicLocationData.h"
#include "data/LogicDataTables.h"
#include "data/LogicDataTable.hpp"
#include "LogicTile.h"

class LogicTileMap {
public:
    LogicTile** tiles;
    int mapWidth;
    int mapHeight;
    void generateTileMap(LogicLocationData* location) {
        loadMap(location->getMap());
    }

    void loadMap(const std::string& mapName) {
        auto maps = TABLES[Map];
        LogicData* data = nullptr;
        int c = maps->getItemCount();
        for (int i = 0;i < c;i++) {
            data = maps->getItemAt(i);
            if (data->csvRow->getName() == mapName) goto find;
        }
        abort();
    find:
        auto row = data->csvRow;
        auto codes = std::vector<std::string>();
        codes.reserve(row->getArraySizeAt(1));
        for (int i = 0;i < row->getArraySizeAt(1);i++) {
            codes.emplace_back(row->getValueAt(1, i));
        }
        mapWidth = codes[0].size();
        mapHeight = codes.size();
        tiles = new LogicTile * [mapWidth * mapHeight];
        for (int i = 0;i < mapWidth * mapHeight;i++) {
            int x = i % mapWidth;
            int y = i / mapWidth;
            LogicTileData* data = LogicDataTables::getOpenTileData();
            for (int i = 0;i < TABLES[Tile]->getItemCount();i++) {
                auto itemAt = (LogicTileData*)TABLES[Tile]->getItemAt(i);
                if (itemAt->getTileCode() == codes[y][x]) {
                    data = itemAt;
                    break;
                }
            }
            tiles[i] = new LogicTile(data, x, y, this);
        }
    }

    LogicTile* getTile(int x, int y) {
        if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) abort();
        return tiles[y * mapWidth + x];
    }
    // static LogicTileData* getLogicTileData(int x, int y) {

    // }
};