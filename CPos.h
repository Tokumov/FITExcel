
//
// Created by chean on 26.04.2024.
//

#ifndef SPREADSHEETPROCESSOR_CPOS_H
#define SPREADSHEETPROCESSOR_CPOS_H
#include <iostream>
using namespace std;
class CPos
{
public:
    CPos                                    ( std::string_view                      str ):position(str){
        parse((string)str);
    }
    CPos(){

    }
    void parse(const string& posStr) ;
    CPos(string_view id, int baseRow, int baseCol) {
        int i = 0;
        col = 0;
        // Parse column part which may contain '$'
        bool colPart = true;
        while (i < id.size() && (isalpha(id[i]) || id[i] == '$')) {
            if (id[i] == '$') {
                if (colPart) isColAbsolute = true;
                i++;
                continue;
            }
            col = col * 26 + (toupper(id[i]) - 'A' + 1);
            i++;
        }
        if (i < id.size() && id[i] == '$') {
            isRowAbsolute = true;
            i++;
        }
        if (i < id.size()) {
            row = stoi(string(id.substr(i))) - 1;
        }

        if (!isColAbsolute) col += baseCol;
        if (!isRowAbsolute) row += baseRow;
    }
    // Overload the less operator for using CPos as a key in std::map
    bool operator<(const CPos& other) const {
        return row == other.row ? col < other.col : row < other.row;
    }
    bool isRowAbsolute = false;
    bool isColAbsolute = false;
    int row=0;
    int col=0;
private:
    string position;
};


#endif //SPREADSHEETPROCESSOR_CPOS_H
