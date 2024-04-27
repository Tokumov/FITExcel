//
// Created by chean on 26.04.2024.
//

#include "CPos.h"

void CPos::parse(const std::string &posStr) {

        col = 0;
        row = 0;
        size_t i = 0;
        bool absCol = false, absRow = false;

        if (posStr[i] == '$') {
            absCol = true;
            i++;
        }
        while (i < posStr.length() && isalpha(posStr[i])) {
            col = col * 26 + (toupper(posStr[i]) - 'A' + 1);
            i++;
        }
        if (i < posStr.length() && posStr[i] == '$') {
            absRow = true;
            i++;
        }
        if (i < posStr.length()) {
            row = stoi(posStr.substr(i)) - 1;
        }

        // Use negative to denote absolute references
        if (absCol) col = -col;
        if (absRow) row = -row;

}