# Exception Handling

The calculator library provides robust error handling through the `calculator::error` exception class. This section covers all aspects of error handling, including exception types, error messages, and best practices for handling errors in your applications.

## Exception Class

### calculator::error

The main exception class for all calculator-related errors.

```cpp
class error : public std::exception {
public:
    explicit error(std::string_view expr, const std::string& message);
    const char* what() const noexcept override;
    
    // Additional methods for error details
    std::string_view expression() const noexcept;
    const std::string& message() const noexcept;
};
```

**Inheritance:**

- Inherits from `std::exception`
- Can be caught as `std::exception` or `calculator::error`

**Methods:**

- `what()`: Returns detailed error message
- `expression()`: Returns the expression that caused the error
- `message()`: Returns the specific error message

## Error Categories

### Syntax Errors

Errors in expression syntax or structure.

#### Invalid Operator Sequences

```cpp
try {
    calculator::eval("2 + + 3");        // Double operator
    calculator::eval("* 5");            // Missing left operand
    calculator::eval("5 *");            // Missing right operand
} catch (const calculator::error& e) {
    std::cerr << "Syntax error: " << e.what() << std::endl;
}
```

#### Unmatched Parentheses

```cpp
try {
    calculator::eval("(2 + 3");         // Missing closing parenthesis
    calculator::eval("2 + 3)");         // Missing opening parenthesis
    calculator::eval("((2 + 3)");       // Unbalanced parentheses
} catch (const calculator::error& e) {
    std::cerr << "Parenthesis error: " << e.what() << std::endl;
}
```

#### Invalid Number Formats

```cpp
try {
    calculator::eval("1..2");           // Double decimal point
    calculator::eval("1.2.3");         // Multiple decimal points
    calculator::eval("1e");             // Incomplete scientific notation
} catch (const calculator::error& e) {
    std::cerr << "Number format error: " << e.what() << std::endl;
}
```

#### Empty or Invalid Expressions

```cpp
try {
    calculator::eval("");               // Empty expression
    calculator::eval("   ");            // Whitespace only
    calculator::eval("abc");            // Invalid identifier
} catch (const calculator::error& e) {
    std::cerr << "Invalid expression: " << e.what() << std::endl;
}
```

### Runtime Errors

Errors that occur during expression evaluation.

#### Division by Zero

```cpp
try {
    calculator::eval("5 / 0");          // Direct division by zero
    calculator::eval("10 / (2 - 2)");   // Computed division by zero
    calculator::eval<int>("7 % 0");     // Modulo by zero
} catch (const calculator::error& e) {
    std::cerr << "Division by zero: " << e.what() << std::endl;
}
```

#### Undefined Variables

```cpp
calculator::ExpressionParser<double> parser;

try {
    parser.eval("x + y");               // Undefined variables
    parser.eval("unknown_var * 5");     // Undefined variable
} catch (const calculator::error& e) {
    std::cerr << "Undefined variable: " << e.what() << std::endl;
}
```

#### Undefined Functions

```cpp
calculator::ExpressionParser<double> parser;

try {
    parser.eval("sqrt(16)");            // Undefined function
    parser.eval("sin(3.14)");           // Undefined function
} catch (const calculator::error& e) {
    std::cerr << "Undefined function: " << e.what() << std::endl;
}
```

### Type Errors

Errors related to type mismatches or unsupported operations.

#### Floating-Point in Integer Context

```cpp
calculator::ExpressionParser<int> int_parser;

try {
    int_parser.eval("3.14");            // Float literal in int parser
    int_parser.eval("1.5 + 2.5");       // Float arithmetic in int parser
} catch (const calculator::error& e) {
    std::cerr << "Type error: " << e.what() << std::endl;
}
```

#### Bitwise Operations on Floating-Point

```cpp
try {
    calculator::eval<double>("5.5 & 3.2");  // Bitwise AND on doubles
    calculator::eval<float>("7.1 | 2.3");   // Bitwise OR on floats
} catch (const calculator::error& e) {
    std::cerr << "Unsupported operation: " << e.what() << std::endl;
}
```

## Error Handling Patterns

### Basic Exception Handling

```cpp
#include "calculator.hpp"
#include <iostream>

double safe_calculate(const std::string& expression) {
    try {
        return calculator::eval(expression);
    } catch (const calculator::error& e) {
        std::cerr << "Calculator error: " << e.what() << std::endl;
        return 0.0;  // Default value
    }
}
```

### Detailed Error Information

```cpp
#include "calculator.hpp"
#include <iostream>

void detailed_error_handling(const std::string& expression) {
    try {
        double result = calculator::eval(expression);
        std::cout << "Result: " << result << std::endl;
    } catch (const calculator::error& e) {
        std::cerr << "Error in expression: '" << e.expression() << "'" << std::endl;
        std::cerr << "Error message: " << e.message() << std::endl;
        std::cerr << "Full error: " << e.what() << std::endl;
    }
}
```

### Multiple Exception Types

```cpp
#include "calculator.hpp"
#include <iostream>
#include <stdexcept>

void comprehensive_error_handling(const std::string& expression) {
    try {
        double result = calculator::eval(expression);
        std::cout << "Result: " << result << std::endl;
    } catch (const calculator::error& e) {
        // Calculator-specific errors
        std::cerr << "Calculator error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // Other standard exceptions
        std::cerr << "Standard error: " << e.what() << std::endl;
    } catch (...) {
        // Unknown errors
        std::cerr << "Unknown error occurred" << std::endl;
    }
}
```

### Error Recovery

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>

class CalculatorWithFallback {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    double evaluate_with_fallback(const std::string& expression, double fallback = 0.0) {
        try {
            return parser_.eval(expression);
        } catch (const calculator::error& e) {
            std::cerr << "Using fallback value due to error: " << e.what() << std::endl;
            return fallback;
        }
    }
    
    std::vector<double> evaluate_batch(const std::vector<std::string>& expressions) {
        std::vector<double> results;
        
        for (const auto& expr : expressions) {
            try {
                results.push_back(parser_.eval(expr));
            } catch (const calculator::error& e) {
                std::cerr << "Skipping invalid expression '" << expr << "': " << e.what() << std::endl;
                results.push_back(std::numeric_limits<double>::quiet_NaN());
            }
        }
        
        return results;
    }
};
```

## Error Message Format

Error messages follow a consistent format:

```
Error: [specific error description] at position [position] in expression '[expression]'
```

### Example Error Messages

```cpp
// Syntax errors
"Error: Unexpected token '+' at position 3 in expression '2 + + 3'"
"Error: Unmatched opening parenthesis at position 0 in expression '(2 + 3'"
"Error: Invalid number format at position 1 in expression '1..2'"

// Runtime errors  
"Error: Division by zero at position 4 in expression '5 / 0'"
"Error: Undefined variable 'x' at position 0 in expression 'x + 5'"
"Error: Undefined function 'sqrt' at position 0 in expression 'sqrt(16)'"

// Type errors
"Error: Floating-point literal not allowed in integer context at position 0 in expression '3.14'"
"Error: Bitwise operation not supported for floating-point types at position 4 in expression '5.5 & 3.2'"
```

## Best Practices

### 1. Always Handle Exceptions

```cpp
// Good: Always wrap in try-catch
try {
    double result = calculator::eval(user_input);
    process_result(result);
} catch (const calculator::error& e) {
    handle_error(e);
}

// Bad: No exception handling
double result = calculator::eval(user_input);  // May throw!
```

### 2. Validate Input When Possible

```cpp
bool is_valid_expression(const std::string& expr) {
    if (expr.empty() || expr.find_first_not_of(" \t\n\r") == std::string::npos) {
        return false;  // Empty or whitespace only
    }
    
    // Additional validation logic...
    return true;
}

double safe_eval(const std::string& expr) {
    if (!is_valid_expression(expr)) {
        throw std::invalid_argument("Invalid expression format");
    }
    
    return calculator::eval(expr);
}
```

### 3. Provide Meaningful Error Messages

```cpp
void user_friendly_calculator(const std::string& expression) {
    try {
        double result = calculator::eval(expression);
        std::cout << "Result: " << result << std::endl;
    } catch (const calculator::error& e) {
        std::string error_msg = e.what();
        
        if (error_msg.find("Division by zero") != std::string::npos) {
            std::cerr << "Error: Cannot divide by zero. Please check your expression." << std::endl;
        } else if (error_msg.find("Undefined variable") != std::string::npos) {
            std::cerr << "Error: Unknown variable. Please define all variables first." << std::endl;
        } else {
            std::cerr << "Error: " << error_msg << std::endl;
        }
    }
}
```

### 4. Log Errors for Debugging

```cpp
#include <fstream>
#include <chrono>

class ErrorLogger {
private:
    std::ofstream log_file_;
    
public:
    ErrorLogger(const std::string& filename) : log_file_(filename, std::ios::app) {}
    
    void log_error(const calculator::error& e) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        log_file_ << "[" << std::ctime(&time_t) << "] "
                  << "Expression: '" << e.expression() << "' "
                  << "Error: " << e.message() << std::endl;
    }
};

double logged_eval(const std::string& expression, ErrorLogger& logger) {
    try {
        return calculator::eval(expression);
    } catch (const calculator::error& e) {
        logger.log_error(e);
        throw;  // Re-throw for caller to handle
    }
}
```

## Testing Error Conditions

### Unit Testing Error Cases

```cpp
#include <gtest/gtest.h>
#include "calculator.hpp"

TEST(CalculatorErrorTest, SyntaxErrors) {
    EXPECT_THROW(calculator::eval("2 + + 3"), calculator::error);
    EXPECT_THROW(calculator::eval("(2 + 3"), calculator::error);
    EXPECT_THROW(calculator::eval(""), calculator::error);
}

TEST(CalculatorErrorTest, RuntimeErrors) {
    EXPECT_THROW(calculator::eval("5 / 0"), calculator::error);
    EXPECT_THROW(calculator::eval<int>("7 % 0"), calculator::error);
}

TEST(CalculatorErrorTest, TypeErrors) {
    EXPECT_THROW(calculator::eval<int>("3.14"), calculator::error);
    EXPECT_THROW(calculator::eval<double>("5.5 & 3.2"), calculator::error);
}
```

## See Also

- [ExpressionParser Class](expression-parser.md) - Main parser class documentation
- [Free Functions](functions.md) - Global evaluation functions
- [Examples](../examples/basic-examples.md) - Practical error handling examples
