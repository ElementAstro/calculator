# Modern C++ Calculator

[![CodeQL](https://github.com/ElementAstro/calculator/actions/workflows/codeql.yml/badge.svg)](https://github.com/ElementAstro/calculator/actions/workflows/codeql.yml)
[![MinGW](https://github.com/ElementAstro/calculator/actions/workflows/windows-mingw.yml/badge.svg)](https://github.com/ElementAstro/calculator/actions/workflows/windows-mingw.yml)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=ElementAstro_calculator&metric=bugs)](https://sonarcloud.io/summary/new_code?id=ElementAstro_calculator)
[![Documentation](https://img.shields.io/badge/docs-mkdocs-blue.svg)](https://elementastro.github.io/calculator/)

A modern, high-performance C++ header-only calculator library for evaluating mathematical expressions. Built with C++17/20 features, this library provides a robust and flexible expression parser that supports variables, functions, and comprehensive mathematical operations.

📖 **[Complete Documentation](https://elementastro.github.io/calculator/)** | 🚀 **[Quick Start Guide](https://elementastro.github.io/calculator/getting-started/quick-start/)** | 📋 **[API Reference](https://elementastro.github.io/calculator/api-reference/)**

## ✨ Key Features

- **🚀 Header-only**: Single file, easy integration
- **🔢 Multiple numeric types**: Support for `int`, `float`, and `double`
- **⚡ High performance**: Optimized expression parsing and evaluation
- **🧮 Comprehensive operations**: Arithmetic, bitwise, and exponentiation
- **📐 Mathematical functions**: Support for custom functions and variables
- **🛡️ Robust error handling**: Detailed error messages with position information
- **📏 Operator precedence**: Follows standard mathematical precedence rules
- **🔧 Template-based**: Type-safe generic implementation

## 🚀 Quick Start

### Installation

Simply download and include the header file:

```bash
curl -O https://raw.githubusercontent.com/ElementAstro/calculator/main/calculator.hpp
```

### Basic Usage

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

**[📖 See complete examples and tutorials →](https://elementastro.github.io/calculator/getting-started/quick-start/)**

## 📚 Supported Operations

| Category | Operators | Example | Types |
|----------|-----------|---------|-------|
| **Arithmetic** | `+`, `-`, `*`, `/`, `%` | `3 + 4 * 2` | All |
| **Bitwise** | `\|`, `^`, `&`, `<<`, `>>`, `~` | `5 & 3` | `int` only |
| **Exponentiation** | `**`, `e`/`E` | `2 ** 3`, `1.5e2` | All |
| **Unary** | `+`, `-`, `~` | `+5`, `-3`, `~7` | All/`int` |
| **Grouping** | `(`, `)` | `(2 + 3) * 4` | All |

### Advanced Features

- **Variables**: `parser.set("x", 5.0)` → `"x + 10"`
- **Functions**: `parser.set("sqrt", [](double x) { return std::sqrt(x); })` → `"sqrt(16)"`
- **Multiple Types**: `int`, `float`, `double` with type-specific behaviors
- **Error Handling**: Detailed error messages with position information

**[📋 Complete operator reference and examples →](https://elementastro.github.io/calculator/reference/operators/)**

## 🎯 Use Cases

- **Scientific Computing**: Mathematical expression evaluation with variables and functions
- **Configuration Systems**: Dynamic formula parsing for application settings
- **Game Development**: Damage calculations, physics formulas, procedural generation
- **Financial Applications**: Interest calculations, custom metrics, business rules
- **Educational Tools**: Teaching mathematical concepts and expression evaluation

**[🔍 See real-world examples →](https://elementastro.github.io/calculator/examples/real-world-usage/)**

## 📖 Documentation

| Section | Description |
|---------|-------------|
| **[Getting Started](https://elementastro.github.io/calculator/getting-started/)** | Installation, quick start, and building |
| **[User Guide](https://elementastro.github.io/calculator/user-guide/)** | Comprehensive usage instructions |
| **[API Reference](https://elementastro.github.io/calculator/api-reference/)** | Complete class and function documentation |
| **[Examples](https://elementastro.github.io/calculator/examples/)** | Practical code examples and patterns |
| **[Reference](https://elementastro.github.io/calculator/reference/)** | Operators, types, and limitations |

## 🔧 Requirements

- **C++ Standard**: C++17 minimum, C++20 recommended
- **Compilers**: GCC 8+, Clang 8+, MSVC 19+ (VS 2015+)
- **Dependencies**: None (header-only, standard library only)
- **Platforms**: Linux, Windows, macOS, FreeBSD

## 📄 License

This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details.

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](https://elementastro.github.io/calculator/development/contributing/) for details on how to get started.

- 🐛 [Report bugs](https://github.com/ElementAstro/calculator/issues)
- 💡 [Request features](https://github.com/ElementAstro/calculator/issues)
- 🔧 [Submit pull requests](https://github.com/ElementAstro/calculator/pulls)
- 💬 [Join discussions](https://github.com/ElementAstro/calculator/discussions)

---

**⭐ Star this repository if you find it useful!**
