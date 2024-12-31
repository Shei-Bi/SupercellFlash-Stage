#include "CSVRow.h"
#include "csv/CSVTable.h"

CSVRow::CSVRow(CSVTable* table) :table(table) {
    rowIndex = table->getColumnRowCount();
    table->addRow(this);
}
const std::string& CSVRow::getName() {
    return table->getValueAt(0, rowIndex);
}
int CSVRow::getIntegerValueAt(int column) {
    return table->getIntegerValueAt(column, rowIndex);
}
const std::string& CSVRow::getValue(const char* columnName, int rowOffset) {
    return table->getValue(columnName, rowIndex + rowOffset);
}
const std::string& CSVRow::getValueAt(int column) {
    return table->getValueAt(column, rowIndex);
}