#pragma once
#include "data/LogicTileData.h"

class LogicTileMap;
class LogicTile {
public:
    LogicTileData* data;
    LogicTileData* originalData;
    int x;
    int y;
    bool shouldEncodeStatus;
    LogicTile(LogicTileData* data, int x, int y, LogicTileMap* tileMap) :originalData(data), x(x), y(y) {
        shouldEncodeStatus = false;
        setOriginalData(data);
    }
    void setOriginalData(LogicTileData* data) {
        setData(data);
        originalData = data;
        shouldEncodeStatus = data->isDestructible() || data->isDestructibleOvertime();
    }
    void setData(LogicTileData* data) {
        this->data = data;
    }
};