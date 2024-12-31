#pragma once
#include <string>

class CSVTable;

class CSVRow {
public:
    CSVTable* table;
    int rowIndex;
    CSVRow(CSVTable* table);
    const std::string& getName();
    int getIntegerValueAt(int);
};