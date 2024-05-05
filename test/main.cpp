#include <gtest/gtest.h>
#include "calculator.hpp"

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
    parser.set("pi", 3.1415926535897932);
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("cos", [](double x) { return std::cos(x); });
    EXPECT_DOUBLE_EQ(parser.eval("sqrt(2)"), 1.4142135623730951);
    EXPECT_DOUBLE_EQ(parser.eval("sin(0)"), 0.0);
    EXPECT_DOUBLE_EQ(parser.eval("cos(0)"), 1.0);
    EXPECT_DOUBLE_EQ(parser.eval("sin(pi/2)"), 1.0);

    parser.set("e", 2.718281828459045);
    parser.set("ln", [](double x) { return std::log(x); });
    parser.set("exp", [](double x) { return std::exp(x); });
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

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
