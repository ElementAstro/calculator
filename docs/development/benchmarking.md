# Benchmarking Guide

This guide covers performance testing and benchmarking for the Modern C++ Calculator library. Proper benchmarking ensures optimal performance and helps identify bottlenecks and regressions.

## Benchmarking Framework

### Google Benchmark Integration

The project uses Google Benchmark for performance testing:

```cpp
#include <benchmark/benchmark.h>
#include "calculator.hpp"

// Basic benchmark structure
static void BM_SimpleExpression(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("2 + 3 * 4");
        benchmark::DoNotOptimize(result);  // Prevent optimization
    }
}
BENCHMARK(BM_SimpleExpression);

// Register the benchmark
BENCHMARK_MAIN();
```

### Benchmark Organization

Organize benchmarks by functionality:

```cpp
// Arithmetic operations benchmarks
class ArithmeticBenchmark : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        parser.set("pi", M_PI);
        parser.set("e", M_E);
    }
    
    calculator::ExpressionParser<double> parser;
};

BENCHMARK_F(ArithmeticBenchmark, BasicArithmetic)(benchmark::State& state) {
    for (auto _ : state) {
        double result = parser.eval("2 + 3 * 4 - 1");
        benchmark::DoNotOptimize(result);
    }
}
```

## Core Benchmarks

### 1. Basic Operations

```cpp
// Simple arithmetic operations
static void BM_Addition(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("123.456 + 789.012");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Addition);

static void BM_Multiplication(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("123.456 * 789.012");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Multiplication);

static void BM_Division(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("789.012 / 123.456");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Division);

static void BM_Exponentiation(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("2.5 ** 3.7");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Exponentiation);
```

### 2. Expression Complexity

```cpp
// Test different expression complexities
static void BM_SimpleExpression(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("2 + 3");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_SimpleExpression);

static void BM_MediumExpression(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("(2 + 3) * 4 - 1 / 2");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_MediumExpression);

static void BM_ComplexExpression(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("((2 + 3) * 4 - 1) / (5 + 2 * 3) ** 2");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ComplexExpression);

static void BM_VeryComplexExpression(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval(
            "((2 + 3) * 4 - 1) / (5 + 2 * 3) ** 2 + "
            "(7 - 2) * (8 + 1) / (3 ** 2 - 1)"
        );
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_VeryComplexExpression);
```

### 3. Variable and Function Usage

```cpp
class VariableBenchmark : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        parser.set("x", 5.0);
        parser.set("y", 10.0);
        parser.set("z", 2.5);
        parser.set("pi", M_PI);
        parser.set("e", M_E);
    }
    
    calculator::ExpressionParser<double> parser;
};

BENCHMARK_F(VariableBenchmark, SimpleVariable)(benchmark::State& state) {
    for (auto _ : state) {
        double result = parser.eval("x + y");
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_F(VariableBenchmark, MultipleVariables)(benchmark::State& state) {
    for (auto _ : state) {
        double result = parser.eval("x * y + z ** 2");
        benchmark::DoNotOptimize(result);
    }
}

class FunctionBenchmark : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) override {
        parser.set("sin", [](double x) { return std::sin(x); });
        parser.set("cos", [](double x) { return std::cos(x); });
        parser.set("sqrt", [](double x) { return std::sqrt(x); });
        parser.set("log", [](double x) { return std::log(x); });
        parser.set("pi", M_PI);
        parser.set("e", M_E);
    }
    
    calculator::ExpressionParser<double> parser;
};

BENCHMARK_F(FunctionBenchmark, SingleFunction)(benchmark::State& state) {
    for (auto _ : state) {
        double result = parser.eval("sin(pi/2)");
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_F(FunctionBenchmark, NestedFunctions)(benchmark::State& state) {
    for (auto _ : state) {
        double result = parser.eval("sin(cos(pi/4))");
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK_F(FunctionBenchmark, ComplexFunctionExpression)(benchmark::State& state) {
    for (auto _ : state) {
        double result = parser.eval("sqrt(sin(pi/2) ** 2 + cos(0) ** 2)");
        benchmark::DoNotOptimize(result);
    }
}
```

### 4. Type-Specific Benchmarks

```cpp
// Integer operations
static void BM_IntegerArithmetic(benchmark::State& state) {
    for (auto _ : state) {
        int result = calculator::eval<int>("123 + 456 * 789");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_IntegerArithmetic);

static void BM_BitwiseOperations(benchmark::State& state) {
    for (auto _ : state) {
        int result = calculator::eval<int>("0xFF & 0xAA | 0x55 ^ 0x33");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_BitwiseOperations);

// Float vs Double comparison
static void BM_FloatArithmetic(benchmark::State& state) {
    for (auto _ : state) {
        float result = calculator::eval<float>("123.456 + 789.012 * 2.5");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_FloatArithmetic);

static void BM_DoubleArithmetic(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval<double>("123.456 + 789.012 * 2.5");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_DoubleArithmetic);
```

## Advanced Benchmarking

### 1. Parameterized Benchmarks

```cpp
// Test different expression lengths
static void BM_ExpressionLength(benchmark::State& state) {
    int length = state.range(0);
    
    // Generate expression of specified length
    std::string expr = "1";
    for (int i = 1; i < length; ++i) {
        expr += " + " + std::to_string(i + 1);
    }
    
    for (auto _ : state) {
        double result = calculator::eval(expr);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ExpressionLength)->Range(1, 100);

// Test different nesting depths
static void BM_NestingDepth(benchmark::State& state) {
    int depth = state.range(0);
    
    // Generate nested parentheses expression
    std::string expr = "1";
    for (int i = 0; i < depth; ++i) {
        expr = "(" + expr + " + 1)";
    }
    
    for (auto _ : state) {
        double result = calculator::eval(expr);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_NestingDepth)->Range(1, 50);

// Test different numbers of variables
static void BM_VariableCount(benchmark::State& state) {
    int var_count = state.range(0);
    
    calculator::ExpressionParser<double> parser;
    
    // Set up variables
    for (int i = 0; i < var_count; ++i) {
        parser.set("var" + std::to_string(i), static_cast<double>(i + 1));
    }
    
    // Create expression using all variables
    std::string expr = "var0";
    for (int i = 1; i < var_count; ++i) {
        expr += " + var" + std::to_string(i);
    }
    
    for (auto _ : state) {
        double result = parser.eval(expr);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_VariableCount)->Range(1, 100);
```

### 2. Memory Usage Benchmarks

```cpp
// Benchmark memory allocation patterns
static void BM_ParserCreation(benchmark::State& state) {
    for (auto _ : state) {
        calculator::ExpressionParser<double> parser;
        parser.set("x", 42.0);
        benchmark::DoNotOptimize(parser);
    }
}
BENCHMARK(BM_ParserCreation);

static void BM_ParserReuse(benchmark::State& state) {
    calculator::ExpressionParser<double> parser;
    
    for (auto _ : state) {
        parser.set("x", 42.0);
        double result = parser.eval("x * 2");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ParserReuse);

// Benchmark with many variables
static void BM_ManyVariables(benchmark::State& state) {
    calculator::ExpressionParser<double> parser;
    
    // Set up many variables once
    for (int i = 0; i < 1000; ++i) {
        parser.set("var" + std::to_string(i), static_cast<double>(i));
    }
    
    for (auto _ : state) {
        double result = parser.eval("var500 + var750");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ManyVariables);
```

### 3. Comparison Benchmarks

```cpp
// Compare with direct C++ calculation
static void BM_DirectCalculation(benchmark::State& state) {
    for (auto _ : state) {
        double result = 2.0 + 3.0 * 4.0;
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_DirectCalculation);

static void BM_CalculatorEvaluation(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("2 + 3 * 4");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_CalculatorEvaluation);

// Compare different approaches
static void BM_FreeFunction(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("sin(3.14159/2)");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_FreeFunction);

static void BM_ParserInstance(benchmark::State& state) {
    calculator::ExpressionParser<double> parser;
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("pi", M_PI);
    
    for (auto _ : state) {
        double result = parser.eval("sin(pi/2)");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_ParserInstance);
```

## Running Benchmarks

### Command Line Options

```bash
# Run all benchmarks
./calc_benchmark

# Run specific benchmark
./calc_benchmark --benchmark_filter="BM_SimpleExpression"

# Run benchmarks matching pattern
./calc_benchmark --benchmark_filter=".*Arithmetic.*"

# Set minimum time per benchmark
./calc_benchmark --benchmark_min_time=1.0

# Set number of repetitions
./calc_benchmark --benchmark_repetitions=10

# Output formats
./calc_benchmark --benchmark_format=json
./calc_benchmark --benchmark_format=csv
./calc_benchmark --benchmark_format=console

# Save results to file
./calc_benchmark --benchmark_out=results.json --benchmark_out_format=json

# Display counters
./calc_benchmark --benchmark_counters_tabular=true

# Show CPU and memory info
./calc_benchmark --benchmark_display_aggregates_only=false
```

### Benchmark Configuration

```cpp
// Custom benchmark configuration
static void BM_CustomConfig(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("complex_expression");
        benchmark::DoNotOptimize(result);
    }
    
    // Set custom counters
    state.counters["expressions_per_second"] = benchmark::Counter(
        state.iterations(), benchmark::Counter::kIsRate);
    
    // Set bytes processed
    state.SetBytesProcessed(state.iterations() * sizeof(double));
    
    // Set items processed
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_CustomConfig)->MinTime(2.0)->Repetitions(5);
```

## Performance Analysis

### 1. Interpreting Results

```
Benchmark                    Time             CPU   Iterations
BM_SimpleExpression        156 ns          156 ns      4480000
BM_MediumExpression        234 ns          234 ns      2987733
BM_ComplexExpression       445 ns          445 ns      1571429
```

Key metrics:

- **Time**: Wall clock time per iteration
- **CPU**: CPU time per iteration
- **Iterations**: Number of iterations run

### 2. Performance Targets

| Operation Type | Target Time | Notes |
|----------------|-------------|-------|
| Simple arithmetic | < 100 ns | Basic operations |
| Medium expressions | < 500 ns | Multiple operations |
| Complex expressions | < 1 μs | Nested operations |
| Function calls | < 200 ns | Single function |
| Variable lookup | < 50 ns | Variable access |

### 3. Regression Detection

```cpp
// Baseline benchmark for regression testing
static void BM_Baseline_v1_0(benchmark::State& state) {
    for (auto _ : state) {
        double result = calculator::eval("2 + 3 * 4 / (1 + 1)");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Baseline_v1_0);

// Compare with previous version
static void BM_RegressionTest(benchmark::State& state) {
    // This should perform similarly to baseline
    for (auto _ : state) {
        double result = calculator::eval("2 + 3 * 4 / (1 + 1)");
        benchmark::DoNotOptimize(result);
    }
    
    // Fail if performance degrades significantly
    // (This would be checked in CI/CD pipeline)
}
BENCHMARK(BM_RegressionTest);
```

## Profiling Integration

### 1. CPU Profiling

```bash
# Profile with perf (Linux)
perf record -g ./calc_benchmark --benchmark_filter="BM_ComplexExpression"
perf report

# Profile with Instruments (macOS)
instruments -t "Time Profiler" ./calc_benchmark

# Profile with Intel VTune
vtune -collect hotspots ./calc_benchmark
```

### 2. Memory Profiling

```bash
# Profile with Valgrind
valgrind --tool=massif ./calc_benchmark --benchmark_filter="BM_ManyVariables"

# Profile with AddressSanitizer
export ASAN_OPTIONS=detect_leaks=1
./calc_benchmark_asan --benchmark_filter="BM_ParserCreation"
```

### 3. Cache Analysis

```bash
# Cache miss analysis with perf
perf stat -e cache-references,cache-misses,instructions,cycles \
    ./calc_benchmark --benchmark_filter="BM_VariableCount"
```

## Continuous Performance Monitoring

### 1. CI/CD Integration

```yaml
# .github/workflows/benchmark.yml
name: Performance Benchmarks

on: [push, pull_request]

jobs:
  benchmark:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Build benchmarks
      run: |
        cmake -B build -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
        cmake --build build
    
    - name: Run benchmarks
      run: |
        cd build
        ./calc_benchmark --benchmark_format=json --benchmark_out=results.json
    
    - name: Store benchmark results
      uses: benchmark-action/github-action-benchmark@v1
      with:
        tool: 'googlecpp'
        output-file-path: build/results.json
```

### 2. Performance Tracking

```cpp
// Automated performance regression detection
class PerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load baseline performance data
        load_baseline_metrics();
    }
    
    void check_performance_regression(const std::string& benchmark_name, 
                                    double current_time_ns) {
        auto baseline_it = baseline_metrics_.find(benchmark_name);
        if (baseline_it != baseline_metrics_.end()) {
            double baseline_time = baseline_it->second;
            double regression_threshold = 1.1;  // 10% slower is a regression
            
            EXPECT_LT(current_time_ns, baseline_time * regression_threshold)
                << "Performance regression detected in " << benchmark_name
                << ": " << current_time_ns << " ns vs baseline " << baseline_time << " ns";
        }
    }
    
private:
    std::unordered_map<std::string, double> baseline_metrics_;
    
    void load_baseline_metrics() {
        // Load from file or database
        baseline_metrics_["BM_SimpleExpression"] = 150.0;  // ns
        baseline_metrics_["BM_ComplexExpression"] = 400.0;  // ns
        // ... other baselines
    }
};
```

## Best Practices

### 1. Benchmark Design

- **Isolate what you're measuring**: Focus on specific operations
- **Use realistic data**: Test with representative inputs
- **Avoid optimization artifacts**: Use `DoNotOptimize()` and `ClobberMemory()`
- **Run sufficient iterations**: Let the framework determine iteration count
- **Control for external factors**: Run on dedicated hardware when possible

### 2. Result Interpretation

- **Look for trends**: Single runs can be noisy
- **Consider statistical significance**: Use multiple repetitions
- **Account for system variance**: CPU frequency scaling, thermal throttling
- **Compare fairly**: Same compiler, flags, and hardware

### 3. Performance Optimization

- **Profile first**: Identify actual bottlenecks
- **Measure impact**: Benchmark before and after changes
- **Consider trade-offs**: Performance vs. maintainability
- **Document assumptions**: Note performance-critical code paths

## Next Steps

- **[Learn about testing](testing.md)** for correctness verification
- **[Read the contributing guide](contributing.md)** for development workflow
- **[Explore advanced features](../user-guide/advanced-features.md)** for optimization techniques
- **[Check the API reference](../api-reference/)** for implementation details

Happy benchmarking! ⚡
