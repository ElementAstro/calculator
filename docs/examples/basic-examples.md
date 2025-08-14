# Basic Examples

This page provides fundamental examples to get you started with the Modern C++ Calculator library. These examples cover the most common use cases and demonstrate core functionality.

## Simple Arithmetic

### Basic Operations

```cpp
#include "calculator.hpp"
#include <iostream>

int main() {
    // Basic arithmetic operations
    std::cout << "Addition: " << calculator::eval("10 + 5") << std::endl;        // 15
    std::cout << "Subtraction: " << calculator::eval("10 - 5") << std::endl;     // 5
    std::cout << "Multiplication: " << calculator::eval("10 * 5") << std::endl;  // 50
    std::cout << "Division: " << calculator::eval("10 / 5") << std::endl;        // 2
    
    // Integer-specific operations
    std::cout << "Modulo: " << calculator::eval<int>("10 % 3") << std::endl;     // 1
    std::cout << "Power: " << calculator::eval("2 ** 3") << std::endl;           // 8
    
    return 0;
}
```

### Order of Operations

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_precedence() {
    // Demonstrates operator precedence
    std::cout << "2 + 3 * 4 = " << calculator::eval("2 + 3 * 4") << std::endl;           // 14
    std::cout << "(2 + 3) * 4 = " << calculator::eval("(2 + 3) * 4") << std::endl;       // 20
    std::cout << "2 ** 3 ** 2 = " << calculator::eval("2 ** 3 ** 2") << std::endl;       // 512
    std::cout << "(2 ** 3) ** 2 = " << calculator::eval("(2 ** 3) ** 2") << std::endl;   // 64
    
    // Complex expressions
    std::cout << "Complex: " << calculator::eval("(10 + 5) * 2 / 3 - 1") << std::endl;   // 9
}
```

## Working with Different Types

### Integer Calculations

```cpp
#include "calculator.hpp"
#include <iostream>

void integer_examples() {
    // Integer arithmetic
    int result1 = calculator::eval<int>("15 + 25");        // 40
    int result2 = calculator::eval<int>("20 / 3");         // 6 (integer division)
    int result3 = calculator::eval<int>("17 % 5");         // 2
    
    std::cout << "Integer addition: " << result1 << std::endl;
    std::cout << "Integer division: " << result2 << std::endl;
    std::cout << "Modulo operation: " << result3 << std::endl;
    
    // Bitwise operations (integer only)
    int bitwise1 = calculator::eval<int>("5 | 3");         // 7 (bitwise OR)
    int bitwise2 = calculator::eval<int>("5 & 3");         // 1 (bitwise AND)
    int bitwise3 = calculator::eval<int>("5 ^ 3");         // 6 (bitwise XOR)
    int bitwise4 = calculator::eval<int>("8 >> 1");        // 4 (right shift)
    int bitwise5 = calculator::eval<int>("4 << 1");        // 8 (left shift)
    int bitwise6 = calculator::eval<int>("~5");            // -6 (bitwise NOT)
    
    std::cout << "Bitwise OR: " << bitwise1 << std::endl;
    std::cout << "Bitwise AND: " << bitwise2 << std::endl;
    std::cout << "Bitwise XOR: " << bitwise3 << std::endl;
    std::cout << "Right shift: " << bitwise4 << std::endl;
    std::cout << "Left shift: " << bitwise5 << std::endl;
    std::cout << "Bitwise NOT: " << bitwise6 << std::endl;
}
```

### Floating-Point Calculations

```cpp
#include "calculator.hpp"
#include <iostream>
#include <iomanip>

void floating_point_examples() {
    // Double precision (default)
    double result1 = calculator::eval("10.5 + 3.7");       // 14.2
    double result2 = calculator::eval("22.0 / 7.0");       // 3.142857...
    double result3 = calculator::eval("2.5 ** 2");         // 6.25
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Double addition: " << result1 << std::endl;
    std::cout << "Double division: " << result2 << std::endl;
    std::cout << "Double power: " << result3 << std::endl;
    
    // Single precision
    float float_result1 = calculator::eval<float>("3.14159 * 2");  // 6.28318
    float float_result2 = calculator::eval<float>("1.0 / 3.0");    // 0.333333
    
    std::cout << "Float multiplication: " << float_result1 << std::endl;
    std::cout << "Float division: " << float_result2 << std::endl;
    
    // Scientific notation
    double scientific1 = calculator::eval("1.5e2");        // 150
    double scientific2 = calculator::eval("2.5E-3");       // 0.0025
    double scientific3 = calculator::eval("6.02e23");      // 6.02e+23
    
    std::cout << "Scientific 1: " << scientific1 << std::endl;
    std::cout << "Scientific 2: " << scientific2 << std::endl;
    std::cout << "Scientific 3: " << scientific3 << std::endl;
}
```

## Using Variables

### Simple Variable Usage

```cpp
#include "calculator.hpp"
#include <iostream>

void variable_examples() {
    calculator::ExpressionParser<double> parser;
    
    // Set variables
    parser.set("x", 5.0);
    parser.set("y", 10.0);
    parser.set("pi", 3.14159);
    
    // Use variables in expressions
    double result1 = parser.eval("x + y");                 // 15
    double result2 = parser.eval("x * y");                 // 50
    double result3 = parser.eval("y / x");                 // 2
    double result4 = parser.eval("pi * x ** 2");           // ~78.54
    
    std::cout << "x + y = " << result1 << std::endl;
    std::cout << "x * y = " << result2 << std::endl;
    std::cout << "y / x = " << result3 << std::endl;
    std::cout << "pi * x^2 = " << result4 << std::endl;
    
    // Update variables
    parser.set("x", 3.0);
    double result5 = parser.eval("x + y");                 // 13
    std::cout << "Updated x + y = " << result5 << std::endl;
}
```

### Mathematical Constants

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

void constants_example() {
    calculator::ExpressionParser<double> parser;
    
    // Set mathematical constants
    parser.set("pi", M_PI);
    parser.set("e", M_E);
    parser.set("sqrt2", M_SQRT2);
    parser.set("ln2", M_LN2);
    
    // Use constants in calculations
    double circle_area = parser.eval("pi * 5 ** 2");       // Area of circle with radius 5
    double natural_log = parser.eval("ln2 * 3");           // ln(8) ≈ ln(2) * 3
    double diagonal = parser.eval("sqrt2 * 10");           // Diagonal of 10x10 square
    
    std::cout << "Circle area (r=5): " << circle_area << std::endl;
    std::cout << "ln(8): " << natural_log << std::endl;
    std::cout << "Diagonal of 10x10 square: " << diagonal << std::endl;
}
```

## Using Functions

### Basic Function Usage

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

void function_examples() {
    calculator::ExpressionParser<double> parser;
    
    // Define custom functions
    parser.set("square", [](double x) { return x * x; });
    parser.set("cube", [](double x) { return x * x * x; });
    parser.set("double_val", [](double x) { return x * 2; });
    
    // Use functions in expressions
    double result1 = parser.eval("square(5)");             // 25
    double result2 = parser.eval("cube(3)");               // 27
    double result3 = parser.eval("double_val(7)");         // 14
    double result4 = parser.eval("square(3) + cube(2)");   // 9 + 8 = 17
    
    std::cout << "square(5) = " << result1 << std::endl;
    std::cout << "cube(3) = " << result2 << std::endl;
    std::cout << "double_val(7) = " << result3 << std::endl;
    std::cout << "square(3) + cube(2) = " << result4 << std::endl;
}
```

### Mathematical Functions

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

void math_functions_example() {
    calculator::ExpressionParser<double> parser;
    
    // Set up mathematical constants
    parser.set("pi", M_PI);
    parser.set("e", M_E);
    
    // Define mathematical functions
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("cos", [](double x) { return std::cos(x); });
    parser.set("tan", [](double x) { return std::tan(x); });
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    parser.set("log", [](double x) { return std::log(x); });
    parser.set("exp", [](double x) { return std::exp(x); });
    parser.set("abs", [](double x) { return std::abs(x); });
    
    // Use mathematical functions
    double result1 = parser.eval("sin(pi/2)");             // 1
    double result2 = parser.eval("cos(0)");                // 1
    double result3 = parser.eval("sqrt(16)");              // 4
    double result4 = parser.eval("log(e)");                // 1
    double result5 = parser.eval("exp(1)");                // e ≈ 2.718
    double result6 = parser.eval("abs(-5)");               // 5
    
    std::cout << "sin(π/2) = " << result1 << std::endl;
    std::cout << "cos(0) = " << result2 << std::endl;
    std::cout << "sqrt(16) = " << result3 << std::endl;
    std::cout << "log(e) = " << result4 << std::endl;
    std::cout << "exp(1) = " << result5 << std::endl;
    std::cout << "abs(-5) = " << result6 << std::endl;
    
    // Complex expressions with functions
    double complex1 = parser.eval("sin(pi/4) + cos(pi/4)"); // √2 ≈ 1.414
    double complex2 = parser.eval("sqrt(sin(pi/2) ** 2 + cos(pi/2) ** 2)"); // 1
    
    std::cout << "sin(π/4) + cos(π/4) = " << complex1 << std::endl;
    std::cout << "sqrt(sin²(π/2) + cos²(π/2)) = " << complex2 << std::endl;
}
```

## Error Handling

### Basic Error Handling

```cpp
#include "calculator.hpp"
#include <iostream>

void error_handling_examples() {
    std::cout << "=== Error Handling Examples ===" << std::endl;
    
    // Syntax errors
    try {
        calculator::eval("2 + + 3");
    } catch (const calculator::error& e) {
        std::cout << "Syntax error: " << e.what() << std::endl;
    }
    
    // Division by zero
    try {
        calculator::eval("5 / 0");
    } catch (const calculator::error& e) {
        std::cout << "Division by zero: " << e.what() << std::endl;
    }
    
    // Undefined variable
    try {
        calculator::ExpressionParser<double> parser;
        parser.eval("x + 5");
    } catch (const calculator::error& e) {
        std::cout << "Undefined variable: " << e.what() << std::endl;
    }
    
    // Type error
    try {
        calculator::eval<int>("3.14");
    } catch (const calculator::error& e) {
        std::cout << "Type error: " << e.what() << std::endl;
    }
}
```

### Safe Evaluation Function

```cpp
#include "calculator.hpp"
#include <iostream>
#include <optional>

std::optional<double> safe_eval(const std::string& expression) {
    try {
        return calculator::eval(expression);
    } catch (const calculator::error& e) {
        std::cerr << "Error evaluating '" << expression << "': " << e.what() << std::endl;
        return std::nullopt;
    }
}

void safe_evaluation_example() {
    std::vector<std::string> expressions = {
        "2 + 3",        // Valid
        "10 / 2",       // Valid
        "5 / 0",        // Error: Division by zero
        "2 + + 3",      // Error: Syntax error
        "sqrt(16)"      // Error: Undefined function
    };
    
    for (const auto& expr : expressions) {
        auto result = safe_eval(expr);
        if (result) {
            std::cout << expr << " = " << *result << std::endl;
        } else {
            std::cout << expr << " = ERROR" << std::endl;
        }
    }
}
```

## Practical Applications

### Unit Conversion

```cpp
#include "calculator.hpp"
#include <iostream>

void unit_conversion_examples() {
    calculator::ExpressionParser<double> converter;
    
    // Temperature conversions
    double celsius = 25.0;
    converter.set("c", celsius);
    double fahrenheit = converter.eval("c * 9 / 5 + 32");      // 77°F
    double kelvin = converter.eval("c + 273.15");              // 298.15K
    
    std::cout << celsius << "°C = " << fahrenheit << "°F" << std::endl;
    std::cout << celsius << "°C = " << kelvin << "K" << std::endl;
    
    // Distance conversions
    double miles = 10.0;
    converter.set("miles", miles);
    double kilometers = converter.eval("miles * 1.60934");     // 16.0934 km
    double feet = converter.eval("miles * 5280");              // 52800 ft
    
    std::cout << miles << " miles = " << kilometers << " km" << std::endl;
    std::cout << miles << " miles = " << feet << " feet" << std::endl;
    
    // Area conversions
    double acres = 5.0;
    converter.set("acres", acres);
    double square_meters = converter.eval("acres * 4046.86");  // 20234.3 m²
    double square_feet = converter.eval("acres * 43560");      // 217800 ft²
    
    std::cout << acres << " acres = " << square_meters << " m²" << std::endl;
    std::cout << acres << " acres = " << square_feet << " ft²" << std::endl;
}
```

### Financial Calculations

```cpp
#include "calculator.hpp"
#include <iostream>
#include <iomanip>

void financial_examples() {
    calculator::ExpressionParser<double> finance;
    
    // Simple interest: I = P * r * t
    finance.set("principal", 1000.0);
    finance.set("rate", 0.05);      // 5%
    finance.set("time", 3.0);       // 3 years
    
    double simple_interest = finance.eval("principal * rate * time");
    double total_amount = finance.eval("principal + principal * rate * time");
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Simple Interest: $" << simple_interest << std::endl;
    std::cout << "Total Amount: $" << total_amount << std::endl;
    
    // Compound interest: A = P(1 + r)^t
    double compound_amount = finance.eval("principal * (1 + rate) ** time");
    double compound_interest = finance.eval("principal * (1 + rate) ** time - principal");
    
    std::cout << "Compound Amount: $" << compound_amount << std::endl;
    std::cout << "Compound Interest: $" << compound_interest << std::endl;
    
    // Monthly payment calculation (simplified)
    finance.set("loan_amount", 200000.0);   // $200,000 loan
    finance.set("annual_rate", 0.04);       // 4% annual rate
    finance.set("years", 30.0);             // 30 years
    
    double monthly_rate = finance.eval("annual_rate / 12");
    double num_payments = finance.eval("years * 12");
    finance.set("monthly_rate", monthly_rate);
    finance.set("num_payments", num_payments);
    
    // This is a simplified calculation - real mortgage calculations are more complex
    double monthly_payment = finance.eval("loan_amount * monthly_rate / (1 - (1 + monthly_rate) ** (-num_payments))");
    
    std::cout << "Monthly Payment: $" << monthly_payment << std::endl;
}
```

## Complete Example Program

```cpp
#include "calculator.hpp"
#include <iostream>
#include <string>
#include <cmath>

class BasicCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    BasicCalculator() {
        setup_constants();
        setup_functions();
    }
    
    double calculate(const std::string& expression) {
        try {
            return parser_.eval(expression);
        } catch (const calculator::error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 0.0;
        }
    }
    
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
    }
    
private:
    void setup_constants() {
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
    }
    
    void setup_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("square", [](double x) { return x * x; });
    }
};

int main() {
    BasicCalculator calc;
    
    // Test basic operations
    std::cout << "Basic arithmetic:" << std::endl;
    std::cout << "2 + 3 * 4 = " << calc.calculate("2 + 3 * 4") << std::endl;
    std::cout << "(2 + 3) * 4 = " << calc.calculate("(2 + 3) * 4") << std::endl;
    
    // Test with variables
    std::cout << "\nWith variables:" << std::endl;
    calc.set_variable("x", 5.0);
    calc.set_variable("y", 3.0);
    std::cout << "x = 5, y = 3" << std::endl;
    std::cout << "x + y = " << calc.calculate("x + y") << std::endl;
    std::cout << "x * y = " << calc.calculate("x * y") << std::endl;
    
    // Test with functions
    std::cout << "\nWith functions:" << std::endl;
    std::cout << "sin(pi/2) = " << calc.calculate("sin(pi/2)") << std::endl;
    std::cout << "sqrt(16) = " << calc.calculate("sqrt(16)") << std::endl;
    std::cout << "square(5) = " << calc.calculate("square(5)") << std::endl;
    
    return 0;
}
```

## Next Steps

Now that you've mastered the basics, explore more advanced topics:

- **[Advanced Examples](advanced-examples.md)** - Complex expressions and optimization
- **[Real-world Usage](real-world-usage.md)** - Production applications and patterns
- **[User Guide](../user-guide/)** - Comprehensive usage documentation
- **[API Reference](../api-reference/)** - Complete class and function documentation
