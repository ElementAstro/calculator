# Modern C++ Calculator

[![CodeQL](https://github.com/ElementAstro/calculator/actions/workflows/codeql.yml/badge.svg)](https://github.com/ElementAstro/calculator/actions/workflows/codeql.yml)
[![MinGW](https://github.com/ElementAstro/calculator/actions/workflows/windows-mingw.yml/badge.svg)](https://github.com/ElementAstro/calculator/actions/workflows/windows-mingw.yml)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=ElementAstro_calculator&metric=bugs)](https://sonarcloud.io/summary/new_code?id=ElementAstro_calculator)
[![Documentation](https://img.shields.io/badge/docs-mkdocs-blue.svg)](https://elementastro.github.io/calculator/)

A modern, high-performance C++ header-only calculator library for evaluating mathematical expressions. Built with C++17/20 features, this library provides a robust and flexible expression parser that supports various arithmetic operations, bitwise operations, variables, and custom functions.

!!! important "Development Status"
    This library is actively maintained and continuously improved. While stable for production use, new features are regularly added.

## ✨ Key Features

- **🚀 Header-only**: Single header file, easy integration
- **🔢 Multiple numeric types**: Support for `int`, `float`, and `double`
- **⚡ High performance**: Optimized expression parsing and evaluation
- **🧮 Comprehensive operations**: Arithmetic, bitwise, and exponentiation
- **📐 Mathematical functions**: Support for custom functions and variables
- **🛡️ Robust error handling**: Detailed error messages with position information
- **📏 Operator precedence**: Follows standard mathematical precedence rules
- **🔧 Template-based**: Type-safe generic implementation

## 🚀 Quick Start

Get started with the calculator in just a few lines:

```cpp
#include "calculator.hpp"
#include <iostream>

int main() {
    // Simple expression evaluation
    double result = calculator::eval("3 + 4 * 2 / (1 - 5) ** 2");
    std::cout << "Result: " << result << std::endl;  // Output: 3.5
    
    // Using variables and functions
    calculator::ExpressionParser<double> parser;
    parser.set("x", 5.0);
    parser.set("square", [](double x) { return x * x; });
    
    double advanced_result = parser.eval("square(x) + 10");
    std::cout << "Advanced result: " << advanced_result << std::endl;  // Output: 35
    
    return 0;
}
```

## 📚 What's Included

### Arithmetic Operations

- **Basic**: `+`, `-`, `*`, `/`, `%`
- **Advanced**: `**` (power), `e`/`E` (scientific notation)
- **Unary**: `+`, `-`, `~` (bitwise NOT)

### Bitwise Operations

- **Logical**: `|` (OR), `^` (XOR), `&` (AND)
- **Shift**: `<<` (left shift), `>>` (right shift)

### Advanced Features

- **Variables**: Store and reuse values
- **Functions**: Custom mathematical functions
- **Parentheses**: Complex expression grouping
- **Type safety**: Template-based type checking

## 🎯 Use Cases

- **Scientific computing**: Mathematical expression evaluation
- **Configuration systems**: Dynamic formula parsing
- **Educational tools**: Teaching mathematical concepts
- **Game development**: Damage calculations, physics formulas
- **Data processing**: Custom metric calculations

## 📖 Documentation Structure

- **[Getting Started](getting-started/)**: Installation and basic setup
- **[User Guide](user-guide/)**: Comprehensive usage instructions
- **[API Reference](api-reference/)**: Complete class and function documentation
- **[Examples](examples/)**: Practical code examples and use cases
- **[Reference](reference/)**: Operator precedence, types, and limitations
- **[Development](development/)**: Building, testing, and contributing

## 🔗 Quick Links

- [Installation Guide](getting-started/installation.md)
- [Quick Start Tutorial](getting-started/quick-start.md)
- [API Reference](api-reference/expression-parser.md)
- [Examples Gallery](examples/basic-examples.md)
- [GitHub Repository](https://github.com/ElementAstro/calculator)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/ElementAstro/calculator/blob/main/LICENSE) file for details.

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](development/contributing.md) for details on how to get started.
