# API Reference

This section provides comprehensive documentation for all classes, functions, and types in the Modern C++ Calculator library. The API is designed to be simple, type-safe, and efficient.

## Overview

The calculator library provides a clean, template-based API that supports:

- **Type-safe expression evaluation** with multiple numeric types
- **Variable and function management** for dynamic expressions
- **Robust error handling** with detailed error messages
- **High-performance parsing** with optimized algorithms

## Core Components

### Main Classes

| Class | Description | Header |
|-------|-------------|--------|
| [`ExpressionParser<T>`](expression-parser.md) | Template-based expression parser and evaluator | `calculator.hpp` |
| [`calculator::error`](exceptions.md) | Exception class for error handling | `calculator.hpp` |

### Free Functions

| Function | Description | Header |
|----------|-------------|--------|
| [`eval<T>(expression)`](functions.md#eval-template) | Template function for expression evaluation | `calculator.hpp` |
| [`eval(expression)`](functions.md#eval-double) | Double-precision expression evaluation | `calculator.hpp` |

### Type Definitions

| Type | Definition | Purpose |
|------|------------|---------|
| `calculator::error` | Exception class | Error reporting |
| `Operator` | Internal operator representation | Operator handling |
| `OperatorValue` | Internal value-operator pair | Expression parsing |

## Quick API Reference

### Basic Usage

```cpp
#include "calculator.hpp"

// Simple evaluation
double result = calculator::eval("2 + 3 * 4");

// Type-specific evaluation
int int_result = calculator::eval<int>("10 / 2");
float float_result = calculator::eval<float>("3.14 * 2");
```

### Advanced Usage

```cpp
// Create parser instance
calculator::ExpressionParser<double> parser;

// Set variables
parser.set("x", 5.0);
parser.set("pi", 3.14159);

// Set functions
parser.set("square", [](double x) { return x * x; });

// Evaluate expressions
double result = parser.eval("square(x) + pi");
```

### Error Handling

```cpp
try {
    double result = calculator::eval("invalid expression");
} catch (const calculator::error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## Supported Types

The calculator supports the following numeric types:

| Type | Precision | Range | Bitwise Ops | Notes |
|------|-----------|-------|-------------|-------|
| `int` | 32-bit | ±2.1B | ✅ | Integer arithmetic, all operations |
| `float` | 32-bit | ±3.4E±38 | ❌ | Single precision, no bitwise |
| `double` | 64-bit | ±1.7E±308 | ❌ | Double precision, default type |

## Namespace Structure

```cpp
namespace calculator {
    // Main parser class
    template<typename T>
    class ExpressionParser { /* ... */ };
    
    // Exception class
    class error : public std::exception { /* ... */ };
    
    // Free functions
    template<typename T>
    T eval(std::string_view expression);
    
    double eval(const std::string& expression);
}
```

## Thread Safety

!!! warning "Thread Safety"
    The `ExpressionParser` class is **not thread-safe**. If you need to use the same parser instance across multiple threads, you must provide external synchronization. For multi-threaded applications, consider:

    - Creating separate parser instances per thread
    - Using thread-local storage for parser instances
    - Implementing external locking mechanisms

## Performance Characteristics

| Operation | Complexity | Notes |
|-----------|------------|-------|
| Simple expression | O(n) | Linear in expression length |
| Variable lookup | O(1) | Hash table based |
| Function call | O(1) + function cost | Plus user function execution time |
| Parser creation | O(1) | Lightweight construction |

## Memory Usage

- **Parser instance**: ~1KB base overhead
- **Variables**: ~32 bytes per variable
- **Functions**: ~64 bytes per function
- **Expression parsing**: Temporary stack usage proportional to expression depth

## Compatibility

### Compiler Requirements

| Compiler | Minimum Version | Recommended |
|----------|----------------|-------------|
| GCC | 8.0 | 11.0+ |
| Clang | 8.0 | 13.0+ |
| MSVC | 19.0 (VS 2015) | 19.29+ (VS 2019) |

### Standard Library

- **Minimum**: C++17
- **Recommended**: C++20
- **Dependencies**: Standard library only (no external dependencies)

## API Stability

The calculator library follows semantic versioning:

- **Major version**: Breaking API changes
- **Minor version**: New features, backward compatible
- **Patch version**: Bug fixes, no API changes

Current API is considered **stable** for production use.

## Detailed Documentation

For complete documentation of each component:

- **[ExpressionParser Class](expression-parser.md)** - Complete class documentation
- **[Free Functions](functions.md)** - Global evaluation functions
- **[Exception Handling](exceptions.md)** - Error types and handling

## Migration Guide

### From Version 0.x to 1.x

No breaking changes - the API is backward compatible.

### Best Practices for API Usage

1. **Use appropriate types** for your use case
2. **Handle exceptions** properly in production code
3. **Reuse parser instances** when possible for performance
4. **Validate user input** before evaluation
5. **Use const-correctness** where applicable

## Examples

For practical examples of API usage, see:

- **[Basic Examples](../examples/basic-examples.md)** - Simple use cases
- **[Advanced Examples](../examples/advanced-examples.md)** - Complex scenarios
- **[Real-world Usage](../examples/real-world-usage.md)** - Production patterns
