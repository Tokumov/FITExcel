#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "expression.h"
using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned                     SPREADSHEET_CYCLIC_DEPS                 = 0x01;
constexpr unsigned                     SPREADSHEET_FUNCTIONS                   = 0x02;
constexpr unsigned                     SPREADSHEET_FILE_IO                     = 0x04;
constexpr unsigned                     SPREADSHEET_SPEED                       = 0x08;
constexpr unsigned                     SPREADSHEET_PARSER                      = 0x10;
#endif /* __PROGTEST__ */
using namespace std;
using CValue = variant<monostate, double, string>;

#include "CPos.h"
#include <regex>
struct ExpressionNode {
    enum Type { Number, String, Reference, Operator, Function };
    Type type;
    std::variant<double, std::string, CPos> value;
    std::vector<std::shared_ptr<ExpressionNode>> children;

    ExpressionNode(Type type, std::variant<double, std::string, CPos> value)
            : type(type), value(value) {}

    static std::shared_ptr<ExpressionNode> createNumberNode(double val) {
        return std::make_shared<ExpressionNode>(Number, val);
    }

    static std::shared_ptr<ExpressionNode> createStringNode(const std::string& val) {
        return std::make_shared<ExpressionNode>(String, val);
    }

    static std::shared_ptr<ExpressionNode> createReferenceNode(const CPos& val) {
        return std::make_shared<ExpressionNode>(Reference, val);
    }

    static std::shared_ptr<ExpressionNode> createOperatorNode(const std::string& op, std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right) {
        auto node = std::make_shared<ExpressionNode>(Operator, op);
        node->children.push_back(left);
        node->children.push_back(right);
        return node;
    }
};
std::ostream& operator<<(std::ostream& os, const CPos& pos) {
    // Assuming CPos has members `row` and `col` that represent the row and column indices
    int row = pos.row;
    int col = pos.col;
    std::string colStr;

    // Convert column index to Excel-style column letter(s)
    while (col > 0) {
        int rem = (col - 1) % 26;
        colStr = static_cast<char>(rem + 'A') + colStr;
        col = (col - 1) / 26;
    }

    // Output the column letters followed by the row number
    os << colStr << (row + 1); // +1 because rows are usually 1-indexed in Excel
    return os;
}
class CSpreadsheet;
class SimpleExpressionEvaluator : public CExprBuilder {
private:
    CSpreadsheet& spreadsheet;
    //map<CPos, CValue> &cells;
    CPos basePos;
public:
   // stack<CValue> evalStack;
    stack<std::shared_ptr<ExpressionNode>> evalStack2;
    stack<std::shared_ptr<ExpressionNode>> evalStack3;
    SimpleExpressionEvaluator(CSpreadsheet& spreadsheet) : spreadsheet(spreadsheet) {}

   // SimpleExpressionEvaluator(map<CPos, CValue> &cells) : cells(cells) {}

    void opAdd() override {
        applyBinaryOp("+");
    }

    void opSub() override {
        applyBinaryOp("-");
    }

    void opMul() override {
        applyBinaryOp("*");
    }

    void opDiv() override {
        applyBinaryOp("/");
    }

    void opPow() override {
        applyBinaryOp("^");
    }

    void valNumber(double num) override {
        evalStack2.push(ExpressionNode::createNumberNode(num));


    }

    void valReference(std::string val) override {

        CPos pos(val, basePos.row, basePos.col);
        evalStack2.push(ExpressionNode::createReferenceNode(pos));
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

    CValue getResult() {
        if (evalStack2.empty())
            throw std::runtime_error("Evaluation stack is empty");

        if (evalStack2.size() > 1)
            throw std::runtime_error("Evaluation error: More than one value left in stack after evaluation");

        auto resultNode = evalStack2.top();
        evalStack2.pop(); // Remove the node from the stack now that we are processing it

        // Return the final evaluated node directly since it is already a CValue
        return evaluateNode(resultNode);
    }

    void opNeg() override {
        auto operand = evalStack2.top();
        evalStack2.pop();
        auto value = evaluateNode(operand);
        if (!holds_alternative<double>(value)) {
            throw std::runtime_error("Unary negation applied to a non-numeric value");
        }
        evalStack2.push(ExpressionNode::createNumberNode(-get<double>(value)));

    }

    void opEq() override {
       /* if (evalStack.size() < 2) throw runtime_error("Insufficient values for comparison");
        auto right = evalStack.top();
        evalStack.pop();
        auto left = evalStack.top();
        evalStack.pop();
        if (left.index() != right.index()) {
            evalStack.push(0.0); // false
        } else {
            if (holds_alternative<double>(left)) {
                evalStack.push(get<double>(left) == get<double>(right) ? 1.0 : 0.0);
            } else {
                evalStack.push(get<string>(left) == get<string>(right) ? 1.0 : 0.0);
            }
        }*/
    }

    void opNe() override {
       /* if (evalStack.size() < 2) throw runtime_error("Insufficient values for comparison");
        auto right = evalStack.top();
        evalStack.pop();
        auto left = evalStack.top();
        evalStack.pop();
        if (left.index() != right.index()) {
            evalStack.push(0.0); // false
        } else {
            if (holds_alternative<double>(left)) {
                evalStack.push(get<double>(left) == get<double>(right) ? 1.0 : 0.0);
            } else {
                evalStack.push(get<string>(left) == get<string>(right) ? 1.0 : 0.0);
            }
        }*/
    }

    void opLt() override {
        performComparison([](auto a, auto b) { return a < b; });
    }

    void opLe() override {

    }

    void opGt() override {

    }

    void opGe() override {

    }

    void valString(std::string val) override {
        evalStack2.push(ExpressionNode::createStringNode(val));
    }

    void valRange(std::string val) override {
        cout << "hui?";
    }

    void funcCall(const std::string fnName, int paramCount) override {
        std::vector<std::shared_ptr<ExpressionNode>> params;
        for (int i = 0; i < paramCount; ++i) {
            params.push_back(evalStack2.top());
            evalStack2.pop();
        }
        evalStack2.push(applyFunction(fnName, params));
    }

    std::shared_ptr<ExpressionNode> applyFunction(const std::string& fnName, const std::vector<std::shared_ptr<ExpressionNode>>& params) {
        // Example: Implementing SUM function
        if (fnName == "sum") {
            double sum = 0;
            for (auto& node : params) {
                auto value = evaluateNode(node);
                if (std::holds_alternative<double>(value)) {
                    sum += std::get<double>(value);
                }
            }
            return ExpressionNode::createNumberNode(sum);
        }
        throw std::runtime_error("Function not implemented");
    }

    template<typename Func>
    void performComparison(Func comp) {
        /*if (evalStack2.size() < 2) throw runtime_error("Insufficient values for comparison");
        auto right = evalStack2.top();
        evalStack2.pop();
        auto left = evalStack2.top();
        evalStack2.pop();
        if (left.index() != right.index()) throw runtime_error("Type mismatch for comparison");
        if (holds_alternative<double>(left)) {
            evalStack.push(comp(get<double>(left), get<double>(right)) ? 1.0 : 0.0);
        } else if (holds_alternative<string>(left)) {
            evalStack.push(comp(get<string>(left), get<string>(right)) ? 1.0 : 0.0);
        } else {
            throw runtime_error("Unsupported types for comparison");
        }*/
    }

    void applyBinaryOp(const std::string &op) {
        auto right = evalStack2.top();
        evalStack2.pop();
        auto left = evalStack2.top();
        evalStack2.pop();
        evalStack2.push(ExpressionNode::createOperatorNode(op, left, right));
    }

    CValue applyOperator(const std::string& op, const CValue& leftVal, const CValue& rightVal) {
        double left = holds_alternative<double>(leftVal) ? get<double>(leftVal) : throw std::runtime_error("Left operand is not a number");
        double right = holds_alternative<double>(rightVal) ? get<double>(rightVal) : throw std::runtime_error("Right operand is not a number");

        if (op == "+") return left + right;
        if (op == "-") return left - right;
        if (op == "*") return left * right;
        if (op == "/") {
            if (right == 0) throw std::runtime_error("Division by zero");
            return left / right;
        }
        if (op == "^") return pow(left, right);

        throw std::runtime_error("Invalid operator");
    }

    CValue evaluateNode(const std::shared_ptr<ExpressionNode>& node);


};


class CSpreadsheet
{
public:
    static unsigned                    capabilities                            ()
    {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }
    CSpreadsheet                            (){

    }
    bool                               load                                    ( std::istream                        & is ){
        std::string line, cellPos, expr;
        while (std::getline(is, line)) {
            std::istringstream iss(line);
            if (!std::getline(iss, cellPos, ',') || !std::getline(iss, expr)) return false;
            CPos pos(cellPos);
            setCell(pos, unescape(expr));
        }
        return is.eof();
    }
    bool                               save                                    ( std::ostream                        & os ) const{
        for (const auto &[pos, val] : expressions) {
            os << pos<< ',' << escape(val) << '\n';
            if (os.fail()) return false;
        }
        return true;
    }
    bool                               setCell                                 ( CPos                                  pos,
                                                                                 std::string                           contents ){
        if (contents[0] == '=') {
            expressions[pos] = contents.substr(0);  // Skip the '=' sign
            values.erase(pos);  // Invalidate the cached value
        } else {
            // Otherwise, try to convert it to a double or store as a string
            try {
                double num = stod(contents);
                values[pos] = num;
            } catch (const invalid_argument&) {
                values[pos] = contents;
            }
        }
        return true;
    }
    CValue                             getValue                                ( CPos                                  pos ){
        if (isValueCurrent(pos) && expressions.find(pos) == expressions.end()) {
            return values[pos];
        }

        // If there's an expression to evaluate, do so.
        if (expressions.find(pos) != expressions.end()) {
            SimpleExpressionEvaluator evaluator(*this);
            cout<<expressions[pos]<<endl;
            parseExpression(expressions[pos], evaluator);
            // Cache the new value.
            values[pos] = evaluator.getResult();
        }

        // If the value is now up to date, return it.
        if (isValueCurrent(pos)) {
            return values[pos];
        }

        // If we reach this point, there's no value or expression for the cell.
        return monostate();
    }
    void                               copyRect                                ( CPos                                  dst,
                                                                                 CPos                                  src,
                                                                                 int                                   w = 1,
                                                                                 int                                   h = 1 ){
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                CPos offset(y, x);
                CPos from = src + offset;
                CPos to = dst + offset;

                if (expressions.find(from) != expressions.end()) {
                    // We have a formula to copy and adjust
                    std::string formula = expressions[from];
                    cout<<formula<<endl;
                    expressions[to] = moveRelativeReferencesBy(formula, offset);
                    values.erase(to); // Invalidate the cached value
                } else if (values.find(from) != values.end()) {
                    // It's a direct value, copy it over
                    values[to] = values[from];
                }
            }
        }

    }
    std::string moveRelativeReferencesBy(const std::string& formula, const CPos& offset) {
        // Here you'd implement the logic to parse the formula and adjust the cell references
        // I'll use the `adjustFormula` you provided previously
        return adjustFormula(formula, offset.row, offset.col);
    }
    std::string columnIndexToLetters(int index) {
        std::string column;
        while (index > 0) {
            index--;
            column = static_cast<char>('A' + (index % 26)) + column;
            index /= 26;
        }
        return column;
    }

// Helper function to convert column letter(s) to column index
    int columnLettersToIndex(const std::string& letters) {
        int index = 0;
        for (char c : letters) {
            index = index * 26 + (c - 'A' + 1);
        }
        return index;
    }

    std::string adjustCellReference(const std::string& cellRef, int rowOffset, int colOffset) {
        std::string adjusted;
        std::string cell;
        bool isAbsoluteRow, isAbsoluteCol;

        for (char ch : cellRef) {
            if (isalpha(ch) || isdigit(ch) || ch == '$') {
                cell += ch;
                if (ch == '$') {
                    if (isdigit(cell.back())) {
                        isAbsoluteRow = true;
                    } else {
                        isAbsoluteCol = true;
                    }
                }
            } else {
                if (!cell.empty()) {
                    // Parse the cell and adjust if necessary
                    std::string colPart;
                    std::string rowPart;
                    for (char c : cell) {
                        if (isalpha(c)) {
                            if (!isAbsoluteCol) {
                                colPart += c;
                            }
                        } else if (isdigit(c)) {
                            if (!isAbsoluteRow) {
                                rowPart += c;
                            }
                        }
                    }
                    if (!colPart.empty() && !isAbsoluteCol) {
                        int col = columnLettersToIndex(colPart) - 1; // Convert to 0-based index
                        col += colOffset;
                        adjusted += columnIndexToLetters(col + 1); // Convert back to 1-based index
                    } else {
                        adjusted += colPart;
                    }
                    if (!rowPart.empty() && !isAbsoluteRow) {
                        int row = stoi(rowPart) - 1; // Convert to 0-based index
                        row += rowOffset;
                        adjusted += to_string(row + 1); // Convert back to 1-based index
                    } else {
                        adjusted += rowPart;
                    }
                    cell.clear();
                    isAbsoluteRow = isAbsoluteCol = false;
                }
                adjusted += ch;
            }
        }
        return adjusted;
    }

    std::string adjustFormula(const std::string& formula, int rowOffset, int colOffset) {
        std::istringstream iss(formula);
        std::string token, adjustedFormula;
        // Use regex to identify cell references in the formula
        std::regex cellRefRegex(R"((\$?[A-Z]+\$?\d+))");
        std::smatch match;

        while (iss >> token) {
            // Check each token to see if it matches a cell reference pattern
            if (std::regex_match(token, match, cellRefRegex)) {
                // Adjust the cell reference
                std::string adjustedRef = adjustCellReference(match[0], rowOffset, colOffset);
                adjustedFormula += adjustedRef + " ";
            } else {
                // If it's not a cell reference, keep the token unchanged
                adjustedFormula += token + " ";
            }
        }

        // Trim the trailing space and return the adjusted formula
        if (!adjustedFormula.empty()) {
            adjustedFormula.pop_back();
        }
        return adjustedFormula;
    }
    std::string escape(const std::string &input) const {
        std::string output;
        for (char c : input) {
            if (c == ',' || c == '\\') output += '\\';
            output += c;
        }
        return output;
    }

    std::string unescape(const std::string &input) const {
        std::string output;
        bool escapeNext = false;
        for (char c : input) {
            if (escapeNext) {
                output += c;
                escapeNext = false;
            } else if (c == '\\') {
                escapeNext = true;
            } else {
                output += c;
            }
        }
        return output;
    }
    void updateDependentValues(const CPos& pos) {
        // Find all cells that depend on the cell that just changed.
        // For simplicity, let's assume we have a method that can retrieve this information.
        std::set<CPos> dependents = getDependentCells(pos);

        for (const CPos& dep : dependents) {
            // Invalidate the old value
            values.erase(dep);
            // Force re-evaluation of the dependent cell's value.
            getValue(dep);
        }
    }
    void addDependency(const CPos& cell, const CPos& dependent) {
        dependents[cell].insert(dependent);
    }

    set<CPos> getDependentCells(const CPos& cell) {
        return dependents[cell];
    }

    // This function must be called whenever a cell value changes.
    void updateDependentCells(const CPos& cell) {
        for (const auto& dependent : getDependentCells(cell)) {
            // Invalidate the cached value of the dependent cell.
            values.erase(dependent);
            // Force re-evaluation of this cell's value.
            getValue(dependent);
        }
    }
    map<CPos, set<CPos>> dependents;
    map<CPos, CValue> cells;
    map<CPos, string> expressions;  // Store the raw expressions
    map<CPos, CValue> values;       // Store evaluated values
    bool isValueCurrent(const CPos& pos) {
        // Check if the value of a cell is current or needs re-evaluation
        return values.find(pos) != values.end();
    }
private:
    // todo
};

CValue SimpleExpressionEvaluator::evaluateNode(const std::shared_ptr<ExpressionNode> &node) {

    switch (node->type) {
        case ExpressionNode::Number:
            return get<double>(node->value);
        case ExpressionNode::Reference: {
            CPos pos = get<CPos>(node->value);
            return spreadsheet.getValue(pos);
        }
        case ExpressionNode::Operator: {
            auto left = evaluateNode(node->children.at(0));
            auto right = evaluateNode(node->children.at(1));
            return applyOperator(get<string>(node->value), left, right);
        }
        default:
            throw std::runtime_error("Unsupported node type in evaluation");
    }

}

#ifndef __PROGTEST__

bool                                   valueMatch                              ( const CValue                        & r,
                                                                                 const CValue                        & s )

{
    if ( r . index () != s . index () )
        return false;
    if ( r . index () == 0 )
        return true;
    if ( r . index () == 2 )
        return std::get<std::string> ( r ) == std::get<std::string> ( s );
    if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
        return true;
    if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
        return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
               || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
    return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}
int main ()
{
    CSpreadsheet x0, x1,x2;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;
/*    // Set initial values clearly
    CSpreadsheet testSheet;
    testSheet.setCell(CPos("A1"), "10");
    testSheet.setCell(CPos("A2"), "20");
    testSheet.setCell(CPos("A3"), "30");
    SimpleExpressionEvaluator eval(testSheet.cells);
    eval.valReference("A1");
    eval.valReference("A2");
    eval.valReference("A3");
    eval.opMul(); // Should multiply A2 and A3
    eval.opAdd(); // Should add A1 and the result of A2*A3
    double result = eval.getResult();
    cerr << "Manual evaluation result: " << result << endl;
 //   cout << "Cell A7 contains: " << result << endl;*/
    assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A2" ), "20.5" ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "3e1" ) );
    assert ( x0 . setCell ( CPos ( "A4" ), "=40" ) );
    assert ( x0 . setCell ( CPos ( "A5" ), "=5e+1" ) );
   assert ( x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
     assert ( x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );
        assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
       assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ) );
       assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ) );
       assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ) );
       assert ( valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ) );
      assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
      // assert ( valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
      assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ) );
      assert ( valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ) );
      assert ( x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ) );
       assert ( x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
     assert ( x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ) );
      assert ( x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ) );
        assert ( x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ) );
       assert ( x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ) );

      assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ) );
      assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -110.25 ) ) );
     assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ) );
     assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ) );
         assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ) );
        assert ( x0 . setCell ( CPos ( "A1" ), "12" ) );
        cout<<get<double>(x0 . getValue ( CPos ( "B1" )))<<"hui\n";
        assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ) );
            assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ) );
           x1 = x0;
           assert ( x0 . setCell ( CPos ( "A2" ), "100" ) );
           assert ( x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ) );
           oss . clear ();
           oss . str ( "" );
           assert ( x0 . save ( oss ) );
           data = oss . str ();
           iss . clear ();
           iss . str ( data );
           assert ( x1 . load ( iss ) );
           x1=x0;
           cout<<get<double>(x1 . getValue ( CPos ( "B1" ) ))<<"hui\n";
           assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
           assert ( x0 . setCell ( CPos ( "A3" ), "4e1" ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
           assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
           oss . clear ();
           oss . str ( "" );
           assert ( x0 . save ( oss ) );
           data = oss . str ();
           for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
               data[i] ^=0x5a;
           iss . clear ();
           iss . str ( data );
           assert ( ! x1 . load ( iss ) );
           assert ( x0 . setCell ( CPos ( "D0" ), "10" ) );
           assert ( x0 . setCell ( CPos ( "D1" ), "20" ) );
           assert ( x0 . setCell ( CPos ( "D2" ), "30" ) );
           assert ( x0 . setCell ( CPos ( "D3" ), "40" ) );
           assert ( x0 . setCell ( CPos ( "D4" ), "50" ) );
           assert ( x0 . setCell ( CPos ( "E0" ), "60" ) );
           assert ( x0 . setCell ( CPos ( "E1" ), "70" ) );
           assert ( x0 . setCell ( CPos ( "E2" ), "80" ) );
           assert ( x0 . setCell ( CPos ( "E3" ), "90" ) );
           assert ( x0 . setCell ( CPos ( "E4" ), "100" ) );
           assert ( x0 . setCell ( CPos ( "F10" ), "=D0+5" ) );
           assert ( x0 . setCell ( CPos ( "F11" ), "=$D0+5" ) );
           assert ( x0 . setCell ( CPos ( "F12" ), "=D$0+5" ) );
           assert ( x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ) );

           x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 1, 4 );
         assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
       x0 . getValue ( CPos ( "G11" ));
      //     assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
            /* assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
           x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 2, 4 );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ) );
           assert ( x0 . setCell ( CPos ( "F0" ), "-27" ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
           x0 . copyRect ( CPos ( "H12" ), CPos ( "H13" ), 1, 2 );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ) );
           assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );*/
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
