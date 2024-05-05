#include "calculator.hpp"
#include <gtest/gtest.h>


using namespace calculator;

TEST(CalculatorTest, BasicArithmetic) {
  EXPECT_EQ(eval<int>("1 + 2"), 3);
  EXPECT_EQ(eval<int>("4 - 3"), 1);
  EXPECT_EQ(eval<int>("2 * 3"), 6);
  EXPECT_EQ(eval<int>("6 / 2"), 3);
  EXPECT_EQ(eval<int>("7 % 3"), 1);
  EXPECT_EQ(eval<int>("7 / 3"), 2);
}

TEST(CalculatorTest, BitwiseOperators) {
  EXPECT_EQ(eval<int>("5 | 3"), 7);
  EXPECT_EQ(eval<int>("5 ^ 3"), 6);
  EXPECT_EQ(eval<int>("5 & 3"), 1);
  EXPECT_EQ(eval<int>("5 << 1"), 10);
  EXPECT_EQ(eval<int>("5 >> 1"), 2);
}

TEST(CalculatorTest, UnaryOperators) {
  EXPECT_EQ(eval<int>("~0"), -1);
  EXPECT_EQ(eval<int>("+5"), 5);
  EXPECT_EQ(eval<int>("-5"), -5);
}

TEST(CalculatorTest, Parentheses) {
  EXPECT_EQ(eval<int>("(1 + 2) * 3"), 9);
  EXPECT_EQ(eval<int>("2 * (3 + 4)"), 14);
  EXPECT_EQ(eval<int>("(1 + (2 - 3)) * 4"), 0);
}

TEST(CalculatorTest, PowerAndExponent) {
  EXPECT_EQ(eval<int>("2 ** 3"), 8);
  EXPECT_EQ(eval<int>("2 ** 3 ** 2"), 512);
  EXPECT_EQ(eval<int>("1e3"), 1000);
  EXPECT_EQ(eval<int>("1E+2"), 100);
}

TEST(CalculatorTest, HexadecimalNumbers) {
  EXPECT_EQ(eval<int>("0x0"), 0);
  EXPECT_EQ(eval<int>("0x1"), 1);
  EXPECT_EQ(eval<int>("0xA"), 10);
  EXPECT_EQ(eval<int>("0xF"), 15);
  EXPECT_EQ(eval<int>("0xFF"), 255);
}

TEST(CalculatorTest, WhiteSpaces) {
  EXPECT_EQ(eval<int>("1 + 2"), 3);
  EXPECT_EQ(eval<int>("1+2"), 3);
  EXPECT_EQ(eval<int>(" 1 + 2 "), 3);
  EXPECT_EQ(eval<int>("\t1\n+\r2\v"), 3);
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
  EXPECT_DOUBLE_EQ(eval<double>("2.1 +1.5"), 3.6);
  EXPECT_DOUBLE_EQ(eval<double>("2.1 + 1.5"), 3.6);
  EXPECT_DOUBLE_EQ(eval<double>("1.5 - 2.5"), -1.0);
  EXPECT_DOUBLE_EQ(eval<double>("2.5 * 3.5"), 8.75);
  EXPECT_DOUBLE_EQ(eval<double>("7.5 / 2.5"), 3.0);
  EXPECT_DOUBLE_EQ(eval<double>("2.5 ** 3.5"), 24.705294220065465);
  EXPECT_DOUBLE_EQ(eval<double>("1.5e2"), 150.0);
  EXPECT_DOUBLE_EQ(eval<double>("-2.5"), -2.5);
  EXPECT_DOUBLE_EQ(eval("+2.5"), 2.5);
  EXPECT_DOUBLE_EQ(eval<double>("(1.5 + 2.5) * 3.5"), 14.0);
  EXPECT_DOUBLE_EQ(eval<double>("2.5 * (1.5 + 2.5)"), 10.0);
}

TEST(CalculatorTest, LargeNumbers) {
  EXPECT_EQ(eval<long long>("1000000000 + 1000000000"), 2000000000);
  EXPECT_EQ(eval<long long>("1000000000 * 1000000000"), 1000000000000000000);
}

TEST(CalculatorTest, InvalidExpressions) {
  EXPECT_THROW(eval<int>("1 +"), error);
  EXPECT_THROW(eval<double>("1......1 + 1"), error);
  EXPECT_THROW(eval<int>("1 + 1 + 1.1"), error);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
