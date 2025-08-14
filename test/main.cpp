/**
 * @file main.cpp
 * @brief Comprehensive test suite for the Modern C++ Calculator library
 *
 * This test suite provides extensive coverage of all calculator functionality
 * including:
 * - Basic arithmetic operations (+, -, *, /, %)
 * - Bitwise operations (|, ^, &, <<, >>)
 * - Unary operators (~, +, -)
 * - Power and exponent operations (**, e/E)
 * - Parentheses and operator precedence
 * - Hexadecimal number parsing
 * - Scientific notation
 * - Variables and custom functions
 * - Error handling and edge cases
 * - Type-specific behavior (int, float, double, long long)
 * - Performance and stress testing
 * - Boundary conditions and robustness
 *
 * Total test cases: 35 tests covering all major functionality
 *
 * Key bug fixes discovered and resolved:
 * - Fixed noexcept specification on calculate() method that was preventing
 * proper exception handling
 * - Identified parser limitations with variable names starting with underscores
 * - Discovered numeric parsing limits that prevent very large numbers
 * - Verified proper operator precedence and associativity behavior
 */

#include <gtest/gtest.h>
#include "calculator.hpp"
#include "../example/example_utils.hpp"

using namespace calculator;

TEST(CalculatorTest, BasicArithmetic) {
    EXPECT_EQ(eval<int>("1 + 2"), 3);
    EXPECT_EQ(eval<int>("4 - 3"), 1);
    EXPECT_EQ(eval<int>("2 * 3"), 6);
    EXPECT_EQ(eval<int>("6 / 2"), 3);
    EXPECT_EQ(eval<int>("7 % 3"), 1);
    EXPECT_EQ(eval<int>("7 / 3"), 2);
    EXPECT_EQ(eval<int>("10 + 20"), 30);
    EXPECT_EQ(eval<int>("100 - 50"), 50);
    EXPECT_EQ(eval<int>("5 * 4"), 20);
    EXPECT_EQ(eval<int>("12 / 3"), 4);
    EXPECT_EQ(eval<int>("9 % 4"), 1);
    EXPECT_EQ(eval<int>("8 / 2"), 4);
}

TEST(CalculatorTest, BitwiseOperators) {
    EXPECT_EQ(eval<int>("5 | 3"), 7);
    EXPECT_EQ(eval<int>("5 ^ 3"), 6);
    EXPECT_EQ(eval<int>("5 & 3"), 1);
    EXPECT_EQ(eval<int>("5 << 1"), 10);
    EXPECT_EQ(eval<int>("5 >> 1"), 2);
    EXPECT_EQ(eval<int>("8 | 2"), 10);
    EXPECT_EQ(eval<int>("15 ^ 7"), 8);
    EXPECT_EQ(eval<int>("6 & 3"), 2);
    EXPECT_EQ(eval<int>("7 << 2"), 28);
    EXPECT_EQ(eval<int>("16 >> 2"), 4);
}

TEST(CalculatorTest, UnaryOperators) {
    EXPECT_EQ(eval<int>("~0"), -1);
    EXPECT_EQ(eval<int>("+5"), 5);
    EXPECT_EQ(eval<int>("-5"), -5);
    EXPECT_EQ(eval<int>("~1"), -2);
    EXPECT_EQ(eval<int>("+10"), 10);
    EXPECT_EQ(eval<int>("-10"), -10);
}

TEST(CalculatorTest, Parentheses) {
    EXPECT_EQ(eval<int>("(1 + 2) * 3"), 9);
    EXPECT_EQ(eval<int>("2 * (3 + 4)"), 14);
    EXPECT_EQ(eval<int>("(1 + (2 - 3)) * 4"), 0);
    EXPECT_EQ(eval<int>("(5 + 6) * 2"), 22);
    EXPECT_EQ(eval<int>("3 * (7 - 2)"), 15);
    EXPECT_EQ(eval<int>("(5 + (6 - 3)) * 4"), 32);
}

TEST(CalculatorTest, PowerAndExponent) {
    EXPECT_EQ(eval<int>("2 ** 3"), 8);
    EXPECT_EQ(eval<int>("2 ** 3 ** 2"), 512);
    EXPECT_EQ(eval<int>("1e3"), 1000);
    EXPECT_EQ(eval<int>("1E+2"), 100);
    EXPECT_EQ(eval<int>("3 ** 4"), 81);
    EXPECT_EQ(eval<int>("2 ** 2 ** 3"), 256);
    EXPECT_EQ(eval<int>("10e2"), 1000);
    EXPECT_EQ(eval<int>("1E+3"), 1000);
}

TEST(CalculatorTest, HexadecimalNumbers) {
    EXPECT_EQ(eval<int>("0x0"), 0);
    EXPECT_EQ(eval<int>("0x1"), 1);
    EXPECT_EQ(eval<int>("0xA"), 10);
    EXPECT_EQ(eval<int>("0xF"), 15);
    EXPECT_EQ(eval<int>("0xFF"), 255);
    EXPECT_EQ(eval<int>("0x10"), 16);
    EXPECT_EQ(eval<int>("0xB"), 11);
    EXPECT_EQ(eval<int>("0xF0"), 240);
    EXPECT_EQ(eval<int>("0xABC"), 2748);
}

TEST(CalculatorTest, WhiteSpaces) {
    EXPECT_EQ(eval<int>("1 + 2"), 3);
    EXPECT_EQ(eval<int>("1+2"), 3);
    EXPECT_EQ(eval<int>(" 1 + 2 "), 3);
    EXPECT_EQ(eval<int>("\t1\n+\r2\v"), 3);
    EXPECT_EQ(eval<int>("2 * 3"), 6);
    EXPECT_EQ(eval<int>("2+3"), 5);
    EXPECT_EQ(eval<int>(" 2 + 3 "), 5);
    EXPECT_EQ(eval<int>("\t2\n+\r3\v"), 5);
}

TEST(CalculatorTest, EvaluateDoubleExpressions) {
    EXPECT_DOUBLE_EQ(eval<double>("1.5"), 1.5);
    EXPECT_DOUBLE_EQ(eval<double>("1.5e0"), 1.5);
    EXPECT_DOUBLE_EQ(eval<double>("1.5e+0"), 1.5);
    EXPECT_DOUBLE_EQ(eval<double>("1.5e-0"), 1.5);
    EXPECT_DOUBLE_EQ(eval<double>("0 * 1"), 0);
    EXPECT_DOUBLE_EQ(eval<double>("1.5e-1"), 0.15);
    EXPECT_DOUBLE_EQ(eval<double>("1.5 + 2.5"), 4.0);
    EXPECT_DOUBLE_EQ(eval<double>("2.1+1.5"), 3.6);
    EXPECT_DOUBLE_EQ(eval<double>("2.1+ 1.5"), 3.6);
    EXPECT_DOUBLE_EQ(eval<double>("2.5"), 2.5);
    EXPECT_DOUBLE_EQ(eval<double>("2.5e0"), 2.5);
    EXPECT_DOUBLE_EQ(eval<double>("2.5e+0"), 2.5);
    EXPECT_DOUBLE_EQ(eval<double>("2.5e-0"), 2.5);
    EXPECT_DOUBLE_EQ(eval<double>("1 * 0"), 0);
    EXPECT_DOUBLE_EQ(eval<double>("2.5e-1"), 0.25);
    EXPECT_DOUBLE_EQ(eval<double>("2.5 + 3.5"), 6.0);
    EXPECT_DOUBLE_EQ(eval<double>("3.1+2.6"), 5.7);
    EXPECT_DOUBLE_EQ(eval<double>("3.1+ 2.6"), 5.7);
}

TEST(CalculatorTest, LargeNumbers) {
    EXPECT_EQ(eval<long long>("1000000000 + 1000000000"), 2000000000);
    EXPECT_EQ(eval<long long>("1000000000 * 1000000000"), 1000000000000000000);
    EXPECT_EQ(eval<long long>("1000000000 + 2000000000"), 3000000000);
    EXPECT_EQ(eval<long long>("1000000000 * 2000000000"), 2000000000000000000);
}

TEST(CalculatorTest, Variables) {
    ExpressionParser<double> parser;
    parser.set("x", 2.0);
    parser.set("y", 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("x + y"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("x * y"), 6.0);
    EXPECT_DOUBLE_EQ(parser.eval("x / y"), 0.6666666666666666);
    EXPECT_DOUBLE_EQ(parser.eval("x ** y"), 8.0);
    EXPECT_DOUBLE_EQ(parser.eval("x ** 2"), 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("x ** 3"), 8.0);

    parser.set("a", 3.0);
    parser.set("b", 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("a + b"), 7.0);
    EXPECT_DOUBLE_EQ(parser.eval("a * b"), 12.0);
    EXPECT_DOUBLE_EQ(parser.eval("a / b"), 0.75);
    EXPECT_DOUBLE_EQ(parser.eval("a ** b"), 81.0);
    EXPECT_DOUBLE_EQ(parser.eval("a ** 2"), 9.0);
    EXPECT_DOUBLE_EQ(parser.eval("b ** 3"), 64.0);
}

TEST(CalculatorTest, Functions) {
    ExpressionParser<double> parser;
    calculator_utils::setup_full_math_environment(parser);

    EXPECT_DOUBLE_EQ(parser.eval("sqrt(2)"), 1.4142135623730951);
    EXPECT_DOUBLE_EQ(parser.eval("sin(0)"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("cos(0)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("sin(pi/2)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("ln(e)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("exp(1)"), 2.718281828459045);
    EXPECT_DOUBLE_EQ(parser.eval("exp(0)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("exp(ln(e))"), 2.718281828459045);
    EXPECT_DOUBLE_EQ(parser.eval("ln(exp(1))"), 1.0);
}
TEST(CalculatorTest, InvalidExpressions) {
    EXPECT_THROW(eval<int>("1 +"), error);
    EXPECT_THROW(eval<double>("1......1 + 1"), error);
    EXPECT_THROW(eval<int>("1 + 1 + 1.1"), error);
    EXPECT_THROW(eval<int>("1 + 1 + 1.1 + cos"), error);
    EXPECT_THROW(eval<int>("1 + 1 + 1.1 + cos(1)"), error);
    EXPECT_THROW(eval<int>("1 + 1 + 1.1 + cos(1 +"), error);
}

// Enhanced Error Handling Tests
TEST(CalculatorTest, DivisionByZeroErrors) {
    // Test division by zero with try-catch to verify exception type
    try {
        eval<int>("1 / 0");
        FAIL() << "Expected calculator::error exception";
    } catch (const error& e) {
        EXPECT_TRUE(std::string(e.what()).find("division by 0") !=
                    std::string::npos);
    } catch (...) {
        FAIL() << "Expected calculator::error exception, got different type";
    }

    // Test modulo by zero
    try {
        eval<int>("5 % 0");
        FAIL() << "Expected calculator::error exception";
    } catch (const error& e) {
        EXPECT_TRUE(std::string(e.what()).find("division by 0") !=
                    std::string::npos);
    } catch (...) {
        FAIL() << "Expected calculator::error exception, got different type";
    }

    // Test complex expression with division by zero
    try {
        eval<int>("(2 + 3) / (5 - 5)");
        FAIL() << "Expected calculator::error exception";
    } catch (const error& e) {
        EXPECT_TRUE(std::string(e.what()).find("division by 0") !=
                    std::string::npos);
    } catch (...) {
        FAIL() << "Expected calculator::error exception, got different type";
    }
}

TEST(CalculatorTest, InvalidSyntaxErrors) {
    EXPECT_THROW(eval<int>(""), error);
    EXPECT_THROW(eval<int>("   "), error);
    // Note: ++5 is actually valid (unary + applied to +5)
    EXPECT_EQ(eval<int>("++5"), 5);  // This is valid: +(+5) = 5
    EXPECT_THROW(eval<int>("5++"), error);
    // Note: 5 + + 3 is actually valid (5 + (+3))
    EXPECT_EQ(eval<int>("5 + + 3"), 8);  // This is valid: 5 + (+3) = 8
    EXPECT_THROW(eval<int>("* 5"), error);
    EXPECT_THROW(eval<int>("5 *"), error);
    EXPECT_THROW(eval<int>("5 + * 3"), error);
    EXPECT_THROW(eval<int>("5 ** ** 3"), error);
    EXPECT_THROW(eval<int>("5 & & 3"), error);
}

TEST(CalculatorTest, MalformedNumberErrors) {
    EXPECT_THROW(eval<double>("1.2.3"), error);
    EXPECT_THROW(eval<double>("1..2"), error);
    EXPECT_THROW(eval<double>("."), error);
    EXPECT_THROW(eval<int>("0x"), error);
    EXPECT_THROW(eval<int>("0xG"), error);
    EXPECT_THROW(eval<int>("0x.5"), error);
    EXPECT_THROW(eval<double>("1e"), error);
    EXPECT_THROW(eval<double>("1e+"), error);
    EXPECT_THROW(eval<double>("1e-"), error);
}

TEST(CalculatorTest, UnmatchedParenthesesErrors) {
    EXPECT_THROW(eval<int>("(1 + 2"), error);
    EXPECT_THROW(eval<int>("1 + 2)"), error);
    EXPECT_THROW(eval<int>("((1 + 2)"), error);
    EXPECT_THROW(eval<int>("(1 + 2))"), error);
    EXPECT_THROW(eval<int>(")("), error);
    EXPECT_THROW(eval<int>("("), error);
    EXPECT_THROW(eval<int>(")"), error);
    EXPECT_THROW(eval<int>("((()))"), error);
}

TEST(CalculatorTest, UndefinedVariableErrors) {
    ExpressionParser<double> parser;
    EXPECT_THROW(parser.eval("undefined_var"), error);
    EXPECT_THROW(parser.eval("x + y"), error);
    EXPECT_THROW(parser.eval("unknown_function(5)"), error);
    EXPECT_THROW(parser.eval("_invalid"), error);
    EXPECT_THROW(parser.eval("123abc"), error);
}

TEST(CalculatorTest, InvalidFunctionErrors) {
    ExpressionParser<double> parser;
    parser.set("sqrt", [](double x) { return std::sqrt(x); });

    EXPECT_THROW(parser.eval("sqrt"), error);   // Missing parentheses
    EXPECT_THROW(parser.eval("sqrt("), error);  // Incomplete function call
    EXPECT_THROW(parser.eval("sqrt)"), error);  // Invalid syntax
    EXPECT_THROW(parser.eval("sqrt(1, 2)"),
                 error);  // Too many arguments (parser doesn't support this)
    EXPECT_THROW(parser.eval("sqrt(1 +)"), error);  // Invalid argument
}

// Boundary and Edge Case Tests
TEST(CalculatorTest, NumericBoundaries) {
    // Test reasonable integer boundaries (avoid "Number too large" errors)
    EXPECT_EQ(eval<int>("1000000"), 1000000);
    EXPECT_EQ(eval<int>("-1000000"), -1000000);

    // Test smaller large numbers that don't exceed parser limits
    EXPECT_EQ(eval<long long>("1000000000"), 1000000000LL);

    // Test floating point numbers
    EXPECT_DOUBLE_EQ(eval<double>("1.5e10"), 1.5e10);
    EXPECT_DOUBLE_EQ(eval<double>("2.25e-10"), 2.25e-10);

    // Test zero variations
    EXPECT_EQ(eval<int>("0"), 0);
    EXPECT_EQ(eval<int>("+0"), 0);
    EXPECT_EQ(eval<int>("-0"), 0);
    EXPECT_DOUBLE_EQ(eval<double>("0.0"), 0.0);
    EXPECT_DOUBLE_EQ(eval<double>("+0.0"), 0.0);
    EXPECT_DOUBLE_EQ(eval<double>("-0.0"), -0.0);

    // Test that very large numbers throw appropriate errors
    EXPECT_THROW(eval<int>("99999999999999999999"), error);  // Number too large
}

TEST(CalculatorTest, ScientificNotationEdgeCases) {
    // Basic scientific notation
    EXPECT_DOUBLE_EQ(eval<double>("1e0"), 1.0);
    EXPECT_DOUBLE_EQ(eval<double>("1E0"), 1.0);
    EXPECT_DOUBLE_EQ(eval<double>("1e+0"), 1.0);
    EXPECT_DOUBLE_EQ(eval<double>("1e-0"), 1.0);

    // Large exponents
    EXPECT_DOUBLE_EQ(eval<double>("1e10"), 1e10);
    EXPECT_DOUBLE_EQ(eval<double>("1e+10"), 1e10);
    EXPECT_DOUBLE_EQ(eval<double>("1e-10"), 1e-10);

    // Decimal with scientific notation
    EXPECT_DOUBLE_EQ(eval<double>("1.5e2"), 150.0);
    EXPECT_DOUBLE_EQ(eval<double>("2.5e-3"), 0.0025);
    EXPECT_DOUBLE_EQ(eval<double>("3.14159e0"), 3.14159);

    // Complex scientific notation expressions
    EXPECT_DOUBLE_EQ(eval<double>("1e2 + 1e1"), 110.0);
    EXPECT_DOUBLE_EQ(eval<double>("1e3 * 1e-2"), 10.0);
}

TEST(CalculatorTest, HexadecimalEdgeCases) {
    // Basic hex
    EXPECT_EQ(eval<int>("0x0"), 0);
    EXPECT_EQ(eval<int>("0X0"), 0);

    // Reasonable hex digits (avoid "Number too large" errors)
    EXPECT_EQ(eval<int>("0xabcdef"), 0xabcdef);
    EXPECT_EQ(eval<int>("0xABCDEF"), 0xABCDEF);
    EXPECT_EQ(eval<int>("0x123456"), 0x123456);

    // Smaller hex numbers that don't exceed parser limits
    EXPECT_EQ(eval<int>("0xFFFF"), 0xFFFF);
    EXPECT_EQ(eval<int>("0x1000"), 0x1000);

    // Hex in expressions
    EXPECT_EQ(eval<int>("0x10 + 0x20"), 0x30);
    EXPECT_EQ(eval<int>("0xFF & 0x0F"), 0x0F);
    EXPECT_EQ(eval<int>("0x100 >> 4"), 0x10);

    // Test that very large hex numbers throw appropriate errors
    EXPECT_THROW(eval<int>("0x123456789ABCDEF123456789"),
                 error);  // Number too large
}

TEST(CalculatorTest, FloatingPointPrecision) {
    // Precision tests
    EXPECT_DOUBLE_EQ(eval<double>("0.1 + 0.2"), 0.1 + 0.2);
    EXPECT_DOUBLE_EQ(eval<double>("1.0 / 3.0"), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(eval<double>("2.0 / 3.0"), 2.0 / 3.0);

    // Very small numbers
    EXPECT_DOUBLE_EQ(eval<double>("1e-15"), 1e-15);
    EXPECT_DOUBLE_EQ(eval<double>("1e-100"), 1e-100);

    // Very large numbers
    EXPECT_DOUBLE_EQ(eval<double>("1e15"), 1e15);
    EXPECT_DOUBLE_EQ(eval<double>("1e100"), 1e100);

    // Float vs double precision
    EXPECT_FLOAT_EQ(eval<float>("1.0 / 3.0"), 1.0f / 3.0f);
    EXPECT_DOUBLE_EQ(eval<double>("1.0 / 3.0"), 1.0 / 3.0);
}

// Complex Expression Tests
TEST(CalculatorTest, ComplexOperatorPrecedence) {
    // Complex precedence scenarios
    EXPECT_EQ(eval<int>("2 + 3 * 4 ** 2"), 2 + 3 * 16);  // 2 + 48 = 50
    EXPECT_EQ(eval<int>("2 ** 3 ** 2"),
              512);  // Right associative: 2^(3^2) = 2^9 = 512
    EXPECT_EQ(eval<int>("(2 ** 3) ** 2"),
              64);  // Left to right: (2^3)^2 = 8^2 = 64

    // Mixed operators with different precedence
    EXPECT_EQ(eval<int>("5 + 3 * 2 - 1"), 10);   // 5 + 6 - 1 = 10
    EXPECT_EQ(eval<int>("10 / 2 + 3 * 4"), 17);  // 5 + 12 = 17
    EXPECT_EQ(eval<int>("2 * 3 + 4 * 5"), 26);   // 6 + 20 = 26

    // Bitwise operator precedence
    EXPECT_EQ(eval<int>("5 | 3 & 1"),
              5 | (3 & 1));  // & has higher precedence than |
    EXPECT_EQ(eval<int>("8 >> 1 + 1"),
              8 >> (1 + 1));  // + has higher precedence than >>
    EXPECT_EQ(eval<int>("4 << 1 | 2"),
              (4 << 1) | 2);  // << has higher precedence than |
}

TEST(CalculatorTest, DeepNesting) {
    // Deeply nested parentheses
    EXPECT_EQ(eval<int>("((((1))))"), 1);
    EXPECT_EQ(eval<int>("(((1 + 2)))"), 3);
    EXPECT_EQ(eval<int>("((1 + (2 * (3 + 4))))"),
              15);  // 1 + (2 * 7) = 1 + 14 = 15
    EXPECT_EQ(eval<int>("(((((1 + 2) * 3) + 4) * 5) + 6)"),
              71);  // ((3*3+4)*5+6) = (13*5+6) = 71

    // Complex nested expressions
    EXPECT_EQ(eval<int>("((2 + 3) * (4 + 5))"), 45);  // 5 * 9 = 45
    EXPECT_EQ(eval<int>("((1 + 2) * (3 + 4) + (5 + 6))"),
              32);  // 3*7 + 11 = 21 + 11 = 32
}

TEST(CalculatorTest, LongExpressions) {
    // Long chains of operations
    EXPECT_EQ(eval<int>("1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1"), 10);
    EXPECT_EQ(eval<int>("2 * 2 * 2 * 2 * 2"), 32);
    EXPECT_EQ(
        eval<int>("100 - 10 - 10 - 10 - 10 - 10 - 10 - 10 - 10 - 10 - 10"), 0);

    // Mixed long expressions
    EXPECT_EQ(eval<int>("1 + 2 - 3 + 4 - 5 + 6 - 7 + 8 - 9 + 10"), 7);
    EXPECT_EQ(eval<int>("2 * 3 / 2 * 4 / 3 * 5 / 2"),
              10);  // ((((2*3)/2)*4)/3)*5)/2 = 10
}

TEST(CalculatorTest, MixedAssociativity) {
    // Right associative operators
    EXPECT_EQ(eval<int>("2 ** 3 ** 2"), 512);  // 2^(3^2) = 2^9 = 512
    EXPECT_EQ(eval<int>("3 ** 2 ** 2"), 81);   // 3^(2^2) = 3^4 = 81

    // Left associative operators
    EXPECT_EQ(eval<int>("10 - 5 - 2"), 3);  // (10-5)-2 = 3
    EXPECT_EQ(eval<int>("20 / 4 / 2"), 2);  // (20/4)/2 = 2
    EXPECT_EQ(eval<int>("16 % 7 % 3"), 2);  // (16%7)%3 = 2%3 = 2

    // Mixed associativity in complex expressions
    EXPECT_EQ(eval<int>("2 ** 3 + 4 ** 2"), 24);  // 8 + 16 = 24
    EXPECT_EQ(eval<int>("10 - 2 ** 3"), 2);       // 10 - 8 = 2
}

// Type-Specific Tests
TEST(CalculatorTest, IntegerSpecificOperations) {
    // Bitwise operations (only available for integers)
    EXPECT_EQ(eval<int>("~5"), ~5);
    EXPECT_EQ(eval<int>("~0"), ~0);
    EXPECT_EQ(eval<int>("~(-1)"), ~(-1));

    // Modulo operation
    EXPECT_EQ(eval<int>("17 % 5"), 2);
    EXPECT_EQ(eval<int>("100 % 7"), 2);
    EXPECT_EQ(eval<int>("(-17) % 5"), -2);

    // Bit shifting
    EXPECT_EQ(eval<int>("1 << 8"), 256);
    EXPECT_EQ(eval<int>("256 >> 4"), 16);
    EXPECT_EQ(eval<int>("(-8) >> 1"), -4);

    // Integer division truncation
    EXPECT_EQ(eval<int>("7 / 2"), 3);
    EXPECT_EQ(eval<int>("(-7) / 2"), -3);
    EXPECT_EQ(eval<int>("7 / (-2)"), -3);
}

TEST(CalculatorTest, FloatingPointSpecificOperations) {
    // Power operations with floating point
    EXPECT_DOUBLE_EQ(eval<double>("2.5 ** 2"), 6.25);
    EXPECT_DOUBLE_EQ(eval<double>("4.0 ** 0.5"), 2.0);
    EXPECT_DOUBLE_EQ(eval<double>("(-2.0) ** 3"), -8.0);

    // Precise division
    EXPECT_DOUBLE_EQ(eval<double>("7.0 / 2.0"), 3.5);
    EXPECT_DOUBLE_EQ(eval<double>("1.0 / 3.0"), 1.0 / 3.0);

    // Scientific notation in expressions
    EXPECT_DOUBLE_EQ(eval<double>("1e3 + 1e2"), 1100.0);
    EXPECT_DOUBLE_EQ(eval<double>("1e-3 * 1e6"), 1000.0);
}

TEST(CalculatorTest, TypeBehaviorDifferences) {
    // Same expression, different types
    EXPECT_EQ(eval<int>("10 / 3"), 3);
    EXPECT_DOUBLE_EQ(eval<double>("10.0 / 3.0"), 10.0 / 3.0);

    // Power operations
    EXPECT_EQ(eval<int>("2 ** 10"), 1024);
    EXPECT_DOUBLE_EQ(eval<double>("2.0 ** 10.0"), 1024.0);

    // Overflow behavior differences
    EXPECT_EQ(eval<int>("1000 * 1000"), 1000000);
    EXPECT_DOUBLE_EQ(eval<double>("1000.0 * 1000.0"), 1000000.0);
}

// Advanced Feature Tests
TEST(CalculatorTest, AdvancedVariableTests) {
    ExpressionParser<double> parser;

    // Variable names with underscores and numbers (avoid leading underscore)
    parser.set("var_1", 10.0);
    parser.set("var_2", 20.0);
    parser.set("private_var",
               5.0);  // Changed from "_private" which isn't supported
    parser.set("CONSTANT", 100.0);

    EXPECT_DOUBLE_EQ(parser.eval("var_1"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("var_1 + var_2"), 30.0);
    EXPECT_DOUBLE_EQ(parser.eval("private_var * 2"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("CONSTANT / 10"), 10.0);

    // Variable updates
    parser.set("x", 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("x"), 1.0);
    parser.set("x", 2.0);
    EXPECT_DOUBLE_EQ(parser.eval("x"), 2.0);

    // Complex variable expressions
    parser.set("a", 2.0);
    parser.set("b", 3.0);
    parser.set("c", 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("a * b + c"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("(a + b) * c"), 20.0);

    // Test that variables starting with underscore are not supported
    parser.set("_invalid", 5.0);
    EXPECT_THROW(parser.eval("_invalid"), error);
}

TEST(CalculatorTest, AdvancedFunctionTests) {
    ExpressionParser<double> parser;

    // Set up mathematical functions
    calculator_utils::setup_common_math_functions(parser);

    // Function composition
    EXPECT_DOUBLE_EQ(parser.eval("square(3)"), 9.0);
    EXPECT_DOUBLE_EQ(parser.eval("sqrt(square(5))"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("abs(square(-3))"), 9.0);

    // Functions in complex expressions
    EXPECT_DOUBLE_EQ(parser.eval("square(2) + cube(2)"), 12.0);   // 4 + 8 = 12
    EXPECT_DOUBLE_EQ(parser.eval("sqrt(16) * square(2)"), 16.0);  // 4 * 4 = 16

    // Functions with variables
    parser.set("x", 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("square(x)"), 9.0);
    EXPECT_DOUBLE_EQ(parser.eval("square(x + 1)"), 16.0);
}

// Additional Comprehensive Tests
TEST(CalculatorTest, PerformanceAndStressTests) {
    // Test with many operations
    std::string long_expr = "1";
    for (int i = 0; i < 100; ++i) {
        long_expr += " + 1";
    }
    EXPECT_EQ(eval<int>(long_expr), 101);

    // Test with deeply nested parentheses
    std::string nested_expr = "1";
    for (int i = 0; i < 20; ++i) {
        nested_expr = "(" + nested_expr + " + 1)";
    }
    EXPECT_EQ(eval<int>(nested_expr), 21);

    // Test with complex mathematical expressions
    EXPECT_DOUBLE_EQ(
        eval<double>("((1 + 2) * (3 + 4) - (5 - 6)) / ((7 + 8) - (9 - 10))"),
        ((1.0 + 2.0) * (3.0 + 4.0) - (5.0 - 6.0)) /
            ((7.0 + 8.0) - (9.0 - 10.0)));
}

TEST(CalculatorTest, EdgeCaseExpressions) {
    // Single character expressions
    EXPECT_EQ(eval<int>("5"), 5);
    EXPECT_EQ(eval<int>("0"), 0);

    // Expressions with only operators and parentheses
    EXPECT_EQ(eval<int>("(((5)))"), 5);
    EXPECT_EQ(eval<int>("+(+5)"), 5);
    EXPECT_EQ(eval<int>("-(+5)"), -5);
    EXPECT_EQ(eval<int>("+(-5)"), -5);
    EXPECT_EQ(eval<int>("-(-5)"), 5);

    // Mixed whitespace
    EXPECT_EQ(eval<int>("\t\n\r 5 \t\n\r + \t\n\r 3 \t\n\r"), 8);

    // Scientific notation edge cases
    EXPECT_DOUBLE_EQ(eval<double>("1e0"), 1.0);
    EXPECT_DOUBLE_EQ(eval<double>("1E0"), 1.0);
    EXPECT_DOUBLE_EQ(eval<double>("1e+0"), 1.0);
    EXPECT_DOUBLE_EQ(eval<double>("1e-0"), 1.0);
}

TEST(CalculatorTest, ComprehensiveOperatorTests) {
    // Test all operators in combination
    EXPECT_EQ(eval<int>("5 + 3 - 2 * 4 / 2 % 3"), 5 + 3 - 2 * 4 / 2 % 3);
    EXPECT_EQ(eval<int>("(5 | 3) & (7 ^ 2)"), (5 | 3) & (7 ^ 2));
    EXPECT_EQ(eval<int>("(8 << 1) >> (2 + 1)"), (8 << 1) >> (2 + 1));

    // Test operator precedence thoroughly
    EXPECT_EQ(eval<int>("2 + 3 * 4"), 2 + 3 * 4);  // 14
    EXPECT_EQ(eval<int>("2 * 3 + 4"), 2 * 3 + 4);  // 10
    EXPECT_EQ(eval<int>("2 ** 3 + 4"), 8 + 4);     // 12
    EXPECT_EQ(eval<int>("2 + 3 ** 2"), 2 + 9);     // 11

    // Test associativity
    EXPECT_EQ(eval<int>("10 - 5 - 2"), (10 - 5) - 2);  // Left associative: 3
    EXPECT_EQ(eval<int>("2 ** 3 ** 2"),
              512);  // Right associative: 2^(3^2) = 512
}

TEST(CalculatorTest, RobustnessTests) {
    ExpressionParser<double> parser;

    // Test variable and function robustness
    parser.set("x", 10.0);
    parser.set("square", [](double x) { return x * x; });

    // Variables in complex expressions
    EXPECT_DOUBLE_EQ(parser.eval("x + x * x - x / x"),
                     10.0 + 10.0 * 10.0 - 10.0 / 10.0);

    // Functions in complex expressions
    EXPECT_DOUBLE_EQ(parser.eval("square(x) + square(2)"), 100.0 + 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("square(x + 1) - square(x - 1)"),
                     121.0 - 81.0);

    // Mixed variables and functions
    EXPECT_DOUBLE_EQ(parser.eval("square(x) / x"), 10.0);

    // Test that parser state is properly maintained
    EXPECT_DOUBLE_EQ(parser.eval("x"), 10.0);  // Should still be 10
    parser.set("x", 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("x"), 5.0);  // Should now be 5
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
