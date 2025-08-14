# Variables & Functions

This guide covers how to use variables and custom functions with the Modern C++ Calculator library. Variables and functions enable dynamic expression evaluation and make the calculator suitable for complex mathematical systems.

## Working with Variables

Variables allow you to store values that can be referenced in expressions. This is particularly useful for complex calculations where you need to reuse values or when building configurable systems.

### Setting Variables

Use the `set()` method to define variables:

```cpp
#include "calculator.hpp"
#include <iostream>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Set basic variables
    parser.set("x", 5.0);
    parser.set("y", 10.0);
    parser.set("z", 2.5);
    
    // Use variables in expressions
    double result1 = parser.eval("x + y");           // 15.0
    double result2 = parser.eval("x * y / z");       // 20.0
    double result3 = parser.eval("(x + y) * z");     // 37.5
    
    std::cout << "x + y = " << result1 << std::endl;
    std::cout << "x * y / z = " << result2 << std::endl;
    std::cout << "(x + y) * z = " << result3 << std::endl;
    
    return 0;
}
```

### Mathematical Constants

Define commonly used mathematical constants:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

void setup_constants(calculator::ExpressionParser<double>& parser) {
    // Mathematical constants
    parser.set("pi", M_PI);                    // 3.14159...
    parser.set("e", M_E);                      // 2.71828...
    parser.set("sqrt2", M_SQRT2);              // 1.41421...
    parser.set("ln2", M_LN2);                  // 0.69314...
    parser.set("ln10", M_LN10);                // 2.30258...
    
    // Physical constants (example values)
    parser.set("c", 299792458.0);              // Speed of light (m/s)
    parser.set("g", 9.80665);                  // Standard gravity (m/s²)
    parser.set("avogadro", 6.02214076e23);     // Avogadro's number
}

int main() {
    calculator::ExpressionParser<double> parser;
    setup_constants(parser);
    
    // Use constants in calculations
    double circle_area = parser.eval("pi * 5 ** 2");              // Area of circle, r=5
    double natural_log = parser.eval("ln2 * 10");                 // ln(2^10) = 10*ln(2)
    double kinetic_energy = parser.eval("0.5 * 10 * 5 ** 2");     // KE = 1/2 * m * v²
    
    std::cout << "Circle area (r=5): " << circle_area << std::endl;
    std::cout << "ln(1024): " << natural_log << std::endl;
    std::cout << "Kinetic energy: " << kinetic_energy << std::endl;
    
    return 0;
}
```

### Dynamic Variable Updates

Variables can be updated during runtime:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_dynamic_variables() {
    calculator::ExpressionParser<double> parser;
    
    // Set initial values
    parser.set("temperature", 20.0);  // Celsius
    parser.set("pressure", 1.0);      // atm
    
    // Calculate initial state
    double initial_volume = parser.eval("temperature * 0.1 + pressure * 5");
    std::cout << "Initial volume: " << initial_volume << std::endl;
    
    // Update variables
    parser.set("temperature", 100.0);  // Heat up
    parser.set("pressure", 2.0);       // Increase pressure
    
    // Recalculate with new values
    double final_volume = parser.eval("temperature * 0.1 + pressure * 5");
    std::cout << "Final volume: " << final_volume << std::endl;
    
    // Calculate change
    parser.set("initial", initial_volume);
    parser.set("final", final_volume);
    double change = parser.eval("final - initial");
    std::cout << "Volume change: " << change << std::endl;
}
```

## Working with Functions

Custom functions enable you to extend the calculator with mathematical operations, transformations, and domain-specific calculations.

### Basic Function Definition

Functions are defined using lambda expressions or function pointers:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Define basic mathematical functions
    parser.set("square", [](double x) { return x * x; });
    parser.set("cube", [](double x) { return x * x * x; });
    parser.set("double_val", [](double x) { return x * 2; });
    parser.set("half", [](double x) { return x / 2; });
    
    // Use functions in expressions
    double result1 = parser.eval("square(5)");              // 25
    double result2 = parser.eval("cube(3)");                // 27
    double result3 = parser.eval("double_val(7.5)");        // 15
    double result4 = parser.eval("half(square(8))");        // 32
    
    std::cout << "square(5) = " << result1 << std::endl;
    std::cout << "cube(3) = " << result2 << std::endl;
    std::cout << "double_val(7.5) = " << result3 << std::endl;
    std::cout << "half(square(8)) = " << result4 << std::endl;
    
    return 0;
}
```

### Standard Mathematical Functions

Implement common mathematical functions:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

void setup_math_functions(calculator::ExpressionParser<double>& parser) {
    // Trigonometric functions
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("cos", [](double x) { return std::cos(x); });
    parser.set("tan", [](double x) { return std::tan(x); });
    parser.set("asin", [](double x) { return std::asin(x); });
    parser.set("acos", [](double x) { return std::acos(x); });
    parser.set("atan", [](double x) { return std::atan(x); });
    
    // Exponential and logarithmic functions
    parser.set("exp", [](double x) { return std::exp(x); });
    parser.set("log", [](double x) { return std::log(x); });      // Natural log
    parser.set("log10", [](double x) { return std::log10(x); });  // Base-10 log
    parser.set("log2", [](double x) { return std::log2(x); });    // Base-2 log
    
    // Power and root functions
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    parser.set("cbrt", [](double x) { return std::cbrt(x); });    // Cube root
    
    // Utility functions
    parser.set("abs", [](double x) { return std::abs(x); });
    parser.set("floor", [](double x) { return std::floor(x); });
    parser.set("ceil", [](double x) { return std::ceil(x); });
    parser.set("round", [](double x) { return std::round(x); });
}

int main() {
    calculator::ExpressionParser<double> parser;
    setup_math_functions(parser);
    
    // Set mathematical constants
    parser.set("pi", M_PI);
    parser.set("e", M_E);
    
    // Demonstrate trigonometric functions
    std::cout << "sin(pi/2) = " << parser.eval("sin(pi/2)") << std::endl;      // 1.0
    std::cout << "cos(0) = " << parser.eval("cos(0)") << std::endl;            // 1.0
    std::cout << "tan(pi/4) = " << parser.eval("tan(pi/4)") << std::endl;      // 1.0
    
    // Demonstrate logarithmic functions
    std::cout << "log(e) = " << parser.eval("log(e)") << std::endl;            // 1.0
    std::cout << "log10(100) = " << parser.eval("log10(100)") << std::endl;    // 2.0
    std::cout << "log2(8) = " << parser.eval("log2(8)") << std::endl;          // 3.0
    
    // Demonstrate utility functions
    std::cout << "sqrt(16) = " << parser.eval("sqrt(16)") << std::endl;        // 4.0
    std::cout << "abs(-5) = " << parser.eval("abs(-5)") << std::endl;          // 5.0
    std::cout << "round(3.7) = " << parser.eval("round(3.7)") << std::endl;    // 4.0
    
    return 0;
}
```

### Domain-Specific Functions

Create functions for specific problem domains:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

// Financial functions
void setup_financial_functions(calculator::ExpressionParser<double>& parser) {
    // Compound interest: A = P(1 + r)^t
    parser.set("compound", [](double principal) {
        // Assumes rate and time are set as variables
        return principal; // This is simplified - see full example below
    });
    
    // Present value calculation
    parser.set("present_value", [](double future_value) {
        return future_value; // Simplified
    });
    
    // Percentage calculation
    parser.set("percent", [](double value) { return value / 100.0; });
}

// Physics functions
void setup_physics_functions(calculator::ExpressionParser<double>& parser) {
    // Kinetic energy: KE = 1/2 * m * v²
    parser.set("kinetic_energy", [](double mass) {
        // Assumes velocity is set as variable 'v'
        return 0.5 * mass; // Simplified
    });
    
    // Gravitational force: F = G * m1 * m2 / r²
    parser.set("gravity_force", [](double mass1) {
        // Simplified - full implementation would need multiple parameters
        return mass1;
    });
    
    // Convert degrees to radians
    parser.set("deg_to_rad", [](double degrees) {
        return degrees * M_PI / 180.0;
    });
    
    // Convert radians to degrees
    parser.set("rad_to_deg", [](double radians) {
        return radians * 180.0 / M_PI;
    });
}

int main() {
    calculator::ExpressionParser<double> parser;
    setup_financial_functions(parser);
    setup_physics_functions(parser);
    
    // Set up constants
    parser.set("pi", M_PI);
    
    // Financial calculations
    parser.set("principal", 1000.0);
    parser.set("rate", 0.05);
    parser.set("time", 10.0);
    
    double compound_interest = parser.eval("principal * (1 + rate) ** time");
    std::cout << "Compound interest: $" << compound_interest << std::endl;
    
    // Physics calculations
    parser.set("mass", 10.0);
    parser.set("velocity", 5.0);
    
    double kinetic_energy = parser.eval("0.5 * mass * velocity ** 2");
    std::cout << "Kinetic energy: " << kinetic_energy << " J" << std::endl;
    
    // Angle conversions
    double radians = parser.eval("deg_to_rad(90)");
    double degrees = parser.eval("rad_to_deg(pi)");
    
    std::cout << "90 degrees = " << radians << " radians" << std::endl;
    std::cout << "π radians = " << degrees << " degrees" << std::endl;
    
    return 0;
}
```

## Advanced Patterns

### Function Composition

Combine functions to create complex calculations:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Set up basic functions
    parser.set("square", [](double x) { return x * x; });
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("cos", [](double x) { return std::cos(x); });
    
    // Set constants
    parser.set("pi", M_PI);
    
    // Function composition examples
    double result1 = parser.eval("sqrt(square(5))");                    // 5.0
    double result2 = parser.eval("square(sin(pi/2))");                  // 1.0
    double result3 = parser.eval("sqrt(square(sin(pi/4)) + square(cos(pi/4)))"); // 1.0
    
    std::cout << "sqrt(square(5)) = " << result1 << std::endl;
    std::cout << "square(sin(π/2)) = " << result2 << std::endl;
    std::cout << "Pythagorean identity = " << result3 << std::endl;
    
    return 0;
}
```

### Conditional Logic with Functions

Implement conditional behavior using mathematical functions:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Define utility functions
    parser.set("max", [](double x) {
        // This is a simplified version - real max would need two parameters
        return std::max(x, 0.0);
    });
    
    parser.set("min", [](double x) {
        return std::min(x, 100.0);
    });
    
    parser.set("clamp", [](double x) {
        return std::max(0.0, std::min(x, 100.0));
    });
    
    parser.set("sign", [](double x) {
        return (x > 0) ? 1.0 : (x < 0) ? -1.0 : 0.0;
    });
    
    // Test conditional functions
    parser.set("value", -5.0);
    double clamped = parser.eval("clamp(value)");
    double sign_val = parser.eval("sign(value)");
    
    std::cout << "clamp(-5) = " << clamped << std::endl;    // 0.0
    std::cout << "sign(-5) = " << sign_val << std::endl;    // -1.0
    
    return 0;
}
```

## Best Practices

### 1. Organize Functions by Domain

```cpp
class ScientificCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    ScientificCalculator() {
        setup_constants();
        setup_trigonometric();
        setup_logarithmic();
        setup_statistical();
    }
    
private:
    void setup_constants() {
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        // ... other constants
    }
    
    void setup_trigonometric() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        // ... other trig functions
    }
    
    void setup_logarithmic() {
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("exp", [](double x) { return std::exp(x); });
        // ... other log functions
    }
    
    void setup_statistical() {
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("round", [](double x) { return std::round(x); });
        // ... other statistical functions
    }
    
public:
    double calculate(const std::string& expression) {
        return parser_.eval(expression);
    }
    
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
    }
};
```

### 2. Error Handling in Functions

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Define functions with error checking
    parser.set("safe_sqrt", [](double x) {
        if (x < 0) {
            throw std::domain_error("Square root of negative number");
        }
        return std::sqrt(x);
    });
    
    parser.set("safe_log", [](double x) {
        if (x <= 0) {
            throw std::domain_error("Logarithm of non-positive number");
        }
        return std::log(x);
    });
    
    parser.set("safe_divide", [](double x) {
        // Assumes 'divisor' is set as a variable
        // This is simplified - real implementation would need two parameters
        return x; // Placeholder
    });
    
    // Test error handling
    try {
        double result = parser.eval("safe_sqrt(16)");  // OK
        std::cout << "safe_sqrt(16) = " << result << std::endl;
        
        // This would throw an exception
        // double bad_result = parser.eval("safe_sqrt(-4)");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

### 3. Performance Considerations

```cpp
// Good: Reuse parser instance
calculator::ExpressionParser<double> parser;
setup_all_functions(parser);

for (const auto& expr : expressions) {
    double result = parser.eval(expr);
    process_result(result);
}

// Bad: Create new parser each time
for (const auto& expr : expressions) {
    calculator::ExpressionParser<double> temp_parser;
    setup_all_functions(temp_parser);  // Expensive!
    double result = temp_parser.eval(expr);
    process_result(result);
}
```

## Next Steps

- Explore [Advanced Features](advanced-features.md) for complex expression patterns
- Review [Error Handling](error-handling.md) for robust applications
- Check [Examples](../examples/advanced-examples.md) for real-world usage patterns
- See [API Reference](../api-reference/expression-parser.md) for complete documentation
