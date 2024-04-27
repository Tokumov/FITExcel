//
// Created by chean on 26.04.2024.
//

#include "SimpleExpressionEvaluator.h"

void SimpleExpressionEvaluator::opAdd() {
    if (evalStack.size() < 2) throw runtime_error("Not enough operands for addition");
    auto right = evalStack.top(); evalStack.pop();
    auto left = evalStack.top(); evalStack.pop();
    if (!holds_alternative<double>(left) || !holds_alternative<double>(right)) {
        throw runtime_error("Invalid types for addition");
    }
    evalStack.push(get<double>(left) + get<double>(right));


}

void SimpleExpressionEvaluator::opSub() {
    if (evalStack.size() < 2) throw runtime_error("Not enough operands for subtraction");
    auto right = evalStack.top(); evalStack.pop();
    auto left = evalStack.top(); evalStack.pop();
    if (!holds_alternative<double>(left) || !holds_alternative<double>(right)) {
        throw runtime_error("Invalid types for subtraction");
    }
    evalStack.push(get<double>(left) - get<double>(right));
}

void SimpleExpressionEvaluator::opMul() {
    if (evalStack.size() < 2) throw runtime_error("Not enough operands for multiplication");
    auto right = evalStack.top(); evalStack.pop();
    auto left = evalStack.top(); evalStack.pop();
    if (!holds_alternative<double>(left) || !holds_alternative<double>(right)) {
        throw runtime_error("Invalid types for multiplication");
    }
    evalStack.push(get<double>(left) * get<double>(right));
}

void SimpleExpressionEvaluator::opDiv() {
    if (evalStack.size() < 2) throw runtime_error("Not enough operands for division");
    auto right = evalStack.top(); evalStack.pop();
    auto left = evalStack.top(); evalStack.pop();
    if (!holds_alternative<double>(left) || !holds_alternative<double>(right) || get<double>(right) == 0) {
        throw runtime_error("Invalid types for division or divide by zero");
    }
    evalStack.push(get<double>(left) / get<double>(right));
}

void SimpleExpressionEvaluator::opPow() {
    if (evalStack.size() < 2) throw runtime_error("Not enough operands for power operation");
    auto right = evalStack.top(); evalStack.pop();
    auto left = evalStack.top(); evalStack.pop();
    if (!holds_alternative<double>(left) || !holds_alternative<double>(right)) {
        throw runtime_error("Invalid types for power operation");
    }
    evalStack.push(pow(get<double>(left), get<double>(right)));
}

void SimpleExpressionEvaluator::valNumber(double num) {
    evalStack.push(num);
}
void SimpleExpressionEvaluator:: valReference  ( std::string                           val )  {
    basePos.col=0;
    basePos.row=0;
    CPos pos(val, basePos.row, basePos.col);
    auto it = cells.find(pos);
if (it == cells.end() || !holds_alternative<double>(it->second)) {
throw std::runtime_error("Invalid reference or non-numeric cell referenced.");
}
evalStack.push(std::get<double>(it->second));
/*cerr << "Resolving reference for: " << val << endl;
CPos pos(val);
auto it = cells.find(pos);
if (it == cells.end()) {
    cerr << "Reference to undefined cell: " << val << endl;
    throw runtime_error("Reference to undefined cell");
}
if (!holds_alternative<double>(it->second)) {
    cerr << "Reference to non-numeric cell: " << val << endl;
    throw runtime_error("Reference to non-numeric cell");
}
double value = get<double>(it->second);
evalStack.push(value);
cerr << "Pushed to stack: " << value << " from cell " << val << endl;*/
}
double SimpleExpressionEvaluator::getResult() {
    if (evalStack.empty()) throw runtime_error("Evaluation stack is empty");
    auto result = evalStack.top();
    evalStack.pop();
    if (!evalStack.empty()) throw runtime_error("Evaluation error: Stack not empty after evaluation");
    return get<double>(result);
}
void         SimpleExpressionEvaluator::              opNeg                                   () {
if (evalStack.size() < 2) throw runtime_error("Insufficient values for comparison");
auto right = evalStack.top(); evalStack.pop();
auto left = evalStack.top(); evalStack.pop();
if (left.index() != right.index()) {
evalStack.push(1.0); // true
} else {
if (holds_alternative<double>(left)) {
evalStack.push(get<double>(left) != get<double>(right) ? 1.0 : 0.0);
} else {
evalStack.push(get<string>(left) != get<string>(right) ? 1.0 : 0.0);
}
}

}
void               SimpleExpressionEvaluator::        opEq                                    () {
if (evalStack.size() < 2) throw runtime_error("Insufficient values for comparison");
auto right = evalStack.top(); evalStack.pop();
auto left = evalStack.top(); evalStack.pop();
if (left.index() != right.index()) {
evalStack.push(0.0); // false
} else {
if (holds_alternative<double>(left)) {
evalStack.push(get<double>(left) == get<double>(right) ? 1.0 : 0.0);
} else {
evalStack.push(get<string>(left) == get<string>(right) ? 1.0 : 0.0);
}
}
}
void              SimpleExpressionEvaluator::         opNe                                    () {

}
void            SimpleExpressionEvaluator::           opLt                                    () {
performComparison([](auto a, auto b) { return a < b; });
}
void           SimpleExpressionEvaluator::            opLe                                    () {

}
void           SimpleExpressionEvaluator::            opGt                                    () {

}
void             SimpleExpressionEvaluator::          opGe                                    () {

}
void              SimpleExpressionEvaluator::         valString                               ( std::string                           val ) {
evalStack.push(val);
}
void           SimpleExpressionEvaluator::            valRange                                ( std::string                           val ) {
cout<<"hui?";
}
void               SimpleExpressionEvaluator::        funcCall                                ( std::string                           fnName,
                                                                     int                                   paramCount ) {
cout<<"hui?";

}
template<typename Func>
void SimpleExpressionEvaluator:: performComparison(Func comp) {
    if (evalStack.size() < 2) throw runtime_error("Insufficient values for comparison");
    auto right = evalStack.top(); evalStack.pop();
    auto left = evalStack.top(); evalStack.pop();
    if (left.index() != right.index()) throw runtime_error("Type mismatch for comparison");
    if (holds_alternative<double>(left)) {
        evalStack.push(comp(get<double>(left), get<double>(right)) ? 1.0 : 0.0);
    } else if (holds_alternative<string>(left)) {
        evalStack.push(comp(get<string>(left), get<string>(right)) ? 1.0 : 0.0);
    } else {
        throw runtime_error("Unsupported types for comparison");
    }
}