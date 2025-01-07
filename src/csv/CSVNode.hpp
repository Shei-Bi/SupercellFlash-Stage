#ifndef CSVNODE_H
#define CSVNODE_H
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "csv/CSVTable.h"
#include <string_view>


static long size(FILE* f) {
    fseek(f, 0, SEEK_END);
    auto l = ftell(f);
    fseek(f, 0, SEEK_SET);
    return l;
}
class CSVNode {
public:
    int columnType;
    CSVTable* table;
    CSVNode(std::vector<std::string_view>* lines) {
        columnType = 0;
        table = NULL;
        load(lines);
    }
    void load(std::vector<std::string_view>* lines) {
        for (auto it : *lines) parseLine(it, lines->size());
        // for(int i=0;i<)
    }
    void parseLine(std::string_view& line, int totalLines) {
        int element_start = 0;
        int elements_loaded = 0;
        std::string something_on_stack;
        while (element_start < line.length()) {
            int has_quote = 0;
            if (line[element_start] == '"') has_quote = 1;
            int element_end = line.find_first_of(has_quote ? '"' : ',', element_start + (has_quote ? 1 : 0));
            if (element_end != std::string_view::npos && has_quote)
                element_end++;
            if (element_end == std::string_view::npos) element_end = line.length();
            auto element = line.substr(element_start + has_quote, element_end - element_start - has_quote * 2);
            // std::cout << element << " start: " << element_start << "end: " << element_end << std::endl;
            element_start = element_end + 1;
            // if (element.length() == 0) continue;
            switch (columnType)
            {
            case 0:
                if (!table) {
                    table = new CSVTable(-1, totalLines - 2);
                    // table->columnNames.reserve()
                }
                something_on_stack = element;
                table->addColumn(something_on_stack);
                break;
            case 1:
                int type;
                something_on_stack = element;
                std::transform(something_on_stack.begin(), something_on_stack.end(), something_on_stack.begin(), [](unsigned char c) {
                    return std::tolower(c);
                    });
                if (something_on_stack == "string") type = 0;
                else if (something_on_stack == "int") type = 1;
                else if (something_on_stack == "boolean") type = 2;
                else {
                    printf("Invalid column type '%s', column name %s, file %s. Expecting: int/string/boolean\n", something_on_stack.c_str(), "idk", "idk");
                    abort();
                }
                table->addColumnType(type);
                break;
            case 2:
                if (elements_loaded == 0 && element.length() > 0) {
                    table->createRow();
                }
                table->addAndConvertValue(element, elements_loaded);
                break;
            }
            elements_loaded++;

        }
        if (columnType == 0) {
            columnType = 1;
            table->columnNamesLoaded();
        }
        else if (columnType == 1) columnType = 2;
    }
    static CSVNode* fromFile(const char* name) {
        clock();
        auto f = fopen(name, "rb");
        assert(f != NULL);
        if (f == NULL) return NULL;

        int length = size(f);
        auto data = new unsigned char[length];
        fread(data, 1, length, f);
        fclose(f);

        std::vector<std::string_view> lines;
        int line_start = 0;
        for (int i = 0;i < length;i++) {
            if (data[i] != '\n') continue;
            if (i - line_start >= 1) {
                lines.emplace_back((char const*)&data[line_start], i - line_start - 1);
                line_start = i + 1;
            }
        }
        // for (auto it : lines) {
        //     std::cout << it << std::endl;
        // }
        auto csvNode = new CSVNode(&lines);
        // lines.clear();
        // delete[] data;
        return csvNode;
    }
};
#endif