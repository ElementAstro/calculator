# Modern C++ Calculator

This is a modern C++ implementation of a calculator that can evaluate mathematical expressions. It supports various arithmetic operations, bitwise operations, and follows the standard order of operations.

> [!IMPORTANT]
> This library is still under development and may contain bugs.

## Features

- Supports basic arithmetic operations: addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`), and modulo (`%`)
- Supports bitwise operations: OR (`|`), XOR (`^`), AND (`&`), left shift (`<<`), and right shift (`>>`)
- Supports exponentiation: power (`**`) and scientific notation (`e` or `E`)
- Handles unary operators: unary plus (`+`), unary minus (`-`), and bitwise NOT (`~`)
- Follows the standard order of operations and operator precedence
- Supports parentheses for grouping subexpressions
- Provides error handling and informative error messages
- Supports different numeric types: `int`, `float`, and `double`
- Lightweight and header-only implementation

## Usage

1. Include the `calculator.hpp` header file in your C++ project.

2. Use the `calculator::eval` function to evaluate mathematical expressions:

```cpp
#include "calculator.hpp"
#include <iostream>

int main() {
    std::string expr = "12.34 + 56.78";
    double result = calculator::eval(expr);
    std::cout << "Result: " << result << std::endl;

    try {
        std::string expr2 = "12.34 + 56.78";
        int result2 = calculator::eval<int>(expr2);
        std::cout << "Result: " << result2 << std::endl;
    } catch (const calculator::error& e) {
        std::cerr << e.what() << std::endl;
    }
  
}
```

## Supported Operations

The calculator supports the following operations:

- Arithmetic Operations:
  - Addition: `+`
  - Subtraction: `-`
  - Multiplication: `*`
  - Division: `/`
  - Modulo: `%`
- Bitwise Operations:
  - Bitwise OR: `|`
  - Bitwise XOR: `^`
  - Bitwise AND: `&`
  - Left Shift: `<<`
  - Right Shift: `>>`
- Exponentiation:
  - Power: `**`
  - Scientific Notation: `e` or `E`
- Unary Operators:
  - Unary Plus: `+`
  - Unary Minus: `-`
  - Bitwise NOT: `~` (only supported for `int` type)

## Error Handling

The calculator provides error handling and throws exceptions of type `calculator::error` in case of syntax errors or invalid expressions. The error message includes information about the unexpected token and its position in the expression.

## Numeric Types

The calculator supports different numeric types:

- `int`: Integers
- `float`: Single-precision floating-point numbers
- `double`: Double-precision floating-point numbers

By default, the `calculator::eval` function uses `double` as the result type. To use a different numeric type, you can explicitly specify the type as a template argument:

```cpp
int result = calculator::eval<int>("1 + 2");
float result = calculator::eval<float>("3.14 * 2");
```

Note that bitwise operations and modulo are only supported for integer types.

## Limitations

- The calculator assumes that the input expression is well-formed and does not contain any invalid characters or unsupported operations.
- The calculator does not support variables or functions.
- The maximum length of numbers is limited by the precision of the chosen numeric type.

## License

This calculator implementation is released under the [MIT License](LICENSE).

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.
