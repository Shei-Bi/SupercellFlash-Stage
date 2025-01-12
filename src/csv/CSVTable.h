#ifndef CSVTABLE_H
#define CSVTABLE_H
#include <stdio.h>
#include <algorithm>
#include "CSVColumn.hpp"
#include "CSVRow.h"

extern std::string EMPTY_STRING;

class CSVTable {
public:
    std::vector<std::string> columnNames;
    std::vector<CSVColumn*> columns;
    std::vector<CSVRow*> rows;
    int rowSize;
    CSVTable(int columnSize, int rowSize);
    void addColumn(std::string& column);
    void addColumnType(int type);
    void columnNamesLoaded();
    CSVRow* createRow();
    void addRow(CSVRow* row);
    int getColumnRowCount();
    void addAndConvertValue(std::string_view& element, int index);
    int getRowCount();
    std::string const& getValue(const char* name, int row);
    std::string const& getValueAt(int column, int row);
    int getIntegerValueAt(int column, int row);
    bool getBooleanValueAt(int column, int row);
    int getColumnIndexByName(const char* name);

    int getArraySizeAt(CSVRow* row, int index);
};
#endif