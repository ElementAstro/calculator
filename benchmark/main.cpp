#include "calculator.hpp"
#include <benchmark/benchmark.h>
#include <complex>

using namespace calculator;

// Basic arithmetic benchmarks
static void BM_BasicArithmetic(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("1 + 2 * 3 - 4 / 2"));
  }
}

static void BM_ComplexExpression(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("(1.5 + 2.5) * (3.5 - 4.5) / (5.5 + 6.5) * (7.5 - 8.5) + 9.5"));
  }
}

// Power operations
static void BM_PowerOperations(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("2^10 + 3^5 - 4^3"));
  }
}

// Comparison and logical operators
static void BM_ComparisonLogical(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("(5 > 3) && (2 < 4) || (1 == 1)"));
  }
}

// Ternary expression
static void BM_TernaryExpression(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("10 > 5 ? 100 + 50 : 200 - 50"));
  }
}

// Built-in functions
static void BM_BuiltinFunctions(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("sqrt(16) + sin(0) + cos(0) + abs(-5)"));
  }
}

// Variables
static void BM_Variables(benchmark::State &state) {
  ExpressionParser<double> parser;
  parser.set("x", 10.0);
  parser.set("y", 20.0);
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("x + y * 2 - x / 2"));
  }
}

// User-defined functions
static void BM_UserDefinedFunction(benchmark::State &state) {
  ExpressionParser<double> parser;
  parser.eval("f(x) = x^2 + 2*x + 1");
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("f(10)"));
  }
}

// Complex numbers
static void BM_ComplexNumbers(benchmark::State &state) {
  ExpressionParser<std::complex<double>> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("(3 + 4*i) * (1 + 2*i)"));
  }
}

// Nested expressions
static void BM_NestedExpressions(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.eval("((((1 + 2) * 3) + 4) * 5) + 6"));
  }
}

// Symbolic differentiation
static void BM_Differentiation(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    benchmark::DoNotOptimize(parser.diff("x^3 + 2*x^2 + x + 1", "x"));
  }
}

BENCHMARK(BM_BasicArithmetic);
BENCHMARK(BM_ComplexExpression);
BENCHMARK(BM_PowerOperations);
BENCHMARK(BM_ComparisonLogical);
BENCHMARK(BM_TernaryExpression);
BENCHMARK(BM_BuiltinFunctions);
BENCHMARK(BM_Variables);
BENCHMARK(BM_UserDefinedFunction);
BENCHMARK(BM_ComplexNumbers);
BENCHMARK(BM_NestedExpressions);
BENCHMARK(BM_Differentiation);

BENCHMARK_MAIN();