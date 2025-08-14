# Error Handling

This guide covers comprehensive error handling strategies for the Modern C++ Calculator library. Learn how to build robust applications that gracefully handle errors, provide meaningful feedback, and recover from exceptional conditions.

## Understanding Calculator Errors

The calculator library throws `calculator::error` exceptions for various error conditions. Understanding these error types helps you build more robust applications.

### Error Categories

#### 1. Syntax Errors

Errors in expression syntax or structure:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_syntax_errors() {
    std::vector<std::string> invalid_expressions = {
        "2 + + 3",          // Double operator
        "* 5",              // Missing left operand
        "5 *",              // Missing right operand
        "(2 + 3",           // Unmatched parenthesis
        "2 + 3)",           // Extra closing parenthesis
        "",                 // Empty expression
        "   ",              // Whitespace only
        "1..2",             // Invalid number format
        "1.2.3",            // Multiple decimal points
    };
    
    for (const auto& expr : invalid_expressions) {
        try {
            double result = calculator::eval(expr);
            std::cout << "Unexpected success: " << expr << " = " << result << std::endl;
        } catch (const calculator::error& e) {
            std::cout << "Syntax error in '" << expr << "': " << e.what() << std::endl;
        }
    }
}
```

#### 2. Runtime Errors

Errors that occur during expression evaluation:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_runtime_errors() {
    calculator::ExpressionParser<double> parser;
    
    std::vector<std::string> runtime_error_expressions = {
        "5 / 0",            // Division by zero
        "10 / (2 - 2)",     // Computed division by zero
        "x + 5",            // Undefined variable
        "sqrt(16)",         // Undefined function
        "unknown_func(5)",  // Undefined function
    };
    
    for (const auto& expr : runtime_error_expressions) {
        try {
            double result = parser.eval(expr);
            std::cout << "Unexpected success: " << expr << " = " << result << std::endl;
        } catch (const calculator::error& e) {
            std::cout << "Runtime error in '" << expr << "': " << e.what() << std::endl;
        }
    }
}
```

#### 3. Type Errors

Errors related to type mismatches or unsupported operations:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_type_errors() {
    std::vector<std::pair<std::string, std::string>> type_error_cases = {
        {"3.14", "Float literal in int context"},
        {"1.5 + 2.5", "Float arithmetic in int context"},
        {"5.5 & 3.2", "Bitwise operation on floating-point"},
        {"7.1 | 2.3", "Bitwise OR on floating-point"},
    };
    
    for (const auto& [expr, description] : type_error_cases) {
        try {
            // Try with integer parser
            int result = calculator::eval<int>(expr);
            std::cout << "Unexpected success: " << expr << " = " << result << std::endl;
        } catch (const calculator::error& e) {
            std::cout << description << " - '" << expr << "': " << e.what() << std::endl;
        }
        
        try {
            // Try bitwise operations with double parser
            if (expr.find('&') != std::string::npos || expr.find('|') != std::string::npos) {
                double result = calculator::eval<double>(expr);
                std::cout << "Unexpected success: " << expr << " = " << result << std::endl;
            }
        } catch (const calculator::error& e) {
            std::cout << description << " - '" << expr << "': " << e.what() << std::endl;
        }
    }
}
```

## Error Handling Patterns

### Basic Exception Handling

The fundamental pattern for handling calculator errors:

```cpp
#include "calculator.hpp"
#include <iostream>

double safe_evaluate(const std::string& expression) {
    try {
        return calculator::eval(expression);
    } catch (const calculator::error& e) {
        std::cerr << "Calculator error: " << e.what() << std::endl;
        return 0.0;  // Default value
    } catch (const std::exception& e) {
        std::cerr << "Standard error: " << e.what() << std::endl;
        return 0.0;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 0.0;
    }
}

int main() {
    std::vector<std::string> expressions = {
        "2 + 3",        // Valid
        "10 / 2",       // Valid
        "5 / 0",        // Error: Division by zero
        "2 + + 3",      // Error: Syntax error
        "unknown_var"   // Error: Undefined variable
    };
    
    for (const auto& expr : expressions) {
        double result = safe_evaluate(expr);
        std::cout << expr << " = " << result << std::endl;
    }
    
    return 0;
}
```

### Detailed Error Information

Extract detailed information from calculator errors:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <string>

struct ErrorInfo {
    std::string expression;
    std::string message;
    std::string full_error;
    bool is_calculator_error;
};

ErrorInfo analyze_error(const std::string& expression) {
    ErrorInfo info;
    info.expression = expression;
    info.is_calculator_error = false;
    
    try {
        calculator::eval(expression);
        info.message = "No error";
        info.full_error = "Expression evaluated successfully";
    } catch (const calculator::error& e) {
        info.is_calculator_error = true;
        info.message = "Calculator error";
        info.full_error = e.what();
    } catch (const std::exception& e) {
        info.message = "Standard exception";
        info.full_error = e.what();
    } catch (...) {
        info.message = "Unknown error";
        info.full_error = "Unhandled exception type";
    }
    
    return info;
}

void print_error_analysis(const ErrorInfo& info) {
    std::cout << "Expression: '" << info.expression << "'" << std::endl;
    std::cout << "Error type: " << info.message << std::endl;
    std::cout << "Calculator error: " << (info.is_calculator_error ? "Yes" : "No") << std::endl;
    std::cout << "Details: " << info.full_error << std::endl;
    std::cout << "---" << std::endl;
}

int main() {
    std::vector<std::string> test_expressions = {
        "2 + 3",
        "5 / 0",
        "2 + + 3",
        "(2 + 3",
        "unknown_var",
        "3.14",  // This might work depending on context
    };
    
    for (const auto& expr : test_expressions) {
        auto error_info = analyze_error(expr);
        print_error_analysis(error_info);
    }
    
    return 0;
}
```

### Error Recovery Strategies

Implement different recovery strategies based on error types:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <regex>

class RobustCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    RobustCalculator() {
        setup_default_variables();
        setup_default_functions();
    }
    
    void setup_default_variables() {
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        parser_.set("zero", 0.0);
        parser_.set("one", 1.0);
    }
    
    void setup_default_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
    }
    
    std::optional<double> evaluate_with_recovery(const std::string& expression) {
        // First, try direct evaluation
        try {
            return parser_.eval(expression);
        } catch (const calculator::error& e) {
            std::string error_msg = e.what();
            
            // Try different recovery strategies
            if (error_msg.find("Division by zero") != std::string::npos) {
                return handle_division_by_zero(expression);
            } else if (error_msg.find("Undefined variable") != std::string::npos) {
                return handle_undefined_variable(expression, error_msg);
            } else if (error_msg.find("Undefined function") != std::string::npos) {
                return handle_undefined_function(expression, error_msg);
            } else if (error_msg.find("Syntax error") != std::string::npos) {
                return handle_syntax_error(expression);
            }
            
            // No recovery possible
            std::cerr << "Unrecoverable error: " << error_msg << std::endl;
            return std::nullopt;
        }
    }
    
private:
    std::optional<double> handle_division_by_zero(const std::string& expression) {
        std::cerr << "Division by zero detected, returning infinity" << std::endl;
        return std::numeric_limits<double>::infinity();
    }
    
    std::optional<double> handle_undefined_variable(const std::string& expression, 
                                                   const std::string& error_msg) {
        // Extract variable name from error message (simplified)
        std::regex var_regex(R"(Undefined variable '(\w+)')");
        std::smatch match;
        
        if (std::regex_search(error_msg, match, var_regex)) {
            std::string var_name = match[1].str();
            std::cerr << "Setting undefined variable '" << var_name << "' to 0" << std::endl;
            
            parser_.set(var_name, 0.0);
            
            try {
                return parser_.eval(expression);
            } catch (const calculator::error&) {
                return std::nullopt;
            }
        }
        
        return std::nullopt;
    }
    
    std::optional<double> handle_undefined_function(const std::string& expression,
                                                   const std::string& error_msg) {
        std::cerr << "Undefined function detected, cannot recover automatically" << std::endl;
        return std::nullopt;
    }
    
    std::optional<double> handle_syntax_error(const std::string& expression) {
        // Try simple fixes for common syntax errors
        std::string fixed_expr = expression;
        
        // Remove double operators
        fixed_expr = std::regex_replace(fixed_expr, std::regex(R"(\+\s*\+)"), "+");
        fixed_expr = std::regex_replace(fixed_expr, std::regex(R"(-\s*-)"), "+");
        fixed_expr = std::regex_replace(fixed_expr, std::regex(R"(\*\s*\*)"), "**");
        
        // Try to balance parentheses (simplified)
        int open_count = 0;
        for (char c : fixed_expr) {
            if (c == '(') open_count++;
            else if (c == ')') open_count--;
        }
        
        // Add missing closing parentheses
        while (open_count > 0) {
            fixed_expr += ")";
            open_count--;
        }
        
        if (fixed_expr != expression) {
            std::cerr << "Attempting syntax fix: '" << expression << "' -> '" << fixed_expr << "'" << std::endl;
            
            try {
                return parser_.eval(fixed_expr);
            } catch (const calculator::error&) {
                return std::nullopt;
            }
        }
        
        return std::nullopt;
    }
};

int main() {
    RobustCalculator calc;
    
    std::vector<std::string> problematic_expressions = {
        "2 + 3",            // Valid
        "5 / 0",            // Division by zero
        "x + 5",            // Undefined variable
        "2 + + 3",          // Syntax error
        "(2 + 3",           // Missing parenthesis
        "unknown_func(5)",  // Undefined function
    };
    
    for (const auto& expr : problematic_expressions) {
        auto result = calc.evaluate_with_recovery(expr);
        
        if (result) {
            std::cout << "'" << expr << "' = " << *result << std::endl;
        } else {
            std::cout << "'" << expr << "' = FAILED" << std::endl;
        }
        std::cout << std::endl;
    }
    
    return 0;
}
```

## Production Error Handling

### Logging and Monitoring

Implement comprehensive error logging for production systems:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

class ProductionCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    std::ofstream error_log_;
    std::ofstream audit_log_;
    size_t error_count_;
    size_t success_count_;
    
public:
    ProductionCalculator(const std::string& log_dir = "logs") {
        error_count_ = 0;
        success_count_ = 0;
        
        // Create log directory if it doesn't exist (simplified)
        error_log_.open(log_dir + "/calculator_errors.log", std::ios::app);
        audit_log_.open(log_dir + "/calculator_audit.log", std::ios::app);
        
        setup_calculator();
        log_info("Calculator initialized");
    }
    
    ~ProductionCalculator() {
        log_info("Calculator shutting down. Success: " + std::to_string(success_count_) + 
                ", Errors: " + std::to_string(error_count_));
    }
    
    void setup_calculator() {
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("log", [](double x) { return std::log(x); });
    }
    
    struct EvaluationResult {
        bool success;
        double value;
        std::string error_message;
        std::chrono::microseconds execution_time;
    };
    
    EvaluationResult evaluate(const std::string& expression, const std::string& context = "") {
        auto start_time = std::chrono::high_resolution_clock::now();
        EvaluationResult result;
        
        try {
            result.value = parser_.eval(expression);
            result.success = true;
            success_count_++;
            
            auto end_time = std::chrono::high_resolution_clock::now();
            result.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            
            log_success(expression, result.value, result.execution_time, context);
            
        } catch (const calculator::error& e) {
            result.success = false;
            result.value = 0.0;
            result.error_message = e.what();
            error_count_++;
            
            auto end_time = std::chrono::high_resolution_clock::now();
            result.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            
            log_error(expression, result.error_message, result.execution_time, context);
            
        } catch (const std::exception& e) {
            result.success = false;
            result.value = 0.0;
            result.error_message = "Standard exception: " + std::string(e.what());
            error_count_++;
            
            auto end_time = std::chrono::high_resolution_clock::now();
            result.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            
            log_error(expression, result.error_message, result.execution_time, context);
        }
        
        return result;
    }
    
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
        log_info("Variable set: " + name + " = " + std::to_string(value));
    }
    
    size_t get_error_count() const { return error_count_; }
    size_t get_success_count() const { return success_count_; }
    double get_success_rate() const {
        size_t total = error_count_ + success_count_;
        return total > 0 ? (double)success_count_ / total : 0.0;
    }
    
private:
    std::string get_timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    void log_error(const std::string& expression, const std::string& error_message,
                   std::chrono::microseconds execution_time, const std::string& context) {
        if (error_log_.is_open()) {
            error_log_ << get_timestamp() << " [ERROR] "
                      << "Expression: '" << expression << "' "
                      << "Error: " << error_message << " "
                      << "Time: " << execution_time.count() << "μs";
            
            if (!context.empty()) {
                error_log_ << " Context: " << context;
            }
            
            error_log_ << std::endl;
            error_log_.flush();
        }
    }
    
    void log_success(const std::string& expression, double result,
                     std::chrono::microseconds execution_time, const std::string& context) {
        if (audit_log_.is_open()) {
            audit_log_ << get_timestamp() << " [SUCCESS] "
                      << "Expression: '" << expression << "' "
                      << "Result: " << result << " "
                      << "Time: " << execution_time.count() << "μs";
            
            if (!context.empty()) {
                audit_log_ << " Context: " << context;
            }
            
            audit_log_ << std::endl;
            audit_log_.flush();
        }
    }
    
    void log_info(const std::string& message) {
        if (audit_log_.is_open()) {
            audit_log_ << get_timestamp() << " [INFO] " << message << std::endl;
            audit_log_.flush();
        }
    }
};

int main() {
    ProductionCalculator calc;
    
    // Simulate production usage
    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"2 + 3", "basic_arithmetic"},
        {"sin(pi/2)", "trigonometry"},
        {"5 / 0", "division_test"},
        {"sqrt(16)", "math_function"},
        {"x + 5", "variable_test"},
        {"2 + + 3", "syntax_test"},
    };
    
    // Set up some variables
    calc.set_variable("x", 10.0);
    
    // Process test cases
    for (const auto& [expression, context] : test_cases) {
        auto result = calc.evaluate(expression, context);
        
        if (result.success) {
            std::cout << "✓ " << expression << " = " << result.value 
                     << " (" << result.execution_time.count() << "μs)" << std::endl;
        } else {
            std::cout << "✗ " << expression << " failed: " << result.error_message 
                     << " (" << result.execution_time.count() << "μs)" << std::endl;
        }
    }
    
    // Print statistics
    std::cout << "\nStatistics:" << std::endl;
    std::cout << "Success: " << calc.get_success_count() << std::endl;
    std::cout << "Errors: " << calc.get_error_count() << std::endl;
    std::cout << "Success rate: " << (calc.get_success_rate() * 100) << "%" << std::endl;
    
    return 0;
}
```

## Best Practices

### 1. Always Handle Exceptions

```cpp
// Good: Always wrap in try-catch
try {
    double result = calculator::eval(user_input);
    process_result(result);
} catch (const calculator::error& e) {
    handle_calculator_error(e);
} catch (const std::exception& e) {
    handle_standard_error(e);
}

// Bad: No exception handling
double result = calculator::eval(user_input);  // May throw!
```

### 2. Provide User-Friendly Error Messages

```cpp
std::string get_user_friendly_error(const calculator::error& e) {
    std::string error_msg = e.what();
    
    if (error_msg.find("Division by zero") != std::string::npos) {
        return "Cannot divide by zero. Please check your expression.";
    } else if (error_msg.find("Undefined variable") != std::string::npos) {
        return "Unknown variable. Please define all variables first.";
    } else if (error_msg.find("Undefined function") != std::string::npos) {
        return "Unknown function. Please check the function name.";
    } else if (error_msg.find("Syntax error") != std::string::npos) {
        return "Invalid expression syntax. Please check your expression.";
    } else {
        return "Calculation error: " + error_msg;
    }
}
```

### 3. Validate Input When Possible

```cpp
bool is_valid_expression_format(const std::string& expr) {
    if (expr.empty() || expr.find_first_not_of(" \t\n\r") == std::string::npos) {
        return false;  // Empty or whitespace only
    }
    
    // Check for balanced parentheses
    int paren_count = 0;
    for (char c : expr) {
        if (c == '(') paren_count++;
        else if (c == ')') paren_count--;
        if (paren_count < 0) return false;  // More closing than opening
    }
    
    return paren_count == 0;  // Balanced parentheses
}
```

### 4. Use RAII for Resource Management

```cpp
class SafeCalculator {
private:
    std::unique_ptr<calculator::ExpressionParser<double>> parser_;
    
public:
    SafeCalculator() : parser_(std::make_unique<calculator::ExpressionParser<double>>()) {
        setup_default_environment();
    }
    
    // Automatic cleanup through RAII
    ~SafeCalculator() = default;
    
    // Move semantics for efficiency
    SafeCalculator(SafeCalculator&&) = default;
    SafeCalculator& operator=(SafeCalculator&&) = default;
    
    // Disable copying to prevent issues
    SafeCalculator(const SafeCalculator&) = delete;
    SafeCalculator& operator=(const SafeCalculator&) = delete;
};
```

## Next Steps

- Explore [Examples](../examples/advanced-examples.md) for real-world error handling patterns
- Review [API Reference](../api-reference/exceptions.md) for complete exception documentation
- Check [Advanced Features](advanced-features.md) for complex error recovery strategies
- See [Basic Usage](basic-usage.md) for fundamental error handling patterns
