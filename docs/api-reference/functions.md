# Free Functions

The calculator library provides convenient free functions for quick expression evaluation without the need to create parser instances. These functions are ideal for simple calculations and one-off evaluations.

## Overview

The free functions provide a simplified interface to the calculator's functionality:

- **`eval<T>(expression)`**: Template function for type-specific evaluation
- **`eval(expression)`**: Convenience function for double-precision evaluation

## Function Reference

### eval<T>(expression) {#eval-template}

Template function for evaluating expressions with a specific numeric type.

```cpp
template<typename T>
T eval(std::string_view expression);
```

**Template Parameters:**

- `T`: Numeric type for the result (`int`, `float`, or `double`)

**Parameters:**

- `expression`: Mathematical expression to evaluate

**Returns:**

- Result of the expression as type `T`

**Throws:**

- `calculator::error`: On syntax errors or evaluation failures

**Example:**

```cpp
#include "calculator.hpp"

// Integer evaluation
int int_result = calculator::eval<int>("10 + 5");        // 15
int bitwise = calculator::eval<int>("7 & 3");            // 3

// Float evaluation  
float float_result = calculator::eval<float>("3.14 * 2"); // 6.28f

// Double evaluation
double double_result = calculator::eval<double>("1.0 / 3.0"); // 0.333...
```

### eval(expression) {#eval-double}

Convenience function for double-precision expression evaluation.

```cpp
double eval(const std::string& expression);
```

**Parameters:**

- `expression`: Mathematical expression to evaluate

**Returns:**

- Result of the expression as `double`

**Throws:**

- `calculator::error`: On syntax errors or evaluation failures

**Example:**

```cpp
#include "calculator.hpp"

double result1 = calculator::eval("2 + 3 * 4");          // 14.0
double result2 = calculator::eval("(10 + 5) / 3");       // 5.0
double result3 = calculator::eval("2 ** 3");             // 8.0
double result4 = calculator::eval("1.5e2");              // 150.0
```

## Usage Patterns

### Quick Calculations

For simple, one-off calculations:

```cpp
#include "calculator.hpp"
#include <iostream>

int main() {
    // Simple arithmetic
    std::cout << "2 + 3 = " << calculator::eval("2 + 3") << std::endl;
    
    // Complex expression
    std::cout << "Result = " << calculator::eval("(10 + 5) * 2 / 3") << std::endl;
    
    return 0;
}
```

### Type-Specific Calculations

When you need specific numeric types:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_types() {
    // Integer calculations (with bitwise operations)
    int int_result = calculator::eval<int>("15 & 7");  // 7
    std::cout << "Bitwise AND: " << int_result << std::endl;
    
    // Float calculations
    float float_result = calculator::eval<float>("3.14159 * 2");
    std::cout << "Float result: " << float_result << std::endl;
    
    // Double calculations (default)
    double double_result = calculator::eval("1.0 / 3.0");
    std::cout << "Double result: " << double_result << std::endl;
}
```

### Error Handling

Always wrap evaluations in try-catch blocks for robust applications:

```cpp
#include "calculator.hpp"
#include <iostream>

double safe_eval(const std::string& expression) {
    try {
        return calculator::eval(expression);
    } catch (const calculator::error& e) {
        std::cerr << "Error evaluating '" << expression << "': " << e.what() << std::endl;
        return 0.0;  // or throw, or return default value
    }
}

int main() {
    double result1 = safe_eval("2 + 3");        // Works fine
    double result2 = safe_eval("2 + + 3");      // Handles error gracefully
    
    return 0;
}
```

## Type Behavior Differences

### Integer Type (`int`)

```cpp
// Supports all operations including bitwise
int result1 = calculator::eval<int>("10 / 3");     // 3 (integer division)
int result2 = calculator::eval<int>("7 % 3");      // 1 (modulo)
int result3 = calculator::eval<int>("5 & 3");      // 1 (bitwise AND)
int result4 = calculator::eval<int>("5 | 3");      // 7 (bitwise OR)
int result5 = calculator::eval<int>("5 ^ 3");      // 6 (bitwise XOR)
int result6 = calculator::eval<int>("8 >> 1");     // 4 (right shift)
int result7 = calculator::eval<int>("4 << 1");     // 8 (left shift)
int result8 = calculator::eval<int>("~5");         // -6 (bitwise NOT)
```

### Floating-Point Types (`float`, `double`)

```cpp
// Supports arithmetic and power operations (no bitwise)
double result1 = calculator::eval<double>("10.0 / 3.0");  // 3.333...
double result2 = calculator::eval<double>("2.5 ** 2");    // 6.25
double result3 = calculator::eval<double>("1.5e2");       // 150.0

// Bitwise operations will throw errors for floating-point types
// calculator::eval<double>("5.5 & 3.2");  // Error!
```

## Performance Considerations

### When to Use Free Functions

**Good for:**

- Simple, one-off calculations
- Quick prototyping
- Command-line calculators
- Configuration value parsing

**Example:**

```cpp
// Good: Simple calculation
double tip = calculator::eval("bill * 0.15");

// Good: One-time configuration parsing
double config_value = calculator::eval(config_string);
```

### When to Use ExpressionParser

**Better for:**

- Multiple evaluations with same variables/functions
- Complex mathematical systems
- Performance-critical applications
- Applications with custom functions

**Example:**

```cpp
// Better: Multiple evaluations with variables
calculator::ExpressionParser<double> parser;
parser.set("rate", 0.05);
parser.set("principal", 1000.0);

for (int years = 1; years <= 10; ++years) {
    parser.set("time", years);
    double amount = parser.eval("principal * (1 + rate) ** time");
    std::cout << "Year " << years << ": $" << amount << std::endl;
}
```

## Limitations

### No Variables or Functions

Free functions cannot use variables or custom functions:

```cpp
// This will NOT work with free functions
// calculator::eval("x + y");  // Error: undefined variables

// Use ExpressionParser instead
calculator::ExpressionParser<double> parser;
parser.set("x", 5.0);
parser.set("y", 10.0);
double result = parser.eval("x + y");  // Works!
```

### No State Persistence

Each call to free functions creates a new parser internally:

```cpp
// Each call is independent
double result1 = calculator::eval("2 + 3");  // New parser
double result2 = calculator::eval("4 * 5");  // New parser (no shared state)
```

## Error Types

Free functions can throw the same errors as `ExpressionParser`:

### Syntax Errors

```cpp
try {
    calculator::eval("2 + + 3");        // Invalid syntax
    calculator::eval("(2 + 3");         // Unmatched parenthesis
    calculator::eval("");               // Empty expression
} catch (const calculator::error& e) {
    std::cerr << "Syntax error: " << e.what() << std::endl;
}
```

### Runtime Errors

```cpp
try {
    calculator::eval("5 / 0");          // Division by zero
    calculator::eval<int>("3.14");      // Type mismatch
} catch (const calculator::error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
}
```

## Best Practices

1. **Use appropriate types** for your calculations
2. **Always handle exceptions** in production code
3. **Prefer ExpressionParser** for complex or repeated evaluations
4. **Validate input** before evaluation when dealing with user input
5. **Use const references** for string parameters when possible

## See Also

- [ExpressionParser Class](expression-parser.md) - For advanced usage with variables and functions
- [Exception Handling](exceptions.md) - Detailed error handling information
- [Examples](../examples/basic-examples.md) - Practical usage examples
