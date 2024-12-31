#include "csv/CSVNode.hpp"
#include "LogicData.h"
#include "LogicLocationThemeData.h"

class LogicDataTable {
public:
    int tableIndex;
    CSVTable* csvTable;
    std::vector<LogicData*> datas;
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
        case 47:
            return new LogicLocationThemeData(csvRow, this);
        default:
            abort();
        }
    }
    LogicData* getDataByName(char* name, LogicData* neededBy) {
        for (LogicData* data : datas) if (data->getName() == name) return data;
        abort();
    }
};