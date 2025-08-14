# Calculator Library Examples

This directory contains comprehensive examples demonstrating various features and usage patterns of the Modern C++ Calculator library.

## Examples Overview

### 1. `main.cpp` - Basic Usage Examples

**Purpose**: Comprehensive introduction to the calculator library  
**Topics Covered**:

- Basic arithmetic operations
- Variables and custom functions
- Different numeric types (int, float, double)
- Error handling fundamentals
- Advanced mathematical operations
- Real-world applications (physics, finance, engineering)

**Key Features Demonstrated**:

- Expression evaluation with `calculator::eval()`
- Using `ExpressionParser` for variables and functions
- Mathematical constants and functions
- Scientific notation
- Operator precedence

### 2. `advanced_usage.cpp` - Advanced Patterns

**Purpose**: Advanced usage patterns and optimization techniques  
**Topics Covered**:

- Performance optimization strategies
- Scientific computing applications
- Error recovery mechanisms
- Memory management patterns
- Thread safety considerations

**Key Features Demonstrated**:

- Parser reuse vs recreation performance
- Expression caching
- Numerical integration and root finding
- Statistical calculations
- RAII patterns and object pooling
- Thread-local storage and parallel computation

### 3. `error_handling_demo.cpp` - Error Management

**Purpose**: Comprehensive error handling strategies  
**Topics Covered**:

- Basic error handling patterns
- Error categorization and specific handling
- Robust error reporting
- Production-ready error management

**Key Features Demonstrated**:

- Try-catch patterns with `calculator::error`
- Safe evaluation functions
- Optional-based error handling
- Error categorization and recovery
- Production error logging and statistics

### 4. `numeric_types_demo.cpp` - Type System

**Purpose**: Detailed exploration of numeric types  
**Topics Covered**:

- Integer operations and bitwise arithmetic
- Floating-point precision differences
- Type-specific behaviors and limitations
- Performance characteristics
- Best practices for type selection

**Key Features Demonstrated**:

- Integer division and modulo operations
- Bitwise operations (AND, OR, XOR, shifts)
- Float vs double precision comparison
- Type-specific error conditions
- Performance benchmarking

## Building the Examples

### Prerequisites

- C++17 compatible compiler (GCC 8+, Clang 8+, MSVC 19.0+)
- CMake 3.20+ (if using CMake build)
- The calculator library header file (`calculator.hpp`)

### Method 1: Using CMake (Recommended)

```bash
# From the project root directory
mkdir build && cd build
cmake .. -DBUILD_EXAMPLES=ON
cmake --build .

# Run examples
./example/main                    # Basic usage
./example/advanced_usage          # Advanced patterns
./example/error_handling_demo     # Error handling
./example/numeric_types_demo      # Numeric types
```

### Method 2: Manual Compilation

```bash
# From the example directory
cd example

# Compile individual examples
g++ -std=c++17 -I.. -O2 main.cpp -o main
g++ -std=c++17 -I.. -O2 advanced_usage.cpp -o advanced_usage
g++ -std=c++17 -I.. -O2 error_handling_demo.cpp -o error_handling_demo
g++ -std=c++17 -I.. -O2 numeric_types_demo.cpp -o numeric_types_demo

# Run examples
./main
./advanced_usage
./error_handling_demo
./numeric_types_demo
```

### Method 3: Using Different Compilers

#### GCC

```bash
g++ -std=c++17 -I.. -Wall -Wextra -O2 main.cpp -o main
```

#### Clang

```bash
clang++ -std=c++17 -I.. -Wall -Wextra -O2 main.cpp -o main
```

#### MSVC (Windows)

```cmd
cl /std:c++17 /I.. /EHsc /O2 main.cpp
```

## Running the Examples

### Expected Output

Each example produces detailed output demonstrating the features:

#### `main.cpp` Output Sample

```
=== Modern C++ Calculator Library Examples ===

--- Basic Arithmetic Operations ---
Simple arithmetic:
2 + 3 = 5
10 - 4 = 6
...

--- Variables and Functions ---
Using variables:
x = 5
x + y = 15
...
```

#### `advanced_usage.cpp` Output Sample

```
=== Advanced Usage Examples ===

--- Performance Optimization ---
Parser recreation: 15234 μs
Parser reuse: 2341 μs
Speedup: 6.51x
...
```

### Interactive Usage

You can modify the examples to test your own expressions:

```cpp
// In main.cpp, add your own test cases:
std::vector<std::string> my_expressions = {
    "your_expression_here",
    "another_expression"
};

for (const auto& expr : my_expressions) {
    try {
        double result = calculator::eval(expr);
        std::cout << expr << " = " << result << std::endl;
    } catch (const calculator::error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
```

## Example Use Cases

### Scientific Computing

```cpp
// From advanced_usage.cpp
calculator::ExpressionParser<double> parser;
parser.set("sin", [](double x) { return std::sin(x); });
parser.set("pi", M_PI);

// Numerical integration
double integral = simpson_integrate("sin(x)", 0.0, M_PI);
```

### Financial Calculations

```cpp
// From main.cpp
parser.set("principal", 1000.0);
parser.set("rate", 0.05);
parser.set("time", 10.0);

double compound_amount = parser.eval("principal * (1 + rate) ** time");
```

### Engineering Applications

```cpp
// From main.cpp
parser.set("voltage", 120.0);
parser.set("current", 5.0);

double power = parser.eval("voltage * current");
```

### Game Development

```cpp
// Damage calculation system
parser.set("base_damage", 50.0);
parser.set("level", 15.0);
parser.set("multiplier", 1.2);

double damage = parser.eval("base_damage * (1 + level * 0.1) * multiplier");
```

## Performance Considerations

### Optimization Tips from Examples

1. **Reuse Parser Instances** (from `advanced_usage.cpp`):

   ```cpp
   // Good: Reuse parser
   calculator::ExpressionParser<double> parser;
   for (int i = 0; i < 1000; ++i) {
       parser.set("x", i);
       double result = parser.eval("x * 2 + 1");
   }
   
   // Bad: Create new parser each time
   for (int i = 0; i < 1000; ++i) {
       calculator::ExpressionParser<double> parser;
       parser.set("x", i);
       double result = parser.eval("x * 2 + 1");
   }
   ```

2. **Cache Frequently Used Expressions**:

   ```cpp
   std::unordered_map<std::string, double> cache;
   // Check cache before evaluation
   ```

3. **Choose Appropriate Types** (from `numeric_types_demo.cpp`):
   - Use `int` for counting and bitwise operations
   - Use `float` for graphics when precision allows
   - Use `double` for scientific and financial calculations

## Error Handling Patterns

### Production-Ready Error Handling

```cpp
// From error_handling_demo.cpp
class ProductionCalculator {
    EvaluationResult evaluate(const std::string& expression) const {
        try {
            double result = parser_.eval(expression);
            return {true, result, "", "success", execution_time};
        } catch (const calculator::error& e) {
            return {false, 0.0, e.what(), categorize_error(e.what()), execution_time};
        }
    }
};
```

### Safe Evaluation Functions

```cpp
// From error_handling_demo.cpp
auto safe_eval = [](const std::string& expr, double default_val = 0.0) -> double {
    try {
        return calculator::eval(expr);
    } catch (const calculator::error&) {
        return default_val;
    }
};
```

## Thread Safety

### Thread-Safe Patterns

```cpp
// From advanced_usage.cpp

// Option 1: Thread-local storage
thread_local calculator::ExpressionParser<double> tls_parser;

// Option 2: Parser per thread
auto thread_function = []() {
    calculator::ExpressionParser<double> local_parser;
    // Use local_parser safely
};

// Option 3: Synchronized access
class ThreadSafeCalculator {
    std::mutex mutex_;
    calculator::ExpressionParser<double> parser_;
public:
    double eval(const std::string& expr) {
        std::lock_guard<std::mutex> lock(mutex_);
        return parser_.eval(expr);
    }
};
```

## Troubleshooting

### Common Issues

1. **Compilation Errors**:
   - Ensure C++17 support: Add `-std=c++17` flag
   - Include path: Add `-I..` to find `calculator.hpp`
   - Missing headers: Some examples require `<chrono>`, `<thread>`, etc.

2. **Runtime Errors**:
   - Division by zero: Handle with try-catch
   - Undefined variables: Set all variables before evaluation
   - Type mismatches: Use appropriate numeric types

3. **Performance Issues**:
   - Parser recreation: Reuse parser instances
   - Complex expressions: Consider caching results
   - Memory usage: Use object pooling for many parsers

### Debug Build

```bash
# For debugging, compile with debug symbols
g++ -std=c++17 -I.. -g -O0 -DDEBUG main.cpp -o main_debug
```

## Next Steps

After exploring these examples:

1. **Read the Documentation**: Check `docs/` for comprehensive guides
2. **Try the API Reference**: See `docs/api-reference/` for detailed API docs
3. **Explore Advanced Features**: Look at `docs/user-guide/advanced-features.md`
4. **Contribute**: See `docs/development/contributing.md` for contribution guidelines

## Support

If you encounter issues with these examples:

1. Check the [documentation](../docs/)
2. Review the [API reference](../docs/api-reference/)
3. Look at [common issues](../docs/reference/limitations.md)
4. Open an issue on GitHub

Happy calculating! 🧮
