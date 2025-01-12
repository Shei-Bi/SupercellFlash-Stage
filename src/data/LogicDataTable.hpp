#include "csv/CSVNode.hpp"
#include "LogicData.h"
#include "LogicLocationThemeData.h"
#include "LogicLocationData.h"
#include "LogicGameModeVariationData.h"
#include "GlobalID.h"
#include "LogicDataTables.h"
#include "LogicCharacterData.h"
#include "LogicSkinData.h"
#include "LogicCardData.h"
#include "LogicAccessoryData.h"
#include "LogicGearBoostData.h"
#include "LogicTileData.h"

class LogicDataTable {
    int tableIndex;
    CSVTable* csvTable;
    std::vector<LogicData*> datas;
public:
    LogicDataTable(CSVTable* csvTable, int tableIndex) :csvTable(csvTable), tableIndex(tableIndex) {
        if (csvTable) {
            datas.reserve(csvTable->getRowCount());
            loadTable(csvTable);
        }
    }
    void loadTable(CSVTable* csvTable) {
        for (int i = 0;i < csvTable->getRowCount();i++) {
            CSVRow* csvRow = csvTable->rows[i];
            addItem(csvRow, ((i < csvTable->getRowCount() - 1) ? (csvTable->rows[i + 1]->rowIndex) : (csvTable->columns[0]->getSize())) - csvRow->rowIndex);
        }

    }
    void addItem(CSVRow* csvRow, int size) {
        auto item = createItem(csvRow);
        item->setCSVRow(csvRow, size);
        datas.push_back(item);
    }
    LogicData* createItem(CSVRow* csvRow) {
        switch (tableIndex)
        {
        case Character:
            return new LogicCharacterData(csvRow, this);
        case Skin:
            return new LogicSkinData(csvRow, this);
        case LocationTheme:
            return new LogicLocationThemeData(csvRow, this);
        case Location:
            return new LogicLocationData(csvRow, this);
        case GameModeVariation:
            return new LogicGameModeVariationData(csvRow, this);
        case Card:
            return new LogicCardData(csvRow, this);
        case Accessory:
            return new LogicAccessoryData(csvRow, this);
        case GearBoost:
            return new LogicGearBoostData(csvRow, this);
        case Map:
        case Credit:
        case MapTemplate:
            return new LogicData(csvRow, this);
        case Tile:
            return new LogicTileData(csvRow, this);
        default:
            abort();
        }
    }
    LogicData* getDataByName(const std::string& name, LogicData* neededBy) {
        return getDataByName(name.c_str(), neededBy);
    }
    LogicData* getDataByName(const char* name, LogicData* neededBy) {
        for (LogicData* data : datas) if (data->getName() == name) return data;
        abort();
    }
    void createReferences() {
        for (LogicData* data : datas) data->createReferences();
    }
    LogicData* getItemById(int globalID) {
        if (!globalID) return nullptr;
        int instanceID = GlobalID::getInstanceID(globalID);
        if (instanceID >= 0 && instanceID < getItemCount()) return getItemAt(instanceID);
        abort();
    }
    int getItemCount() {
        return datas.size();
    }
    LogicData* getItemAt(int index) {
        return datas[index];
    }
    int getTableIndex() {
        return tableIndex;
    }
};