#include "csv/CSVTable.h"
#include <stdio.h>
#include <algorithm>
#include "CSVColumn.hpp"
#include "CSVRow.h"

std::string EMPTY_STRING = "";
CSVTable::CSVTable(int columnSize, int rowSize) :rowSize(rowSize) {
    if (columnSize <= 0) columnSize = 4;
    columnNames.reserve(columnSize);
    columns.reserve(columnSize);
    rows.reserve(rowSize);
}
void CSVTable::addColumn(std::string& column) {
    columnNames.emplace_back(column);
}
void CSVTable::addColumnType(int type) {
    columns.push_back(new CSVColumn(type, rowSize));
}
void CSVTable::columnNamesLoaded() {
    columns.reserve(columnNames.size());
}
CSVRow* CSVTable::createRow() {
    return new CSVRow(this);
}
void CSVTable::addRow(CSVRow* row) {
    rows.push_back(row);
}
int CSVTable::getColumnRowCount() {
    return columns[0]->getSize();
}
void CSVTable::addAndConvertValue(std::string_view& element, int index) {
    if (element.length() == 0) {
        // if (columns.size() <= index) return;
        switch (columns[index]->type) {
        case 0:
            columns[index]->strings.emplace_back("");
            break;
        case 1:
            columns[index]->integers.push_back(0x7FFFFFFF);
            break;
        case 2:
            columns[index]->booleans.push_back(false);
            break;
        }
    }
    else {
        std::string stack_stuff(element);
        switch (columns[index]->type) {
        case 0:
            columns[index]->strings.emplace_back(element);
            break;
        case 1:
            columns[index]->integers.push_back(std::stoi(stack_stuff));
            break;
        case 2:
            std::transform(stack_stuff.begin(), stack_stuff.end(), stack_stuff.begin(), [](unsigned char c) {
                return std::tolower(c);
                });
            columns[index]->booleans.push_back(stack_stuff == "true" ? 1 : (stack_stuff == "false" ? 0 : (abort(), 0)));
            break;
        }
    }
}
int CSVTable::getRowCount() {
    return rows.size();
}
std::string const& CSVTable::getValueAt(int column, int row) {
    if (column < 0) return EMPTY_STRING;
    return columns[column]->getStringValue(row);
}
std::string const& CSVTable::getValue(const char* name, int row) {
    return getValueAt(getColumnIndexByName(name), row);
}
int CSVTable::getIntegerValueAt(int column, int row) {
    if (column < 0) return 0;
    return columns[column]->getIntValue(row);
}
bool CSVTable::getBooleanValueAt(int column, int row) {
    return column >= 0 && columns[column]->getBooleanValue(row);
}
int CSVTable::getColumnIndexByName(const char* name) {
    for (int i = 0;i < columnNames.size();i++) {
        std::string& string = columnNames[i];
        if (string == name) return i;
    }
    return -1;
}