# FITExcel
# Spreadsheet Backend Simulator

## Overview

This project involves developing a backend simulator for a spreadsheet processor. The goal is to create classes that allow manipulation of spreadsheet cells, support expressions for cell values, detect cyclic dependencies, and handle saving/loading spreadsheets to and from files. The implementation will be evaluated based on functionality, class design, use of polymorphism, and proficiency with Git versioning.

## Features

- **Cell Manipulation**: Set cell values, compute values, and copy cells.
- **Expressions**: Define cell values using expressions with support for various functions and operators.
- **Cyclic Dependency Detection**: Automatically detect and handle cyclic dependencies among cells.
- **File Operations**: Save and load spreadsheets to and from files.
- **Expression Parsing**: Utilize a provided parser to handle expression syntax.

## Key Classes

### `CSpreadsheet`
- **Description**: Represents the spreadsheet processor. Each instance corresponds to a single sheet.
- **Responsibilities**:
  - Manage cell contents and values.
  - Handle cell copying and range operations.
  - Save and load spreadsheet data.

### `CPos`
- **Description**: Identifies a cell within the spreadsheet using standard notation (e.g., `A7`, `PROG7250`).
- **Responsibilities**:
  - Parse and store column and row identifiers.
  - Validate cell identifiers and throw exceptions for invalid formats.

### `CExpressionBuilder`
- **Description**: An abstract class used by the expression parser to build expression trees (AST).
- **Responsibilities**:
  - Implement methods to handle various operators and operands during parsing.
  - Construct an efficient representation of expressions for fast computation.

### `CValue`
- **Description**: Represents the value of a cell, which can be undefined, a decimal number, or a string.
- **Implementation**: Specialized using `std::variant`.

## Expression Support

Cells can contain:
- **Numeric Literals**: Integers or decimal numbers (e.g., `15`, `2.54`, `1e+8`).
- **String Literals**: Sequences of characters enclosed in double quotes (e.g., `"Hello World"`).
- **Cell References**: References to other cells, supporting absolute and relative references (e.g., `A5`, `$A$5`).
- **Cell Ranges**: Rectangular ranges of cells (e.g., `A5:X17`).
- **Functions**: Supported functions include `sum`, `count`, `min`, `max`, `countval`, and `if`.
- **Operators**: Arithmetic (`+`, `-`, `*`, `/`, `^`), unary minus, and relational operators (`<`, `<=`, `>`, `>=`, `=`, `<>`).

Expressions follow standard operator precedence and associativity rules.

## Implementation Guidelines

1. **Start with Mandatory Features**:
   - Implement `setCell`, `getValue`, and `copyRect` methods.
   - Use the provided parser for handling expressions.

2. **Expand to Optional Features**:
   - Add support for cell ranges and functions.
   - Implement cyclic dependency detection for enhanced functionality.

3. **Design Considerations**:
   - Use efficient data structures (e.g., ASTs) for expression evaluation.
   - Ensure robust class design with proper use of polymorphism.
   - Maintain clean and modular code across multiple source files.

4. **Version Control**:
   - Use Git for versioning your code.
   - Commit changes regularly with clear commit messages.

## Testing

The implementation will be evaluated through a series of tests:

- **Basic Functionality**: Verify setting and getting cell values, and copying cells.
- **Expression Evaluation**: Test expressions without cyclic dependencies and without functions.
- **File Operations**: Ensure spreadsheets can be saved and loaded correctly, including handling corrupted files.
- **Performance**: Optimize computation speed using ASTs to pass speed tests.
- **Cyclic Dependencies**: Detect and handle cyclic dependencies without causing infinite loops.
- **Function Support**: Validate correct implementation of supported functions.

## Submission Instructions

- **Code Structure**: Organize your code into multiple `.cpp` and `.h` files for maintainability.
- **Single File Submission**: If required, consolidate your source files into a single file by excluding `#include` directives.
- **Versioning**: Use GitLab (faculty GitLab server recommended) to manage and archive your source code.
- **Submission Format**: Follow the provided guidelines for submitting your code, ensuring all necessary classes and functions are included.

## Optional Bonus

- **Custom Parser**: Implement your own expression parser to pass the bonus test. Note that this is optional and only offers a small additional score.

**Caution**: If you choose to implement a custom parser, ensure it does not conflict with the provided `parseExpression()` function to avoid compilation errors.

## Additional Notes

- Focus on designing a robust and efficient class structure.
- Prioritize mandatory features before tackling optional enhancements.
- Regularly test your implementation to ensure correctness and performance.
- Utilize the provided parser and library to streamline expression handling.

---

Happy Coding! 🚀
