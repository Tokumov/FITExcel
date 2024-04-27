//
// Created by chean on 26.04.2024.
//

#ifndef SPREADSHEETPROCESSOR_SIMPLEEXPRESSIONEVALUATOR_H
#define SPREADSHEETPROCESSOR_SIMPLEEXPRESSIONEVALUATOR_H

#include "CPos.h"
#include "expression.h"
#include <iostream>
#include <variant>
#include <map>
#include <stack>
#include <cmath>
using CValue = variant<monostate, double, string>;
class SimpleExpressionEvaluator : public CExprBuilder  {
private:

    map<CPos, CValue>& cells;
    CPos basePos;
public:
    stack<CValue> evalStack;
    SimpleExpressionEvaluator(map<CPos, CValue>& cells) : cells(cells) {}
    void opAdd() override;
    void opSub() override;
    void opMul() override;
    void opDiv() override;
    void opPow() override ;
    void valNumber(double num) override;
    void  valReference  ( std::string                           val ) override ;
    double getResult() ;
    void                       opNeg                                   () override;
    void                       opEq                                    () override;
    void                       opNe                                    () override;
    void                       opLt                                    () override;
    void                       opLe                                    () override;
    void                       opGt                                    () override;
    void                       opGe                                    () override;
    void                       valString                               ( std::string                           val ) override;
    void                       valRange                                ( std::string                           val ) override;
    void                       funcCall                                ( std::string                           fnName,
                                                                         int                                   paramCount ) override;


    template<typename Func>
    void performComparison(Func comp);
};


#endif //SPREADSHEETPROCESSOR_SIMPLEEXPRESSIONEVALUATOR_H
