# Testing Guide

This guide covers the testing framework, best practices, and procedures for the Modern C++ Calculator library. Comprehensive testing ensures reliability, correctness, and maintainability of the codebase.

## Testing Framework

### Google Test Integration

The project uses Google Test (gtest) for unit testing:

```cpp
#include <gtest/gtest.h>
#include "calculator.hpp"

// Basic test structure
TEST(TestSuiteName, TestName) {
    // Arrange
    calculator::ExpressionParser<double> parser;
    
    // Act
    double result = parser.eval("2 + 3");
    
    // Assert
    EXPECT_DOUBLE_EQ(result, 5.0);
}
```

### Test Organization

Tests are organized into logical suites:

```cpp
// Test fixture for shared setup
class CalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
    }
    
    void TearDown() override {
        // Cleanup if needed
    }
    
    calculator::ExpressionParser<double> parser_;
};

// Tests using the fixture
TEST_F(CalculatorTest, BasicArithmetic) {
    EXPECT_DOUBLE_EQ(parser_.eval("2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(parser_.eval("10 - 4"), 6.0);
}
```

## Test Categories

### 1. Unit Tests

Test individual components in isolation:

```cpp
// Test basic arithmetic operations
TEST_F(CalculatorTest, ArithmeticOperations) {
    EXPECT_DOUBLE_EQ(parser_.eval("5 + 3"), 8.0);
    EXPECT_DOUBLE_EQ(parser_.eval("5 - 3"), 2.0);
    EXPECT_DOUBLE_EQ(parser_.eval("5 * 3"), 15.0);
    EXPECT_DOUBLE_EQ(parser_.eval("15 / 3"), 5.0);
}

// Test operator precedence
TEST_F(CalculatorTest, OperatorPrecedence) {
    EXPECT_DOUBLE_EQ(parser_.eval("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(parser_.eval("(2 + 3) * 4"), 20.0);
    EXPECT_DOUBLE_EQ(parser_.eval("2 ** 3 ** 2"), 512.0);  // Right associative
}

// Test different numeric types
TEST(CalculatorTypeTest, IntegerOperations) {
    EXPECT_EQ(calculator::eval<int>("10 / 3"), 3);  // Integer division
    EXPECT_EQ(calculator::eval<int>("10 % 3"), 1);  // Modulo
    EXPECT_EQ(calculator::eval<int>("5 & 3"), 1);   // Bitwise AND
}

TEST(CalculatorTypeTest, FloatingPointOperations) {
    EXPECT_FLOAT_EQ(calculator::eval<float>("10.0 / 3.0"), 10.0f / 3.0f);
    EXPECT_DOUBLE_EQ(calculator::eval<double>("10.0 / 3.0"), 10.0 / 3.0);
}
```

### 2. Integration Tests

Test component interactions:

```cpp
// Test variables and functions together
TEST_F(CalculatorTest, VariablesAndFunctions) {
    parser_.set("x", 5.0);
    parser_.set("square", [](double x) { return x * x; });
    
    EXPECT_DOUBLE_EQ(parser_.eval("square(x)"), 25.0);
    EXPECT_DOUBLE_EQ(parser_.eval("square(x) + x"), 30.0);
    EXPECT_DOUBLE_EQ(parser_.eval("sin(pi/2)"), 1.0);
}

// Test complex expressions
TEST_F(CalculatorTest, ComplexExpressions) {
    parser_.set("x", 3.0);
    parser_.set("y", 4.0);
    
    // Pythagorean theorem
    double result = parser_.eval("sqrt(x ** 2 + y ** 2)");
    EXPECT_DOUBLE_EQ(result, 5.0);
    
    // Nested function calls
    result = parser_.eval("sin(cos(pi/4))");
    EXPECT_NEAR(result, std::sin(std::cos(M_PI/4)), 1e-10);
}
```

### 3. Error Handling Tests

Test error conditions and edge cases:

```cpp
// Test syntax errors
TEST_F(CalculatorTest, SyntaxErrors) {
    EXPECT_THROW(parser_.eval("2 + + 3"), calculator::error);
    EXPECT_THROW(parser_.eval("* 5"), calculator::error);
    EXPECT_THROW(parser_.eval("5 *"), calculator::error);
    EXPECT_THROW(parser_.eval("(2 + 3"), calculator::error);
    EXPECT_THROW(parser_.eval("2 + 3)"), calculator::error);
}

// Test runtime errors
TEST_F(CalculatorTest, RuntimeErrors) {
    EXPECT_THROW(parser_.eval("5 / 0"), calculator::error);
    EXPECT_THROW(parser_.eval("10 / (2 - 2)"), calculator::error);
    EXPECT_THROW(parser_.eval("unknown_var"), calculator::error);
    EXPECT_THROW(parser_.eval("unknown_func(5)"), calculator::error);
}

// Test type errors
TEST(CalculatorTypeTest, TypeErrors) {
    EXPECT_THROW(calculator::eval<int>("3.14"), calculator::error);
    EXPECT_THROW(calculator::eval<double>("5.5 & 3.2"), calculator::error);
    EXPECT_THROW(calculator::eval<float>("7.1 % 2.3"), calculator::error);
}

// Test error messages
TEST_F(CalculatorTest, ErrorMessages) {
    try {
        parser_.eval("2 + + 3");
        FAIL() << "Expected calculator::error";
    } catch (const calculator::error& e) {
        std::string error_msg = e.what();
        EXPECT_TRUE(error_msg.find("Unexpected token") != std::string::npos);
    }
}
```

### 4. Edge Case Tests

Test boundary conditions:

```cpp
// Test edge cases
TEST_F(CalculatorTest, EdgeCases) {
    // Zero values
    EXPECT_DOUBLE_EQ(parser_.eval("0"), 0.0);
    EXPECT_DOUBLE_EQ(parser_.eval("-0"), 0.0);
    EXPECT_DOUBLE_EQ(parser_.eval("0 + 0"), 0.0);
    EXPECT_DOUBLE_EQ(parser_.eval("0 * 1000"), 0.0);
    
    // Very small numbers
    EXPECT_DOUBLE_EQ(parser_.eval("1e-10"), 1e-10);
    EXPECT_DOUBLE_EQ(parser_.eval("1e-100"), 1e-100);
    
    // Very large numbers
    EXPECT_DOUBLE_EQ(parser_.eval("1e10"), 1e10);
    EXPECT_DOUBLE_EQ(parser_.eval("1e100"), 1e100);
    
    // Negative numbers
    EXPECT_DOUBLE_EQ(parser_.eval("-5"), -5.0);
    EXPECT_DOUBLE_EQ(parser_.eval("--5"), 5.0);
    EXPECT_DOUBLE_EQ(parser_.eval("-(3 + 4)"), -7.0);
}

// Test integer limits
TEST(CalculatorTypeTest, IntegerLimits) {
    // Test values near integer limits
    EXPECT_EQ(calculator::eval<int>("2147483647"), 2147483647);  // INT_MAX
    EXPECT_EQ(calculator::eval<int>("-2147483648"), -2147483648); // INT_MIN
    
    // Test overflow behavior (implementation-specific)
    // Note: Actual behavior may vary by compiler/platform
}

// Test floating-point special values
TEST_F(CalculatorTest, FloatingPointSpecialValues) {
    // Test infinity (if supported)
    // Test NaN handling (if supported)
    // Test denormal numbers
}
```

### 5. Performance Tests

Test performance characteristics:

```cpp
#include <chrono>

// Performance test for simple expressions
TEST_F(CalculatorTest, SimpleExpressionPerformance) {
    const int iterations = 10000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        parser_.eval("2 + 3 * 4");
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 100000);  // Less than 100ms
    
    std::cout << "Simple expression: " << duration.count() << " μs for " 
              << iterations << " iterations" << std::endl;
}

// Performance test for complex expressions
TEST_F(CalculatorTest, ComplexExpressionPerformance) {
    const int iterations = 1000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        parser_.eval("sin(cos(pi/4)) + sqrt(e ** 2)");
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    EXPECT_LT(duration.count(), 50000);  // Less than 50ms
    
    std::cout << "Complex expression: " << duration.count() << " μs for " 
              << iterations << " iterations" << std::endl;
}
```

## Test Utilities

### Custom Matchers

```cpp
// Custom matcher for floating-point comparison
MATCHER_P(DoubleNear, expected, "is approximately equal to " + std::to_string(expected)) {
    return std::abs(arg - expected) < 1e-10;
}

// Usage
TEST_F(CalculatorTest, CustomMatcher) {
    EXPECT_THAT(parser_.eval("1.0 / 3.0"), DoubleNear(0.3333333333));
}

// Custom matcher for error messages
MATCHER_P(ErrorMessageContains, substring, "error message contains " + substring) {
    try {
        arg();
        return false;  // No exception thrown
    } catch (const calculator::error& e) {
        return std::string(e.what()).find(substring) != std::string::npos;
    }
}

// Usage
TEST_F(CalculatorTest, ErrorMessageMatcher) {
    EXPECT_THAT([&]() { parser_.eval("5 / 0"); }, 
                ErrorMessageContains("Division by zero"));
}
```

### Test Helpers

```cpp
// Helper function for testing multiple expressions
void test_expressions(const std::vector<std::pair<std::string, double>>& test_cases) {
    calculator::ExpressionParser<double> parser;
    
    for (const auto& [expression, expected] : test_cases) {
        EXPECT_DOUBLE_EQ(parser.eval(expression), expected) 
            << "Failed for expression: " << expression;
    }
}

// Usage
TEST(CalculatorTest, MultipleExpressions) {
    test_expressions({
        {"2 + 3", 5.0},
        {"10 - 4", 6.0},
        {"3 * 4", 12.0},
        {"15 / 3", 5.0}
    });
}

// Helper for testing error conditions
void test_error_expressions(const std::vector<std::string>& expressions) {
    calculator::ExpressionParser<double> parser;
    
    for (const auto& expression : expressions) {
        EXPECT_THROW(parser.eval(expression), calculator::error)
            << "Expected error for expression: " << expression;
    }
}

// Usage
TEST(CalculatorTest, MultipleErrors) {
    test_error_expressions({
        "2 + + 3",
        "5 / 0",
        "(2 + 3",
        "unknown_var"
    });
}
```

## Running Tests

### Command Line Options

```bash
# Run all tests
./calc_test

# Run specific test suite
./calc_test --gtest_filter="CalculatorTest.*"

# Run specific test
./calc_test --gtest_filter="CalculatorTest.BasicArithmetic"

# Run tests matching pattern
./calc_test --gtest_filter="*Error*"

# Exclude tests
./calc_test --gtest_filter="-*Performance*"

# Repeat tests
./calc_test --gtest_repeat=10

# Shuffle test order
./calc_test --gtest_shuffle

# Stop on first failure
./calc_test --gtest_break_on_failure

# Verbose output
./calc_test --gtest_verbose

# List available tests
./calc_test --gtest_list_tests
```

### Using CTest

```bash
# Run all tests through CTest
ctest

# Run with verbose output
ctest --verbose

# Run specific test
ctest -R "CalculatorTest"

# Run tests in parallel
ctest --parallel 4

# Run tests with timeout
ctest --timeout 30
```

## Code Coverage

### Generating Coverage Reports

```bash
# Configure with coverage flags
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON

# Build and run tests
make -j$(nproc)
./calc_test

# Generate coverage data
gcov *.gcno

# Create HTML report with lcov
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info  # Remove system files
genhtml coverage.info --output-directory coverage_html

# View report
open coverage_html/index.html  # macOS
xdg-open coverage_html/index.html  # Linux
```

### Coverage Targets

- **Line Coverage**: Aim for >95%
- **Branch Coverage**: Aim for >90%
- **Function Coverage**: Aim for 100%

## Continuous Integration

### GitHub Actions

The project uses GitHub Actions for automated testing:

```yaml
# .github/workflows/test.yml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest, macos-latest]
        compiler: [gcc, clang, msvc]
        
    steps:
    - uses: actions/checkout@v3
    
    - name: Configure CMake
      run: cmake -B build -DBUILD_TESTS=ON
      
    - name: Build
      run: cmake --build build
      
    - name: Test
      run: ctest --test-dir build --verbose
```

## Best Practices

### 1. Test Naming

```cpp
// Good: Descriptive test names
TEST_F(CalculatorTest, ShouldEvaluateBasicArithmeticCorrectly)
TEST_F(CalculatorTest, ShouldThrowErrorForDivisionByZero)
TEST_F(CalculatorTest, ShouldHandleNestedParenthesesCorrectly)

// Bad: Vague test names
TEST_F(CalculatorTest, Test1)
TEST_F(CalculatorTest, BasicTest)
TEST_F(CalculatorTest, ErrorTest)
```

### 2. Test Structure (AAA Pattern)

```cpp
TEST_F(CalculatorTest, ShouldCalculateCompoundInterest) {
    // Arrange
    parser_.set("principal", 1000.0);
    parser_.set("rate", 0.05);
    parser_.set("time", 10.0);
    
    // Act
    double result = parser_.eval("principal * (1 + rate) ** time");
    
    // Assert
    EXPECT_NEAR(result, 1628.89, 0.01);
}
```

### 3. Test Independence

```cpp
// Good: Each test is independent
TEST_F(CalculatorTest, TestA) {
    parser_.set("x", 5.0);
    EXPECT_DOUBLE_EQ(parser_.eval("x"), 5.0);
}

TEST_F(CalculatorTest, TestB) {
    parser_.set("y", 10.0);  // Doesn't depend on TestA
    EXPECT_DOUBLE_EQ(parser_.eval("y"), 10.0);
}
```

### 4. Comprehensive Error Testing

```cpp
// Test all error paths
TEST_F(CalculatorTest, ShouldHandleAllSyntaxErrors) {
    std::vector<std::string> syntax_errors = {
        "",              // Empty expression
        "   ",           // Whitespace only
        "2 + + 3",       // Double operator
        "* 5",           // Missing left operand
        "5 *",           // Missing right operand
        "(2 + 3",        // Unmatched parenthesis
        "2 + 3)",        // Extra closing parenthesis
        "1..2",          // Invalid number format
        "1.2.3"          // Multiple decimal points
    };
    
    for (const auto& expr : syntax_errors) {
        EXPECT_THROW(parser_.eval(expr), calculator::error)
            << "Should throw for: '" << expr << "'";
    }
}
```

## Debugging Tests

### Using GDB

```bash
# Build with debug symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Run specific test under debugger
gdb --args ./calc_test --gtest_filter="CalculatorTest.SpecificTest"

# Set breakpoint and run
(gdb) break calculator.hpp:123
(gdb) run
```

### Using Valgrind

```bash
# Check for memory leaks
valgrind --leak-check=full ./calc_test

# Check for memory errors
valgrind --tool=memcheck ./calc_test
```

## Next Steps

- **[Learn about benchmarking](benchmarking.md)** for performance testing
- **[Read the contributing guide](contributing.md)** for development workflow
- **[Explore the API reference](../api-reference/)** for implementation details
- **[Check the examples](../examples/)** for usage patterns

Happy testing! 🧪
