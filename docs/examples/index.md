# Examples

This section provides comprehensive examples demonstrating how to use the Modern C++ Calculator library in real-world scenarios. From simple calculations to complex mathematical systems, these examples will help you understand the full potential of the library.

## Example Categories

### 📚 Learning Path

Follow this progression to master the calculator library:

1. **[Basic Examples](basic-examples.md)** - Start here for fundamental usage patterns
2. **[Advanced Examples](advanced-examples.md)** - Complex expressions and optimization techniques  
3. **[Real-world Usage](real-world-usage.md)** - Production-ready applications and patterns

### 🎯 By Use Case

| Domain | Examples | Key Features |
|--------|----------|--------------|
| **Mathematics** | Scientific calculations, formulas | Variables, functions, precision |
| **Engineering** | Unit conversions, physics | Complex expressions, constants |
| **Finance** | Interest calculations, metrics | Precision, error handling |
| **Gaming** | Damage systems, physics | Performance, integer operations |
| **Configuration** | Dynamic formulas, rules | Variables, validation |

## Quick Reference

### Simple Calculations

```cpp
// Basic arithmetic
double result = calculator::eval("2 + 3 * 4");  // 14

// With variables
calculator::ExpressionParser<double> parser;
parser.set("x", 5.0);
double result2 = parser.eval("x * 2 + 1");  // 11
```

### Scientific Computing

```cpp
calculator::ExpressionParser<double> sci;
sci.set("pi", 3.141592653589793);
sci.set("sin", [](double x) { return std::sin(x); });
double result = sci.eval("sin(pi/2)");  // 1.0
```

### Error Handling

```cpp
try {
    double result = calculator::eval("5 / 0");
} catch (const calculator::error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## Featured Examples

### 1. Scientific Calculator

A complete scientific calculator implementation:

```cpp
class ScientificCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    ScientificCalculator() {
        setup_constants();
        setup_functions();
    }
    
    double calculate(const std::string& expression) {
        return parser_.eval(expression);
    }
    
private:
    void setup_constants() {
        parser_.set("pi", 3.141592653589793);
        parser_.set("e", 2.718281828459045);
    }
    
    void setup_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("log", [](double x) { return std::log(x); });
    }
};
```

### 2. Configuration System

Dynamic formula evaluation for configuration:

```cpp
class ConfigCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    std::map<std::string, std::string> formulas_;
    
public:
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
    }
    
    void set_formula(const std::string& name, const std::string& formula) {
        formulas_[name] = formula;
    }
    
    double evaluate_formula(const std::string& name) {
        auto it = formulas_.find(name);
        if (it != formulas_.end()) {
            return parser_.eval(it->second);
        }
        throw std::runtime_error("Formula not found: " + name);
    }
};
```

### 3. Game Damage System

High-performance damage calculations:

```cpp
class DamageCalculator {
private:
    calculator::ExpressionParser<int> parser_;
    
public:
    DamageCalculator() {
        // Pre-define common functions
        parser_.set("max", [](int x) { return std::max(x, 0); });
        parser_.set("min", [](int x) { return std::min(x, 100); });
    }
    
    int calculate_damage(int base_damage, int strength, double multiplier) {
        parser_.set("base", base_damage);
        parser_.set("str", strength);
        parser_.set("mult", static_cast<int>(multiplier * 100));
        
        return parser_.eval("max((base + str * 2) * mult / 100)");
    }
};
```

## Code Organization Patterns

### 1. Calculator Wrapper Class

```cpp
template<typename T>
class Calculator {
private:
    calculator::ExpressionParser<T> parser_;
    
public:
    T eval(const std::string& expr) {
        try {
            return parser_.eval(expr);
        } catch (const calculator::error& e) {
            // Add logging, metrics, etc.
            throw;
        }
    }
    
    void set_variable(const std::string& name, T value) {
        parser_.set(name, value);
    }
    
    void set_function(const std::string& name, std::function<T(T)> func) {
        parser_.set(name, func);
    }
};
```

### 2. Expression Validator

```cpp
class ExpressionValidator {
public:
    static bool is_valid(const std::string& expression) {
        try {
            calculator::eval(expression);
            return true;
        } catch (const calculator::error&) {
            return false;
        }
    }
    
    static std::string validate_and_get_error(const std::string& expression) {
        try {
            calculator::eval(expression);
            return "";  // No error
        } catch (const calculator::error& e) {
            return e.what();
        }
    }
};
```

### 3. Batch Processor

```cpp
class BatchCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    std::vector<double> evaluate_batch(const std::vector<std::string>& expressions) {
        std::vector<double> results;
        results.reserve(expressions.size());
        
        for (const auto& expr : expressions) {
            try {
                results.push_back(parser_.eval(expr));
            } catch (const calculator::error&) {
                results.push_back(std::numeric_limits<double>::quiet_NaN());
            }
        }
        
        return results;
    }
};
```

## Performance Tips

### 1. Reuse Parser Instances

```cpp
// Good: Reuse parser
calculator::ExpressionParser<double> parser;
for (const auto& expr : expressions) {
    double result = parser.eval(expr);
    process(result);
}

// Bad: Create new parser each time
for (const auto& expr : expressions) {
    double result = calculator::eval(expr);  // Creates new parser internally
    process(result);
}
```

### 2. Pre-compile Complex Expressions

```cpp
class PrecompiledCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    std::map<std::string, std::string> compiled_expressions_;
    
public:
    void compile_expression(const std::string& name, const std::string& expr) {
        // Validate expression first
        parser_.eval(expr);
        compiled_expressions_[name] = expr;
    }
    
    double evaluate(const std::string& name) {
        auto it = compiled_expressions_.find(name);
        if (it != compiled_expressions_.end()) {
            return parser_.eval(it->second);
        }
        throw std::runtime_error("Expression not found");
    }
};
```

## Testing Patterns

### Unit Testing with Google Test

```cpp
#include <gtest/gtest.h>
#include "calculator.hpp"

class CalculatorTest : public ::testing::Test {
protected:
    calculator::ExpressionParser<double> parser;
    
    void SetUp() override {
        parser.set("pi", 3.141592653589793);
        parser.set("e", 2.718281828459045);
    }
};

TEST_F(CalculatorTest, BasicArithmetic) {
    EXPECT_DOUBLE_EQ(parser.eval("2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("10 / 2"), 5.0);
}

TEST_F(CalculatorTest, Constants) {
    EXPECT_NEAR(parser.eval("pi"), 3.14159, 0.001);
    EXPECT_NEAR(parser.eval("e"), 2.71828, 0.001);
}
```

## Integration Examples

### With JSON Configuration

```cpp
#include <nlohmann/json.hpp>

class JsonCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    void load_config(const nlohmann::json& config) {
        // Load variables
        if (config.contains("variables")) {
            for (const auto& [key, value] : config["variables"].items()) {
                parser_.set(key, value.get<double>());
            }
        }
        
        // Load formulas
        if (config.contains("formulas")) {
            for (const auto& [key, formula] : config["formulas"].items()) {
                // Validate formula
                parser_.eval(formula.get<std::string>());
            }
        }
    }
};
```

### With Command Line Interface

```cpp
#include <iostream>
#include <string>

class CalculatorCLI {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    void run() {
        std::string input;
        std::cout << "Calculator CLI (type 'quit' to exit)\n";
        
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            
            if (input == "quit") break;
            
            try {
                double result = parser_.eval(input);
                std::cout << "Result: " << result << std::endl;
            } catch (const calculator::error& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }
};
```

## Next Steps

Ready to dive deeper? Choose your path:

- **New to the library?** Start with [Basic Examples](basic-examples.md)
- **Need complex calculations?** Jump to [Advanced Examples](advanced-examples.md)  
- **Building production apps?** Check [Real-world Usage](real-world-usage.md)
- **Want to contribute?** See [Development Guide](../development/contributing.md)
