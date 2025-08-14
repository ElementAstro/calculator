# Quick Start Guide

This guide will get you up and running with the Modern C++ Calculator in just a few minutes. By the end, you'll be evaluating mathematical expressions and using advanced features like variables and functions.

## Your First Calculator Program

Let's start with the simplest possible example:

```cpp title="hello_calculator.cpp"
#include "calculator.hpp"
#include <iostream>

int main() {
    // Evaluate a simple expression
    double result = calculator::eval("3 + 4 * 2");
    std::cout << "3 + 4 * 2 = " << result << std::endl;
    
    return 0;
}
```

**Compile and run**:

```bash
g++ -std=c++17 hello_calculator.cpp -o hello_calculator
./hello_calculator
```

**Output**:

```
3 + 4 * 2 = 11
```

!!! success "Congratulations!"
    You've just evaluated your first mathematical expression! The calculator correctly followed the order of operations (multiplication before addition).

## Basic Operations

The calculator supports all standard mathematical operations:

```cpp title="basic_operations.cpp"
#include "calculator.hpp"
#include <iostream>

int main() {
    // Arithmetic operations
    std::cout << "Addition: " << calculator::eval("10 + 5") << std::endl;        // 15
    std::cout << "Subtraction: " << calculator::eval("10 - 5") << std::endl;     // 5
    std::cout << "Multiplication: " << calculator::eval("10 * 5") << std::endl;  // 50
    std::cout << "Division: " << calculator::eval("10 / 5") << std::endl;        // 2
    std::cout << "Modulo: " << calculator::eval<int>("10 % 3") << std::endl;     // 1
    
    // Exponentiation
    std::cout << "Power: " << calculator::eval("2 ** 3") << std::endl;           // 8
    std::cout << "Scientific: " << calculator::eval("1.5e2") << std::endl;       // 150
    
    // Parentheses for grouping
    std::cout << "Grouped: " << calculator::eval("(10 + 5) * 2") << std::endl;   // 30
    
    return 0;
}
```

## Working with Different Types

The calculator is template-based and supports multiple numeric types:

```cpp title="numeric_types.cpp"
#include "calculator.hpp"
#include <iostream>

int main() {
    // Integer calculations
    int int_result = calculator::eval<int>("10 + 5");
    std::cout << "Integer result: " << int_result << std::endl;
    
    // Float calculations
    float float_result = calculator::eval<float>("3.14 * 2");
    std::cout << "Float result: " << float_result << std::endl;
    
    // Double calculations (default)
    double double_result = calculator::eval("1.0 / 3.0");
    std::cout << "Double result: " << double_result << std::endl;
    
    return 0;
}
```

## Using Variables

For more complex calculations, you can define and use variables:

```cpp title="variables_example.cpp"
#include "calculator.hpp"
#include <iostream>

int main() {
    // Create a parser instance
    calculator::ExpressionParser<double> parser;
    
    // Set variables
    parser.set("x", 5.0);
    parser.set("y", 10.0);
    parser.set("pi", 3.14159);
    
    // Use variables in expressions
    double result1 = parser.eval("x + y");           // 15
    double result2 = parser.eval("x * y / 2");       // 25
    double result3 = parser.eval("pi * x ** 2");     // ~78.54
    
    std::cout << "x + y = " << result1 << std::endl;
    std::cout << "x * y / 2 = " << result2 << std::endl;
    std::cout << "pi * x^2 = " << result3 << std::endl;
    
    return 0;
}
```

## Custom Functions

You can also define custom mathematical functions:

```cpp title="functions_example.cpp"
#include "calculator.hpp"
#include <iostream>
#include <cmath>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Define custom functions
    parser.set("square", [](double x) { return x * x; });
    parser.set("cube", [](double x) { return x * x * x; });
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    
    // Set some variables
    parser.set("a", 4.0);
    
    // Use functions in expressions
    double result1 = parser.eval("square(5)");        // 25
    double result2 = parser.eval("cube(3)");          // 27
    double result3 = parser.eval("sqrt(16)");         // 4
    double result4 = parser.eval("square(a) + 9");    // 25
    
    std::cout << "square(5) = " << result1 << std::endl;
    std::cout << "cube(3) = " << result2 << std::endl;
    std::cout << "sqrt(16) = " << result3 << std::endl;
    std::cout << "square(a) + 9 = " << result4 << std::endl;
    
    return 0;
}
```

## Error Handling

The calculator provides robust error handling:

```cpp title="error_handling.cpp"
#include "calculator.hpp"
#include <iostream>

int main() {
    try {
        // This will work fine
        double good_result = calculator::eval("2 + 3");
        std::cout << "Good result: " << good_result << std::endl;
        
        // This will throw an exception (syntax error)
        double bad_result = calculator::eval("2 + + 3");
        
    } catch (const calculator::error& e) {
        std::cerr << "Calculator error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## Complete Example

Here's a more comprehensive example that demonstrates multiple features:

```cpp title="complete_example.cpp"
#include "calculator.hpp"
#include <iostream>
#include <cmath>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Set up variables
    parser.set("radius", 5.0);
    parser.set("height", 10.0);
    parser.set("pi", 3.14159);
    
    // Set up functions
    parser.set("square", [](double x) { return x * x; });
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("cos", [](double x) { return std::cos(x); });
    
    try {
        // Calculate area of a circle
        double circle_area = parser.eval("pi * square(radius)");
        std::cout << "Circle area: " << circle_area << std::endl;
        
        // Calculate volume of a cylinder
        double cylinder_volume = parser.eval("pi * square(radius) * height");
        std::cout << "Cylinder volume: " << cylinder_volume << std::endl;
        
        // Trigonometric calculation
        double trig_result = parser.eval("sin(pi/2) + cos(0)");
        std::cout << "sin(π/2) + cos(0) = " << trig_result << std::endl;
        
        // Complex expression
        double complex_result = parser.eval("(radius + height) * pi / (2 + 3)");
        std::cout << "Complex result: " << complex_result << std::endl;
        
    } catch (const calculator::error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## What's Next?

Now that you've mastered the basics, explore more advanced topics:

- **[Basic Usage](../user-guide/basic-usage.md)**: Detailed guide to all operations
- **[Advanced Features](../user-guide/advanced-features.md)**: Complex expressions and optimization
- **[Variables & Functions](../user-guide/variables-functions.md)**: In-depth guide to variables and functions
- **[Examples](../examples/)**: Real-world use cases and patterns
- **[API Reference](../api-reference/)**: Complete documentation of all classes and methods

## Tips for Success

1. **Always handle exceptions** when evaluating user input
2. **Use appropriate numeric types** for your use case
3. **Reuse parser instances** when evaluating multiple expressions with the same variables/functions
4. **Check the operator precedence** in the [Reference](../reference/operators.md) section
5. **Start simple** and gradually add complexity to your expressions

Happy calculating! 🧮
