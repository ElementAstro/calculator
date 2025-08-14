# Development Guide

Welcome to the development guide for the Modern C++ Calculator library. This section provides comprehensive information for contributors, maintainers, and developers who want to extend or modify the library.

## Getting Started with Development

### Prerequisites

Before contributing to the calculator library, ensure you have:

- **C++ Compiler**: GCC 8.0+, Clang 8.0+, or MSVC 19.0+
- **CMake**: Version 3.20 or later
- **Git**: For version control
- **Code Editor**: VS Code, CLion, or similar with C++ support

### Development Environment Setup

1. **Clone the repository**:

   ```bash
   git clone https://github.com/ElementAstro/calculator.git
   cd calculator
   ```

2. **Set up build environment**:

   ```bash
   mkdir build && cd build
   cmake .. -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DBUILD_BENCHMARKS=ON
   cmake --build .
   ```

3. **Run tests to verify setup**:

   ```bash
   ./calc_test  # Linux/macOS
   .\calc_test.exe  # Windows
   ```

## Project Structure

```
calculator/
├── calculator.hpp          # Main header file (single-file library)
├── example/                # Example programs
│   ├── main.cpp           # Basic usage examples
│   └── advanced/          # Advanced usage examples
├── test/                  # Unit tests
│   └── test_calculator.cpp
├── benchmark/             # Performance benchmarks
│   └── benchmark.cpp
├── docs/                  # Documentation source
├── .github/               # GitHub workflows and templates
├── CMakeLists.txt         # Build configuration
├── README.md              # Project overview
└── LICENSE                # MIT license
```

## Development Workflow

### 1. Issue-Driven Development

All changes should be associated with an issue:

1. **Check existing issues** on GitHub
2. **Create new issue** if needed with:
   - Clear description of problem/feature
   - Expected behavior
   - Current behavior (for bugs)
   - Minimal reproduction case

### 2. Branch Strategy

```bash
# Create feature branch
git checkout -b feature/your-feature-name

# Create bugfix branch
git checkout -b bugfix/issue-number-description

# Create documentation branch
git checkout -b docs/documentation-improvement
```

### 3. Development Process

1. **Write tests first** (TDD approach recommended)
2. **Implement feature/fix**
3. **Update documentation**
4. **Run all tests and benchmarks**
5. **Submit pull request**

## Code Standards

### C++ Style Guidelines

The project follows modern C++ best practices:

```cpp
// Good: Modern C++ style
class ExpressionParser {
private:
    std::unordered_map<std::string, double> variables_;
    std::unordered_map<std::string, std::function<double(double)>> functions_;
    
public:
    // Use explicit constructors
    explicit ExpressionParser() = default;
    
    // Use const correctness
    double eval(std::string_view expression) const;
    
    // Use move semantics where appropriate
    void set(std::string name, double value) {
        variables_[std::move(name)] = value;
    }
    
    // Use RAII and smart pointers
    std::unique_ptr<Node> parse(std::string_view expr);
};
```

### Naming Conventions

- **Classes**: PascalCase (`ExpressionParser`)
- **Functions/Methods**: snake_case (`eval`, `set_variable`)
- **Variables**: snake_case (`result_value`, `error_message`)
- **Constants**: UPPER_SNAKE_CASE (`MAX_RECURSION_DEPTH`)
- **Private members**: trailing underscore (`variables_`, `functions_`)

### Documentation Standards

All public APIs must be documented:

```cpp
/**
 * @brief Evaluates a mathematical expression
 * @param expression The expression to evaluate (e.g., "2 + 3 * 4")
 * @return The result of the evaluation
 * @throws calculator::error If the expression is invalid
 * 
 * @example
 * ```cpp
 * double result = parser.eval("sin(pi/2)");  // Returns 1.0
 * ```
 */
double eval(std::string_view expression);
```

## Testing Guidelines

### Unit Testing Framework

The project uses Google Test for unit testing:

```cpp
#include <gtest/gtest.h>
#include "calculator.hpp"

class CalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser.set("pi", M_PI);
        parser.set("sin", [](double x) { return std::sin(x); });
    }
    
    calculator::ExpressionParser<double> parser;
};

TEST_F(CalculatorTest, BasicArithmetic) {
    EXPECT_DOUBLE_EQ(parser.eval("2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("10 / 2"), 5.0);
}

TEST_F(CalculatorTest, ErrorHandling) {
    EXPECT_THROW(parser.eval("2 + + 3"), calculator::error);
    EXPECT_THROW(parser.eval("5 / 0"), calculator::error);
}
```

### Test Categories

1. **Unit Tests**: Test individual components
2. **Integration Tests**: Test component interactions
3. **Error Tests**: Test error conditions and edge cases
4. **Performance Tests**: Ensure performance requirements

### Running Tests

```bash
# Build and run all tests
cmake --build . --target calc_test
./calc_test

# Run specific test suite
./calc_test --gtest_filter="CalculatorTest.*"

# Run with verbose output
./calc_test --gtest_verbose
```

## Performance Considerations

### Benchmarking

Use Google Benchmark for performance testing:

```cpp
#include <benchmark/benchmark.h>
#include "calculator.hpp"

static void BM_SimpleExpression(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("2 + 3 * 4");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SimpleExpression);

static void BM_ComplexExpression(benchmark::State& state) {
    calculator::ExpressionParser<double> parser;
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("pi", M_PI);
    
    for (auto _ : state) {
        double result = parser.eval("sin(pi/2) + cos(0)");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ComplexExpression);
```

### Performance Guidelines

1. **Minimize allocations** in hot paths
2. **Use string_view** for read-only string parameters
3. **Prefer stack allocation** over heap when possible
4. **Profile before optimizing** using tools like Valgrind or Intel VTune

## Architecture Overview

### Core Components

```cpp
// Simplified architecture overview
namespace calculator {
    // Main parser class
    template<typename T>
    class ExpressionParser {
        // Token parsing
        std::vector<Token> tokenize(std::string_view expr);
        
        // Expression parsing (recursive descent)
        std::unique_ptr<Node> parse_expression();
        std::unique_ptr<Node> parse_term();
        std::unique_ptr<Node> parse_factor();
        
        // Evaluation
        T evaluate(const Node& node);
    };
    
    // Error handling
    class error : public std::exception {
        // Error reporting with position information
    };
    
    // AST nodes
    struct Node { virtual ~Node() = default; };
    struct NumberNode : Node { T value; };
    struct BinaryOpNode : Node { /* ... */ };
    struct UnaryOpNode : Node { /* ... */ };
    struct VariableNode : Node { /* ... */ };
    struct FunctionNode : Node { /* ... */ };
}
```

### Design Principles

1. **Header-only**: Single file for easy integration
2. **Template-based**: Type-safe for different numeric types
3. **Exception-safe**: RAII and proper error handling
4. **Extensible**: Support for custom variables and functions
5. **Performance**: Optimized for common use cases

## Contributing Guidelines

### Pull Request Process

1. **Fork the repository** and create your branch
2. **Make your changes** following the coding standards
3. **Add/update tests** for your changes
4. **Update documentation** if needed
5. **Ensure all tests pass**
6. **Submit pull request** with:
   - Clear description of changes
   - Reference to related issues
   - Test results

### Code Review Checklist

- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] Documentation is updated
- [ ] No breaking changes (or properly documented)
- [ ] Performance impact is acceptable
- [ ] Error handling is appropriate

### Commit Message Format

```
type(scope): brief description

Longer description if needed

Fixes #123
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

## Release Process

### Version Numbering

The project follows [Semantic Versioning](https://semver.org/):

- **MAJOR**: Breaking API changes
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes, no API changes

### Release Checklist

1. [ ] Update version numbers
2. [ ] Update CHANGELOG.md
3. [ ] Run full test suite
4. [ ] Update documentation
5. [ ] Create release tag
6. [ ] Publish release notes

## Development Tools

### Recommended IDEs

- **Visual Studio Code** with C++ extension
- **CLion** by JetBrains
- **Visual Studio** 2019/2022
- **Qt Creator**

### Useful Tools

- **Clang-format**: Code formatting
- **Clang-tidy**: Static analysis
- **Valgrind**: Memory debugging (Linux)
- **AddressSanitizer**: Memory error detection
- **Compiler Explorer**: Online compiler testing

### CI/CD Pipeline

The project uses GitHub Actions for:

- **Continuous Integration**: Build and test on multiple platforms
- **Code Quality**: Static analysis with SonarCloud
- **Security**: CodeQL analysis
- **Documentation**: Automatic documentation deployment

## Getting Help

### Resources

- **[API Reference](../api-reference/)**: Complete technical documentation
- **[Examples](../examples/)**: Practical usage examples
- **[GitHub Issues](https://github.com/ElementAstro/calculator/issues)**: Bug reports and feature requests
- **[GitHub Discussions](https://github.com/ElementAstro/calculator/discussions)**: Community support

### Communication

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For questions and general discussion
- **Pull Requests**: For code contributions
- **Email**: For security issues or private matters

## Next Steps

Ready to contribute? Here's what to do next:

1. **[Set up your development environment](building-from-source.md)**
2. **[Learn about testing](testing.md)**
3. **[Understand benchmarking](benchmarking.md)**
4. **[Read the contributing guide](contributing.md)**
5. **[Check open issues](https://github.com/ElementAstro/calculator/issues)**

Welcome to the calculator development community! 🚀
