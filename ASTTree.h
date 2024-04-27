//
// Created by chean on 27.04.2024.
//

#ifndef SPREADSHEETPROCESSOR_ASTTREE_H
#define SPREADSHEETPROCESSOR_ASTTREE_H

#include "CPos.h"
#include "expression.h"
#include <iostream>
#include <variant>
#include <map>
#include <stack>
#include <cmath>
using CValue = variant<monostate, double, string>;
class ASTTree {
public:
    virtual ~ASTTree() {}
    virtual CValue evaluate(map<CPos, CValue>& cells, const CPos& basePos) const = 0;
};


#endif //SPREADSHEETPROCESSOR_ASTTREE_H
