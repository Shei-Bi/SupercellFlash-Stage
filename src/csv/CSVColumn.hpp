#pragma once
#include <vector>
#include <assert.h>
#include <string>

class CSVColumn {
public:
    std::vector<std::string> strings;
    std::vector<bool> booleans;
    std::vector<int> integers;
    int type;
    CSVColumn(int type, int rowSize) {
        this->type = type;
        switch (type)
        {
        case 0:
            strings.reserve(rowSize);
            break;
        case 1:
            integers.reserve(rowSize);
            break;
        case 2:
            booleans.reserve(rowSize);
            break;
        }
    }
    int getSize() {
        switch (type)
        {
        case 0:
            return strings.size();
        case 1:
            return integers.size();
        case 2:
            return booleans.size();
        default:
            abort();
        }
    }
    std::string& getStringValue(int row) {
        return strings[row];
    }
    int getIntValue(int row) {
        return integers[row];
    }
    bool getBooleanValue(int row) {
        return booleans[row];
    }
};