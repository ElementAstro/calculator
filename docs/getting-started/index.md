# Getting Started

Welcome to the Modern C++ Calculator library! This section will help you get up and running quickly with the calculator in your C++ projects.

## Overview

The Modern C++ Calculator is a header-only library that provides powerful mathematical expression evaluation capabilities. Whether you're building a scientific application, a configuration system, or just need to evaluate mathematical expressions dynamically, this library has you covered.

## What You'll Learn

In this section, you'll discover:

- **[Installation](installation.md)**: How to integrate the calculator into your project
- **[Quick Start](quick-start.md)**: Your first calculator program in minutes
- **[Building](building.md)**: How to build examples, tests, and benchmarks

## Prerequisites

Before you begin, ensure you have:

- **C++ Compiler**: GCC 8.0+, Clang 8.0+, or MSVC 19.0+
- **C++ Standard**: C++17 or later (C++20 recommended)
- **CMake**: 3.20+ (optional, for building examples and tests)

## Quick Installation

The fastest way to get started:

1. **Download the header file**:

   ```bash
   wget https://raw.githubusercontent.com/ElementAstro/calculator/main/calculator.hpp
   ```

2. **Include in your project**:

   ```cpp
   #include "calculator.hpp"
   ```

3. **Start calculating**:

   ```cpp
   double result = calculator::eval("2 + 3 * 4");
   ```

That's it! You're ready to use the calculator.

## Next Steps

- Follow the [Quick Start Guide](quick-start.md) for a hands-on tutorial
- Check out the [Installation Guide](installation.md) for detailed setup instructions
- Explore the [User Guide](../user-guide/) for comprehensive usage examples

## Need Help?

- Browse the [Examples](../examples/) for practical use cases
- Check the [API Reference](../api-reference/) for detailed documentation
- Visit our [GitHub Issues](https://github.com/ElementAstro/calculator/issues) for support
