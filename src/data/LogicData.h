#pragma once
// #include "Supercell.hpp"
#include "csv/CSVRow.h"
class LogicDataTable;
class LogicData {
public:
    CSVRow* csvRow;
    int rowSize;
    LogicDataTable* parentTable;
    std::string iconSWF;
    std::string iconExportName;
    int globalID;
    LogicData();
    LogicData(CSVRow*, LogicDataTable*);
    void setCSVRow(CSVRow*, int);
    const std::string& getName();
    const std::string& getIconSWF();
    const std::string& getIconExportName();
    virtual void createReferences();
    int getInstanceID();
};