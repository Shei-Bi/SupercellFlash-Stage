#pragma once
// #include "Supercell.hpp"
#include "csv/CSVRow.h"
class LogicDataTable;
class LogicData {
public:
    CSVRow* csvRow;
    int rowSize;
    LogicDataTable* parentTable;
    LogicData();
    LogicData(CSVRow*, LogicDataTable*);
    void setCSVRow(CSVRow*, int);
    const std::string& getName();
};