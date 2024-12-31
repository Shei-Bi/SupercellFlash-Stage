#include "LogicLocationThemeData.h"
#include "csv/CSVTable.h"
int LogicLocationThemeData::sm_columnIndexMapPreviewBGColorRed = -1;
int LogicLocationThemeData::sm_columnIndexMapPreviewBGColorGreen = -1;
int LogicLocationThemeData::sm_columnIndexMapPreviewBGColorBlue = -1;
LogicLocationThemeData::LogicLocationThemeData(CSVRow* csvRow, LogicDataTable* parentTable) :LogicData(csvRow, parentTable) {
    ;
}
void LogicLocationThemeData::resetStatics(CSVTable* table) {
    sm_columnIndexMapPreviewBGColorRed = table->getColumnIndexByName("MapPreviewBGColorRed");
    sm_columnIndexMapPreviewBGColorGreen = table->getColumnIndexByName("MapPreviewBGColorGreen");
    sm_columnIndexMapPreviewBGColorBlue = table->getColumnIndexByName("MapPreviewBGColorBlue");
}