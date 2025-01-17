#pragma once
#include <vector>
#include <assert.h>
#include <string>

class CSVColumn {
public:
    std::vector<std::string> strings;
    std::vector<unsigned char> booleans;
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
        int result = integers[row];
        if (result == 0x7FFFFFFF) return 0;
        return result;
    }
    bool getBooleanValue(int row) {
        return booleans[row] == 1;
    }

    int getArraySize(int start, int end) {
        end--;
        switch (type)
        {
        case 0:
            while (end > start && strings[end].size() == 0) end--;
            break;
        case 1:
            while (end > start && integers[end] == 0x7FFFFFFF) end--;
            break;
        case 2:
            while (end > start && booleans[end] == 2) end--;
            break;
        default:
            abort();
        }
        return end - start + 1;
    }
};