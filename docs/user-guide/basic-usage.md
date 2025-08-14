# Basic Usage

This guide covers the fundamental usage patterns of the Modern C++ Calculator library. You'll learn how to perform basic calculations, understand operator precedence, and work with different numeric types.

## Simple Expression Evaluation

The easiest way to use the calculator is through the free functions:

```cpp
#include "calculator.hpp"
#include <iostream>

int main() {
    // Basic arithmetic
    double result = calculator::eval("2 + 3 * 4");
    std::cout << "2 + 3 * 4 = " << result << std::endl;  // Output: 14
    
    return 0;
}
```

## Arithmetic Operations

### Basic Operations

The calculator supports all standard arithmetic operations:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_arithmetic() {
    std::cout << "Addition: " << calculator::eval("10 + 5") << std::endl;        // 15
    std::cout << "Subtraction: " << calculator::eval("10 - 5") << std::endl;     // 5
    std::cout << "Multiplication: " << calculator::eval("10 * 5") << std::endl;  // 50
    std::cout << "Division: " << calculator::eval("10 / 5") << std::endl;        // 2
    std::cout << "Modulo: " << calculator::eval<int>("10 % 3") << std::endl;     // 1
}
```

### Exponentiation

Power operations using the `**` operator:

```cpp
std::cout << "Power: " << calculator::eval("2 ** 3") << std::endl;           // 8
std::cout << "Power: " << calculator::eval("5 ** 2") << std::endl;           // 25
std::cout << "Fractional: " << calculator::eval("8 ** (1.0/3.0)") << std::endl; // ~2 (cube root)
```

### Scientific Notation

Support for scientific notation with `e` or `E`:

```cpp
std::cout << "Scientific: " << calculator::eval("1.5e2") << std::endl;       // 150
std::cout << "Scientific: " << calculator::eval("2.5E-3") << std::endl;      // 0.0025
std::cout << "Scientific: " << calculator::eval("6.02e23") << std::endl;     // 6.02e+23
```

## Unary Operators

### Unary Plus and Minus

```cpp
std::cout << "Unary plus: " << calculator::eval("+5") << std::endl;          // 5
std::cout << "Unary minus: " << calculator::eval("-5") << std::endl;         // -5
std::cout << "Double negative: " << calculator::eval("--5") << std::endl;    // 5
std::cout << "Complex: " << calculator::eval("-(3 + 4)") << std::endl;       // -7
```

### Bitwise NOT (Integer Only)

```cpp
// Bitwise NOT is only available for integer types
std::cout << "Bitwise NOT: " << calculator::eval<int>("~5") << std::endl;    // -6
std::cout << "Bitwise NOT: " << calculator::eval<int>("~0") << std::endl;    // -1
```

## Bitwise Operations (Integer Only)

Bitwise operations are only supported for integer types:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_bitwise() {
    // Bitwise operations require integer type
    std::cout << "Bitwise OR: " << calculator::eval<int>("5 | 3") << std::endl;    // 7
    std::cout << "Bitwise XOR: " << calculator::eval<int>("5 ^ 3") << std::endl;   // 6
    std::cout << "Bitwise AND: " << calculator::eval<int>("5 & 3") << std::endl;   // 1
    std::cout << "Left shift: " << calculator::eval<int>("4 << 1") << std::endl;   // 8
    std::cout << "Right shift: " << calculator::eval<int>("8 >> 1") << std::endl;  // 4
}
```

!!! warning "Floating-Point Limitation"
    Bitwise operations (`|`, `^`, `&`, `<<`, `>>`, `~`) are only supported for integer types. Using them with `float` or `double` will result in an error.

## Parentheses and Grouping

Use parentheses to control the order of operations:

```cpp
std::cout << "Without parentheses: " << calculator::eval("2 + 3 * 4") << std::endl;    // 14
std::cout << "With parentheses: " << calculator::eval("(2 + 3) * 4") << std::endl;     // 20

std::cout << "Complex grouping: " << calculator::eval("((2 + 3) * 4) / 5") << std::endl; // 4

// Nested parentheses
std::cout << "Nested: " << calculator::eval("2 * (3 + (4 * 5))") << std::endl;         // 46
```

## Numeric Types

### Working with Different Types

The calculator supports three numeric types with different behaviors:

#### Integer Type

```cpp
// Integer calculations
int int_result1 = calculator::eval<int>("10 / 3");     // 3 (integer division)
int int_result2 = calculator::eval<int>("7 % 3");      // 1 (modulo)
int int_result3 = calculator::eval<int>("5 & 3");      // 1 (bitwise operations)

std::cout << "Integer division: " << int_result1 << std::endl;
std::cout << "Modulo: " << int_result2 << std::endl;
std::cout << "Bitwise AND: " << int_result3 << std::endl;
```

#### Float Type

```cpp
// Single-precision floating-point
float float_result1 = calculator::eval<float>("10.0 / 3.0");   // 3.33333...
float float_result2 = calculator::eval<float>("3.14159 * 2");  // 6.28318...

std::cout << "Float division: " << float_result1 << std::endl;
std::cout << "Float multiplication: " << float_result2 << std::endl;
```

#### Double Type (Default)

```cpp
// Double-precision floating-point (default)
double double_result1 = calculator::eval("10.0 / 3.0");        // 3.33333...
double double_result2 = calculator::eval("1.0 / 3.0");         // 0.33333...

// Using the default eval function (returns double)
double default_result = calculator::eval("2.5 * 4.2");         // 10.5

std::cout << "Double division: " << double_result1 << std::endl;
std::cout << "Default result: " << default_result << std::endl;
```

### Type Conversion and Precision

```cpp
#include "calculator.hpp"
#include <iostream>
#include <iomanip>

void demonstrate_precision() {
    // Same calculation with different types
    std::string expr = "1.0 / 3.0";
    
    float float_result = calculator::eval<float>(expr);
    double double_result = calculator::eval<double>(expr);
    
    std::cout << std::setprecision(10);
    std::cout << "Float result:  " << float_result << std::endl;   // ~0.3333333433
    std::cout << "Double result: " << double_result << std::endl;  // ~0.3333333333
}
```

## Operator Precedence

The calculator follows standard mathematical operator precedence:

| Precedence | Operators | Associativity | Example |
|------------|-----------|---------------|---------|
| 1 (Highest) | `~` (unary), `+` (unary), `-` (unary) | Right | `~5`, `+3`, `-7` |
| 2 | `**` | Right | `2 ** 3 ** 2` = `2 ** (3 ** 2)` = 512 |
| 3 | `*`, `/`, `%` | Left | `6 / 2 * 3` = `(6 / 2) * 3` = 9 |
| 4 | `+`, `-` | Left | `5 - 3 + 2` = `(5 - 3) + 2` = 4 |
| 5 | `<<`, `>>` | Left | `8 >> 1 << 2` = `(8 >> 1) << 2` = 16 |
| 6 | `&` | Left | `5 & 3 & 1` = `(5 & 3) & 1` = 1 |
| 7 | `^` | Left | `5 ^ 3 ^ 1` = `(5 ^ 3) ^ 1` = 7 |
| 8 (Lowest) | `\|` | Left | `5 \| 3 \| 1` = `(5 \| 3) \| 1` = 7 |

### Precedence Examples

```cpp
// Demonstrates operator precedence
std::cout << "2 + 3 * 4: " << calculator::eval("2 + 3 * 4") << std::endl;           // 14 (not 20)
std::cout << "2 ** 3 ** 2: " << calculator::eval("2 ** 3 ** 2") << std::endl;       // 512 (not 64)
std::cout << "10 - 5 + 2: " << calculator::eval("10 - 5 + 2") << std::endl;         // 7
std::cout << "8 / 2 * 3: " << calculator::eval("8 / 2 * 3") << std::endl;           // 12

// Bitwise precedence (integer only)
std::cout << "5 | 3 & 1: " << calculator::eval<int>("5 | 3 & 1") << std::endl;      // 5
std::cout << "8 >> 1 + 1: " << calculator::eval<int>("8 >> 1 + 1") << std::endl;    // 4
```

## Common Patterns

### Mathematical Formulas

```cpp
// Quadratic formula: (-b ± √(b² - 4ac)) / 2a
// For a=1, b=-5, c=6
double discriminant = calculator::eval("(-5) ** 2 - 4 * 1 * 6");  // 1
double root1 = calculator::eval("(5 + 1) / (2 * 1)");             // 3
double root2 = calculator::eval("(5 - 1) / (2 * 1)");             // 2

// Area of a circle: πr²
double radius = 5.0;
std::string circle_area_expr = "3.14159 * " + std::to_string(radius) + " ** 2";
double area = calculator::eval(circle_area_expr);                  // ~78.54
```

### Unit Conversions

```cpp
// Temperature conversions
double celsius = 25.0;
std::string fahrenheit_expr = std::to_string(celsius) + " * 9.0 / 5.0 + 32.0";
double fahrenheit = calculator::eval(fahrenheit_expr);             // 77

// Distance conversions
double miles = 10.0;
std::string km_expr = std::to_string(miles) + " * 1.60934";
double kilometers = calculator::eval(km_expr);                     // 16.0934
```

### Financial Calculations

```cpp
// Simple interest: P * r * t
double principal = 1000.0;
double rate = 0.05;
double time = 2.0;

std::string interest_expr = std::to_string(principal) + " * " + 
                           std::to_string(rate) + " * " + 
                           std::to_string(time);
double interest = calculator::eval(interest_expr);                 // 100

// Compound interest: P * (1 + r)^t
std::string compound_expr = std::to_string(principal) + " * (1 + " + 
                           std::to_string(rate) + ") ** " + 
                           std::to_string(time);
double compound_amount = calculator::eval(compound_expr);          // 1102.5
```

## Error Handling

Always wrap calculations in try-catch blocks for robust applications:

```cpp
#include "calculator.hpp"
#include <iostream>

double safe_calculate(const std::string& expression) {
    try {
        return calculator::eval(expression);
    } catch (const calculator::error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 0.0;  // or throw, or return a default value
    }
}

int main() {
    // These will work
    std::cout << "Good: " << safe_calculate("2 + 3") << std::endl;
    
    // These will be handled gracefully
    std::cout << "Bad: " << safe_calculate("2 + + 3") << std::endl;      // Syntax error
    std::cout << "Bad: " << safe_calculate("5 / 0") << std::endl;        // Division by zero
    
    return 0;
}
```

## Best Practices

1. **Use appropriate numeric types** for your calculations
2. **Always handle exceptions** when dealing with user input
3. **Use parentheses** to make complex expressions clear
4. **Validate input** before evaluation when possible
5. **Be aware of operator precedence** to avoid unexpected results
6. **Use scientific notation** for very large or very small numbers
7. **Consider precision requirements** when choosing between float and double

## Next Steps

- Learn about [Variables & Functions](variables-functions.md) for dynamic expressions
- Explore [Advanced Features](advanced-features.md) for complex calculations
- Review [Error Handling](error-handling.md) for robust applications
- Check out [Examples](../examples/basic-examples.md) for practical use cases
