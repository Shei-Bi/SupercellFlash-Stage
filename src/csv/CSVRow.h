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
    bool getBooleanValueAt(int);
    const std::string& getValue(const char*, int);
    const std::string& getValueAt(int);
    const std::string& getValueAt(int, int);

    int getArraySizeAt(int index);
};