#include "calculator.hpp"
#include <benchmark/benchmark.h>

using namespace calculator;

static void BM_CalculatorInt(benchmark::State &state) {
  ExpressionParser<int> parser;
  for (auto _ : state) {
    parser.eval("1 + 2 * 3 - 4 / 2");
  }
}

static void BM_CalculatorDouble(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    parser.eval("1.5 + 2.5 * 3.5 - 4.5 / 2.5");
  }
}

static void BM_CalculatorComplexInt(benchmark::State &state) {
  ExpressionParser<int> parser;
  for (auto _ : state) {
    parser.eval("(1 + 2) * (3 - 4) / (5 + 6) * (7 - 8) + 9");
  }
}

static void BM_CalculatorComplexDouble(benchmark::State &state) {
  ExpressionParser<double> parser;
  for (auto _ : state) {
    parser.eval("(1.5 + 2.5) * (3.5 - 4.5) / (5.5 + 6.5) * (7.5 - 8.5) + 9.5");
  }
}

BENCHMARK(BM_CalculatorInt);
BENCHMARK(BM_CalculatorDouble);
BENCHMARK(BM_CalculatorComplexInt);
BENCHMARK(BM_CalculatorComplexDouble);

BENCHMARK_MAIN();