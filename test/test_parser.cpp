/**
 * @file test_parser.cpp
 * @brief Comprehensive test suite for the calculator expression parser
 *
 * Tests cover:
 * - Basic arithmetic operations
 * - Comparison operators
 * - Logical operators
 * - Conditional expressions
 * - Variable assignment and usage
 * - User-defined functions
 * - Built-in functions and constants
 * - Symbolic differentiation
 * - Complex numbers
 * - Error handling
 */

#include <gtest/gtest.h>
#include "calculator.hpp"
#include <complex>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

using namespace calculator;

// ============================================================================
// Basic Arithmetic Tests
// ============================================================================

class ArithmeticTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(ArithmeticTest, Addition) {
    EXPECT_DOUBLE_EQ(parser.eval("1 + 2"), 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("10 + 20 + 30"), 60.0);
    EXPECT_DOUBLE_EQ(parser.eval("1.5 + 2.5"), 4.0);
}

TEST_F(ArithmeticTest, Subtraction) {
    EXPECT_DOUBLE_EQ(parser.eval("5 - 3"), 2.0);
    EXPECT_DOUBLE_EQ(parser.eval("100 - 50 - 25"), 25.0);
    EXPECT_DOUBLE_EQ(parser.eval("10.5 - 3.5"), 7.0);
}

TEST_F(ArithmeticTest, Multiplication) {
    EXPECT_DOUBLE_EQ(parser.eval("3 * 4"), 12.0);
    EXPECT_DOUBLE_EQ(parser.eval("2 * 3 * 4"), 24.0);
    EXPECT_DOUBLE_EQ(parser.eval("1.5 * 2"), 3.0);
}

TEST_F(ArithmeticTest, Division) {
    EXPECT_DOUBLE_EQ(parser.eval("10 / 2"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("100 / 10 / 2"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("7.5 / 2.5"), 3.0);
}

TEST_F(ArithmeticTest, Modulo) {
    EXPECT_DOUBLE_EQ(parser.eval("10 % 3"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("17 % 5"), 2.0);
    EXPECT_NEAR(parser.eval("10.5 % 3"), 1.5, 1e-10);
}

TEST_F(ArithmeticTest, MixedOperations) {
    EXPECT_DOUBLE_EQ(parser.eval("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(parser.eval("(2 + 3) * 4"), 20.0);
    EXPECT_DOUBLE_EQ(parser.eval("10 - 4 / 2"), 8.0);
    EXPECT_DOUBLE_EQ(parser.eval("(10 - 4) / 2"), 3.0);
}

TEST_F(ArithmeticTest, UnaryOperators) {
    EXPECT_DOUBLE_EQ(parser.eval("-5"), -5.0);
    EXPECT_DOUBLE_EQ(parser.eval("+5"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("--5"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("-(-5)"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("3 + -2"), 1.0);
}

TEST_F(ArithmeticTest, Parentheses) {
    EXPECT_DOUBLE_EQ(parser.eval("((1 + 2))"), 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("((2 + 3) * (4 - 1))"), 15.0);
    EXPECT_DOUBLE_EQ(parser.eval("(((1 + 2) * 3) + 4) * 5"), 65.0);
}

// ============================================================================
// Power Operator Tests
// ============================================================================

class PowerTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(PowerTest, CaretOperator) {
    EXPECT_DOUBLE_EQ(parser.eval("2^3"), 8.0);
    EXPECT_DOUBLE_EQ(parser.eval("3^2"), 9.0);
    EXPECT_DOUBLE_EQ(parser.eval("10^0"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("2^10"), 1024.0);
}

TEST_F(PowerTest, DoubleStarOperator) {
    EXPECT_DOUBLE_EQ(parser.eval("2**3"), 8.0);
    EXPECT_DOUBLE_EQ(parser.eval("3**2"), 9.0);
    EXPECT_DOUBLE_EQ(parser.eval("5**0"), 1.0);
}

TEST_F(PowerTest, RightAssociativity) {
    EXPECT_DOUBLE_EQ(parser.eval("2^3^2"), 512.0);
    EXPECT_DOUBLE_EQ(parser.eval("2**3**2"), 512.0);
}

TEST_F(PowerTest, FractionalExponents) {
    EXPECT_NEAR(parser.eval("4^0.5"), 2.0, 1e-10);
    EXPECT_NEAR(parser.eval("8^(1/3)"), 2.0, 1e-10);
    EXPECT_NEAR(parser.eval("27^(1/3)"), 3.0, 1e-10);
}

TEST_F(PowerTest, NegativeExponents) {
    EXPECT_DOUBLE_EQ(parser.eval("2^(-1)"), 0.5);
    EXPECT_DOUBLE_EQ(parser.eval("10^(-2)"), 0.01);
}

// ============================================================================
// Comparison Operator Tests
// ============================================================================

class ComparisonTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(ComparisonTest, GreaterThan) {
    EXPECT_DOUBLE_EQ(parser.eval("5 > 3"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("3 > 5"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 > 5"), 0.0);
}

TEST_F(ComparisonTest, LessThan) {
    EXPECT_DOUBLE_EQ(parser.eval("3 < 5"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 < 3"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 < 5"), 0.0);
}

TEST_F(ComparisonTest, GreaterEqual) {
    EXPECT_DOUBLE_EQ(parser.eval("5 >= 3"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 >= 5"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("3 >= 5"), 0.0);
}

TEST_F(ComparisonTest, LessEqual) {
    EXPECT_DOUBLE_EQ(parser.eval("3 <= 5"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 <= 5"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 <= 3"), 0.0);
}

TEST_F(ComparisonTest, Equal) {
    EXPECT_DOUBLE_EQ(parser.eval("5 == 5"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 == 3"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("3.14 == 3.14"), 1.0);
}

TEST_F(ComparisonTest, NotEqual) {
    EXPECT_DOUBLE_EQ(parser.eval("5 != 3"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 != 5"), 0.0);
}

TEST_F(ComparisonTest, ChainedComparisons) {
    EXPECT_DOUBLE_EQ(parser.eval("(5 > 3) == 1"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("(3 < 5) + (5 > 3)"), 2.0);
}

// ============================================================================
// Logical Operator Tests
// ============================================================================

class LogicalTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(LogicalTest, And) {
    EXPECT_DOUBLE_EQ(parser.eval("1 && 1"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("1 && 0"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("0 && 1"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("0 && 0"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 && 3"), 1.0);
}

TEST_F(LogicalTest, Or) {
    EXPECT_DOUBLE_EQ(parser.eval("1 || 1"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("1 || 0"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("0 || 1"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("0 || 0"), 0.0);
}

TEST_F(LogicalTest, Not) {
    EXPECT_DOUBLE_EQ(parser.eval("!0"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("!1"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("!5"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("!!1"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("!!!0"), 1.0);
}

TEST_F(LogicalTest, CombinedLogical) {
    EXPECT_DOUBLE_EQ(parser.eval("(5 > 3) && (2 < 4)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("(5 < 3) || (2 < 4)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("!(5 < 3)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("(1 && 1) || (0 && 0)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("(1 || 0) && (0 || 1)"), 1.0);
}

// ============================================================================
// Conditional Expression Tests
// ============================================================================

class TernaryTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(TernaryTest, BasicTernary) {
    EXPECT_DOUBLE_EQ(parser.eval("1 ? 10 : 20"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("0 ? 10 : 20"), 20.0);
}

TEST_F(TernaryTest, ExpressionCondition) {
    EXPECT_DOUBLE_EQ(parser.eval("5 > 3 ? 100 : 200"), 100.0);
    EXPECT_DOUBLE_EQ(parser.eval("5 < 3 ? 100 : 200"), 200.0);
}

TEST_F(TernaryTest, NestedTernary) {
    EXPECT_DOUBLE_EQ(parser.eval("1 ? (0 ? 1 : 2) : 3"), 2.0);
    EXPECT_DOUBLE_EQ(parser.eval("0 ? 1 : (1 ? 2 : 3)"), 2.0);
}

TEST_F(TernaryTest, TernaryWithExpressions) {
    EXPECT_DOUBLE_EQ(parser.eval("10 > 5 ? 2 + 3 : 4 * 5"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("10 < 5 ? 2 + 3 : 4 * 5"), 20.0);
}

TEST_F(TernaryTest, TernaryWithVariables) {
    parser.eval("x = 10");
    parser.eval("y = 5");
    EXPECT_DOUBLE_EQ(parser.eval("x > y ? x : y"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("x < y ? x : y"), 5.0);
}

// ============================================================================
// Variable Tests
// ============================================================================

class VariableTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(VariableTest, SimpleAssignment) {
    EXPECT_DOUBLE_EQ(parser.eval("x = 10"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("x"), 10.0);
}

TEST_F(VariableTest, MultipleVariables) {
    parser.eval("x = 5");
    parser.eval("y = 3");
    parser.eval("z = 2");
    EXPECT_DOUBLE_EQ(parser.eval("x + y + z"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("x * y * z"), 30.0);
}

TEST_F(VariableTest, VariableExpressions) {
    parser.eval("a = 10");
    parser.eval("b = a + 5");
    EXPECT_DOUBLE_EQ(parser.eval("b"), 15.0);
    EXPECT_DOUBLE_EQ(parser.eval("a * b"), 150.0);
}

TEST_F(VariableTest, ReassignVariable) {
    parser.eval("x = 10");
    EXPECT_DOUBLE_EQ(parser.eval("x"), 10.0);
    parser.eval("x = 20");
    EXPECT_DOUBLE_EQ(parser.eval("x"), 20.0);
    parser.eval("x = x + 5");
    EXPECT_DOUBLE_EQ(parser.eval("x"), 25.0);
}

TEST_F(VariableTest, BuiltinConstants) {
    EXPECT_NEAR(parser.eval("pi"), 3.14159265358979323846, 1e-10);
    EXPECT_NEAR(parser.eval("e"), 2.71828182845904523536, 1e-10);
    EXPECT_NEAR(parser.eval("tau"), 6.28318530717958647692, 1e-10);
    EXPECT_NEAR(parser.eval("phi"), 1.61803398874989484820, 1e-10);
}

TEST_F(VariableTest, ConstantsInExpressions) {
    EXPECT_NEAR(parser.eval("2 * pi"), 6.28318530717958647692, 1e-10);
    EXPECT_NEAR(parser.eval("e^1"), 2.71828182845904523536, 1e-10);
}

// ============================================================================
// User-Defined Function Tests
// ============================================================================

class UserFunctionTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(UserFunctionTest, SimpleFunction) {
    parser.eval("f(x) = x * 2");
    EXPECT_DOUBLE_EQ(parser.eval("f(5)"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("f(10)"), 20.0);
    EXPECT_DOUBLE_EQ(parser.eval("f(-3)"), -6.0);
}

TEST_F(UserFunctionTest, QuadraticFunction) {
    parser.eval("f(x) = x^2 + 2*x + 1");
    EXPECT_DOUBLE_EQ(parser.eval("f(0)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("f(1)"), 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("f(2)"), 9.0);
    EXPECT_DOUBLE_EQ(parser.eval("f(3)"), 16.0);
    EXPECT_DOUBLE_EQ(parser.eval("f(-1)"), 0.0);
}

TEST_F(UserFunctionTest, TwoArgumentFunction) {
    parser.eval("g(x, y) = x^2 + y^2");
    EXPECT_DOUBLE_EQ(parser.eval("g(3, 4)"), 25.0);
    EXPECT_DOUBLE_EQ(parser.eval("g(5, 12)"), 169.0);
    EXPECT_DOUBLE_EQ(parser.eval("g(0, 0)"), 0.0);
}

TEST_F(UserFunctionTest, ThreeArgumentFunction) {
    parser.eval("h(a, b, c) = a + b + c");
    EXPECT_DOUBLE_EQ(parser.eval("h(1, 2, 3)"), 6.0);
    EXPECT_DOUBLE_EQ(parser.eval("h(10, 20, 30)"), 60.0);
}

TEST_F(UserFunctionTest, FunctionWithBuiltins) {
    parser.eval("h(x) = sqrt(x) + sin(x)");
    EXPECT_NEAR(parser.eval("h(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("h(1)"), 1.0 + std::sin(1.0), 1e-10);
}

TEST_F(UserFunctionTest, FunctionComposition) {
    parser.eval("f(x) = x^2");
    parser.eval("g(x) = x + 1");
    EXPECT_DOUBLE_EQ(parser.eval("f(g(2))"), 9.0);
    EXPECT_DOUBLE_EQ(parser.eval("g(f(2))"), 5.0);
}

// ============================================================================
// Built-in Function Tests
// ============================================================================

class BuiltinFunctionTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(BuiltinFunctionTest, Trigonometric) {
    EXPECT_NEAR(parser.eval("sin(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("cos(0)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("tan(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("sin(pi/2)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("cos(pi)"), -1.0, 1e-10);
    EXPECT_NEAR(parser.eval("sin(pi/6)"), 0.5, 1e-10);
}

TEST_F(BuiltinFunctionTest, InverseTrigonometric) {
    EXPECT_NEAR(parser.eval("asin(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("acos(1)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("atan(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("asin(1)"), M_PI / 2, 1e-10);
}

TEST_F(BuiltinFunctionTest, HyperbolicTrigonometric) {
    EXPECT_NEAR(parser.eval("sinh(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("cosh(0)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("tanh(0)"), 0.0, 1e-10);
}

TEST_F(BuiltinFunctionTest, Logarithmic) {
    EXPECT_NEAR(parser.eval("ln(e)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("log(10)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("log10(100)"), 2.0, 1e-10);
    EXPECT_NEAR(parser.eval("log2(8)"), 3.0, 1e-10);
    EXPECT_NEAR(parser.eval("ln(1)"), 0.0, 1e-10);
}

TEST_F(BuiltinFunctionTest, Exponential) {
    EXPECT_NEAR(parser.eval("exp(0)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("exp(1)"), std::exp(1.0), 1e-10);
    EXPECT_NEAR(parser.eval("exp(2)"), std::exp(2.0), 1e-10);
}

TEST_F(BuiltinFunctionTest, PowerAndRoot) {
    EXPECT_DOUBLE_EQ(parser.eval("sqrt(16)"), 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("sqrt(25)"), 5.0);
    EXPECT_NEAR(parser.eval("cbrt(27)"), 3.0, 1e-10);
    EXPECT_NEAR(parser.eval("cbrt(8)"), 2.0, 1e-10);
    EXPECT_DOUBLE_EQ(parser.eval("pow(2, 10)"), 1024.0);
    EXPECT_DOUBLE_EQ(parser.eval("pow(3, 4)"), 81.0);
}

TEST_F(BuiltinFunctionTest, Rounding) {
    EXPECT_DOUBLE_EQ(parser.eval("floor(3.7)"), 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("floor(-3.7)"), -4.0);
    EXPECT_DOUBLE_EQ(parser.eval("ceil(3.2)"), 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("ceil(-3.2)"), -3.0);
    EXPECT_DOUBLE_EQ(parser.eval("round(3.5)"), 4.0);
    EXPECT_DOUBLE_EQ(parser.eval("round(3.4)"), 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("trunc(3.9)"), 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("trunc(-3.9)"), -3.0);
}

TEST_F(BuiltinFunctionTest, AbsoluteValue) {
    EXPECT_DOUBLE_EQ(parser.eval("abs(-5)"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("abs(5)"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("abs(0)"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("abs(-3.14)"), 3.14);
}

TEST_F(BuiltinFunctionTest, Sign) {
    EXPECT_DOUBLE_EQ(parser.eval("sign(5)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("sign(-5)"), -1.0);
    EXPECT_DOUBLE_EQ(parser.eval("sign(0)"), 0.0);
}

TEST_F(BuiltinFunctionTest, TwoArgFunctions) {
    EXPECT_DOUBLE_EQ(parser.eval("max(3, 7)"), 7.0);
    EXPECT_DOUBLE_EQ(parser.eval("max(-3, -7)"), -3.0);
    EXPECT_DOUBLE_EQ(parser.eval("min(3, 7)"), 3.0);
    EXPECT_DOUBLE_EQ(parser.eval("min(-3, -7)"), -7.0);
    EXPECT_DOUBLE_EQ(parser.eval("mod(10, 3)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("mod(17, 5)"), 2.0);
}

TEST_F(BuiltinFunctionTest, AngleConversion) {
    EXPECT_NEAR(parser.eval("deg(pi)"), 180.0, 1e-10);
    EXPECT_NEAR(parser.eval("rad(180)"), M_PI, 1e-10);
    EXPECT_NEAR(parser.eval("deg(pi/2)"), 90.0, 1e-10);
}

// ============================================================================
// Symbolic Differentiation Tests
// ============================================================================

class DifferentiationTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(DifferentiationTest, Constant) {
    std::string result = parser.diff("5", "x");
    EXPECT_EQ(result, "0");
}

TEST_F(DifferentiationTest, Variable) {
    std::string result = parser.diff("x", "x");
    EXPECT_EQ(result, "1");
}

TEST_F(DifferentiationTest, DifferentVariable) {
    std::string result = parser.diff("y", "x");
    EXPECT_EQ(result, "0");
}

TEST_F(DifferentiationTest, LinearFunction) {
    std::string result = parser.diff("2*x", "x");
    EXPECT_TRUE(result.find("2") != std::string::npos);
}

TEST_F(DifferentiationTest, PowerRule) {
    std::string result = parser.diff("x^2", "x");
    EXPECT_TRUE(result.find("2") != std::string::npos);
    EXPECT_TRUE(result.find("x") != std::string::npos);
}

TEST_F(DifferentiationTest, CubicFunction) {
    std::string result = parser.diff("x^3", "x");
    EXPECT_TRUE(result.find("3") != std::string::npos);
}

TEST_F(DifferentiationTest, SumRule) {
    std::string result = parser.diff("x + 5", "x");
    EXPECT_TRUE(result.find("1") != std::string::npos || result.find("0") != std::string::npos);
}

TEST_F(DifferentiationTest, TrigonometricDerivatives) {
    std::string sinDeriv = parser.diff("sin(x)", "x");
    EXPECT_TRUE(sinDeriv.find("cos") != std::string::npos);

    std::string cosDeriv = parser.diff("cos(x)", "x");
    EXPECT_TRUE(cosDeriv.find("sin") != std::string::npos);
}

// ============================================================================
// Complex Number Tests
// ============================================================================

class ComplexTest : public ::testing::Test {
protected:
    ExpressionParser<std::complex<double>> parser;
};

TEST_F(ComplexTest, BasicComplex) {
    auto result = parser.eval("3 + 4*i");
    EXPECT_DOUBLE_EQ(result.real(), 3.0);
    EXPECT_DOUBLE_EQ(result.imag(), 4.0);
}

TEST_F(ComplexTest, ImaginaryUnit) {
    auto i_squared = parser.eval("i * i");
    EXPECT_DOUBLE_EQ(i_squared.real(), -1.0);
    EXPECT_NEAR(i_squared.imag(), 0.0, 1e-10);
}

TEST_F(ComplexTest, ComplexAddition) {
    parser.eval("z1 = 3 + 4*i");
    parser.eval("z2 = 1 + 2*i");

    auto sum = parser.eval("z1 + z2");
    EXPECT_DOUBLE_EQ(sum.real(), 4.0);
    EXPECT_DOUBLE_EQ(sum.imag(), 6.0);
}

TEST_F(ComplexTest, ComplexSubtraction) {
    parser.eval("z1 = 3 + 4*i");
    parser.eval("z2 = 1 + 2*i");

    auto diff = parser.eval("z1 - z2");
    EXPECT_DOUBLE_EQ(diff.real(), 2.0);
    EXPECT_DOUBLE_EQ(diff.imag(), 2.0);
}

TEST_F(ComplexTest, ComplexMultiplication) {
    parser.eval("z1 = 3 + 4*i");
    parser.eval("z2 = 1 + 2*i");
    auto product = parser.eval("z1 * z2");
    EXPECT_DOUBLE_EQ(product.real(), -5.0);
    EXPECT_DOUBLE_EQ(product.imag(), 10.0);
}

TEST_F(ComplexTest, ComplexDivision) {
    parser.eval("z1 = 3 + 4*i");
    parser.eval("z2 = 1 + 2*i");
    auto quotient = parser.eval("z1 / z2");
    EXPECT_NEAR(quotient.real(), 2.2, 1e-10);
    EXPECT_NEAR(quotient.imag(), -0.4, 1e-10);
}

TEST_F(ComplexTest, ComplexFunctions) {
    parser.eval("z = 3 + 4*i");

    auto absVal = parser.eval("abs(z)");
    EXPECT_DOUBLE_EQ(absVal.real(), 5.0);

    auto conjVal = parser.eval("conj(z)");
    EXPECT_DOUBLE_EQ(conjVal.real(), 3.0);
    EXPECT_DOUBLE_EQ(conjVal.imag(), -4.0);

    auto realPart = parser.eval("real(z)");
    EXPECT_DOUBLE_EQ(realPart.real(), 3.0);

    auto imagPart = parser.eval("imag(z)");
    EXPECT_DOUBLE_EQ(imagPart.real(), 4.0);
}

// ============================================================================
// Bitwise Operator Tests (Integer)
// ============================================================================

class BitwiseTest : public ::testing::Test {
protected:
    ExpressionParser<int> parser;
};

TEST_F(BitwiseTest, BitwiseAnd) {
    EXPECT_EQ(parser.eval("5 & 3"), 1);
    EXPECT_EQ(parser.eval("12 & 10"), 8);
    EXPECT_EQ(parser.eval("0xFF & 0x0F"), 15);
}

TEST_F(BitwiseTest, BitwiseOr) {
    EXPECT_EQ(parser.eval("5 | 3"), 7);
    EXPECT_EQ(parser.eval("12 | 10"), 14);
}

TEST_F(BitwiseTest, BitwiseNot) {
    EXPECT_EQ(parser.eval("~0"), -1);
}

TEST_F(BitwiseTest, ShiftOperators) {
    EXPECT_EQ(parser.eval("1 << 4"), 16);
    EXPECT_EQ(parser.eval("16 >> 2"), 4);
    EXPECT_EQ(parser.eval("8 << 3"), 64);
}

// ============================================================================
// Error Handling Tests
// ============================================================================

class ErrorTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(ErrorTest, DivisionByZero) {
    EXPECT_THROW(parser.eval("1/0"), error);
    EXPECT_THROW(parser.eval("10 / (5-5)"), error);
}

TEST_F(ErrorTest, ModuloByZero) {
    EXPECT_THROW(parser.eval("10 % 0"), error);
}

TEST_F(ErrorTest, UndefinedVariable) {
    EXPECT_THROW(parser.eval("undefined_var"), error);
    EXPECT_THROW(parser.eval("x + undefined"), error);
}

TEST_F(ErrorTest, UndefinedFunction) {
    EXPECT_THROW(parser.eval("unknown_func(5)"), error);
}

TEST_F(ErrorTest, MismatchedParentheses) {
    EXPECT_THROW(parser.eval("(1 + 2"), error);
    EXPECT_THROW(parser.eval("1 + 2)"), error);
    EXPECT_THROW(parser.eval("((1 + 2)"), error);
}

TEST_F(ErrorTest, InvalidSyntax) {
    // Note: "1 + + 2" is valid as it parses as 1 + (+2)
    EXPECT_THROW(parser.eval("* 5"), error);
    EXPECT_THROW(parser.eval("5 *"), error);
}

TEST_F(ErrorTest, EmptyExpression) {
    EXPECT_THROW(parser.eval(""), error);
}

TEST_F(ErrorTest, WrongArgumentCount) {
    EXPECT_THROW(parser.eval("sin(1, 2)"), error);
    EXPECT_THROW(parser.eval("max(1)"), error);
}

// ============================================================================
// Integration Tests
// ============================================================================

class IntegrationTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(IntegrationTest, ComplexExpression) {
    EXPECT_DOUBLE_EQ(parser.eval("sqrt(16) + 2^3"), 12.0);
    EXPECT_NEAR(parser.eval("sin(pi/2) + cos(0)"), 2.0, 1e-10);
}

TEST_F(IntegrationTest, VariablesAndFunctions) {
    parser.eval("x = 5");
    parser.eval("f(n) = n^2 - 4*n + 4");
    EXPECT_DOUBLE_EQ(parser.eval("f(x)"), 9.0);
}

TEST_F(IntegrationTest, ConditionalWithVariables) {
    parser.eval("x = 10");
    parser.eval("y = 5");
    EXPECT_DOUBLE_EQ(parser.eval("x > y ? x : y"), 10.0);
    EXPECT_DOUBLE_EQ(parser.eval("x < y ? x : y"), 5.0);
}

TEST_F(IntegrationTest, NestedFunctionCalls) {
    EXPECT_DOUBLE_EQ(parser.eval("sqrt(abs(-16))"), 4.0);
    EXPECT_NEAR(parser.eval("sin(asin(0.5))"), 0.5, 1e-10);
    EXPECT_NEAR(parser.eval("exp(ln(5))"), 5.0, 1e-10);
}

TEST_F(IntegrationTest, ScientificCalculations) {
    // Euler's formula requires complex parser, skip for double parser
    EXPECT_NEAR(parser.eval("e^pi"), std::pow(M_E, M_PI), 1e-10);
}

TEST_F(IntegrationTest, FinancialCalculation) {
    parser.eval("principal = 1000");
    parser.eval("rate = 0.05");
    parser.eval("years = 10");
    double result = parser.eval("principal * (1 + rate)^years");
    EXPECT_NEAR(result, 1628.89, 0.01);
}

TEST_F(IntegrationTest, PhysicsFormula) {
    parser.eval("g = 9.81");
    parser.eval("v0 = 20");
    parser.eval("theta = pi/4");
    double range = parser.eval("(v0^2 * sin(2*theta)) / g");
    EXPECT_NEAR(range, 40.77, 0.01);
}

// ============================================================================
// New Features Tests (v2.1)
// ============================================================================

class NewFeaturesTest : public ::testing::Test {
protected:
    ExpressionParser<double> parser;
};

TEST_F(NewFeaturesTest, FloorDivision) {
    EXPECT_DOUBLE_EQ(parser.eval("7 // 3"), 2.0);
    EXPECT_DOUBLE_EQ(parser.eval("10 // 4"), 2.0);
    EXPECT_DOUBLE_EQ(parser.eval("-7 // 3"), -3.0);
    EXPECT_DOUBLE_EQ(parser.eval("15.5 // 3"), 5.0);
}

TEST_F(NewFeaturesTest, AdditionalTrigFunctions) {
    EXPECT_NEAR(parser.eval("cot(pi/4)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("sec(0)"), 1.0, 1e-10);
    EXPECT_NEAR(parser.eval("csc(pi/2)"), 1.0, 1e-10);
}

TEST_F(NewFeaturesTest, InverseHyperbolicFunctions) {
    EXPECT_NEAR(parser.eval("asinh(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("acosh(1)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("atanh(0)"), 0.0, 1e-10);
}

TEST_F(NewFeaturesTest, FactorialAndGamma) {
    EXPECT_NEAR(parser.eval("fact(5)"), 120.0, 1e-10);
    EXPECT_NEAR(parser.eval("factorial(6)"), 720.0, 1e-10);
    EXPECT_NEAR(parser.eval("gamma(5)"), 24.0, 1e-10);  // gamma(5) = 4! = 24
}

TEST_F(NewFeaturesTest, GcdAndLcm) {
    EXPECT_DOUBLE_EQ(parser.eval("gcd(48, 18)"), 6.0);
    EXPECT_DOUBLE_EQ(parser.eval("gcd(100, 35)"), 5.0);
    EXPECT_DOUBLE_EQ(parser.eval("lcm(4, 6)"), 12.0);
    EXPECT_DOUBLE_EQ(parser.eval("lcm(3, 5)"), 15.0);
}

TEST_F(NewFeaturesTest, Combinatorics) {
    EXPECT_NEAR(parser.eval("nCr(10, 3)"), 120.0, 1e-10);
    EXPECT_NEAR(parser.eval("nPr(5, 3)"), 60.0, 1e-10);
    EXPECT_NEAR(parser.eval("comb(6, 2)"), 15.0, 1e-10);
    EXPECT_NEAR(parser.eval("perm(4, 2)"), 12.0, 1e-10);
}

TEST_F(NewFeaturesTest, ErrorFunctions) {
    EXPECT_NEAR(parser.eval("erf(0)"), 0.0, 1e-10);
    EXPECT_NEAR(parser.eval("erfc(0)"), 1.0, 1e-10);
}

TEST_F(NewFeaturesTest, UtilityFunctions) {
    EXPECT_DOUBLE_EQ(parser.eval("sqr(5)"), 25.0);
    EXPECT_DOUBLE_EQ(parser.eval("cube(3)"), 27.0);
    EXPECT_DOUBLE_EQ(parser.eval("recip(4)"), 0.25);
    EXPECT_DOUBLE_EQ(parser.eval("inv(2)"), 0.5);
    EXPECT_NEAR(parser.eval("frac(3.75)"), 0.75, 1e-10);
}

TEST_F(NewFeaturesTest, TwoArgFunctions) {
    EXPECT_NEAR(parser.eval("hypot(3, 4)"), 5.0, 1e-10);
    EXPECT_DOUBLE_EQ(parser.eval("avg(10, 20)"), 15.0);
    EXPECT_NEAR(parser.eval("logb(8, 2)"), 3.0, 1e-10);
    EXPECT_DOUBLE_EQ(parser.eval("wrap(10, 3)"), 1.0);
}

TEST_F(NewFeaturesTest, AdditionalConstants) {
    EXPECT_NEAR(parser.eval("sqrt2"), 1.41421356237, 1e-10);
    EXPECT_NEAR(parser.eval("sqrt3"), 1.73205080757, 1e-10);
    EXPECT_NEAR(parser.eval("ln2"), 0.69314718056, 1e-10);
    EXPECT_NEAR(parser.eval("ln10"), 2.30258509299, 1e-10);
}

TEST_F(NewFeaturesTest, PhysicsConstants) {
    EXPECT_DOUBLE_EQ(parser.eval("c"), 299792458.0);  // Speed of light
    EXPECT_NEAR(parser.eval("Na"), 6.02214076e23, 1e17);  // Avogadro's number
}

TEST_F(NewFeaturesTest, SpecialFunctions) {
    EXPECT_DOUBLE_EQ(parser.eval("heaviside(1)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("heaviside(-1)"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("heaviside(0)"), 0.5);
    EXPECT_DOUBLE_EQ(parser.eval("sign(5)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("sign(-5)"), -1.0);
    EXPECT_DOUBLE_EQ(parser.eval("sign(0)"), 0.0);
}

TEST_F(NewFeaturesTest, AdvancedExpressions) {
    // Quadratic formula components
    parser.eval("a = 1");
    parser.eval("b = -5");
    parser.eval("c = 6");
    double discriminant = parser.eval("sqr(b) - 4*a*c");
    EXPECT_DOUBLE_EQ(discriminant, 1.0);
    
    // Using new functions in expressions
    EXPECT_NEAR(parser.eval("fact(5) / (fact(3) * fact(2))"), 10.0, 1e-10);  // C(5,2)
    EXPECT_NEAR(parser.eval("hypot(3, 4) + sqrt(sqr(3) + sqr(4))"), 10.0, 1e-10);
}
