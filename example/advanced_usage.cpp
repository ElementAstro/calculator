/**
 * @file advanced_usage.cpp
 * @brief Advanced usage patterns for the Modern C++ Calculator library
 *
 * This example demonstrates:
 * - Performance optimization techniques
 * - Complex mathematical systems
 * - Error recovery strategies
 * - Memory management patterns
 * - Thread safety considerations
 */

#include <bitset>
#include <chrono>
#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../calculator.hpp"

// Forward declarations
void demonstrate_performance_optimization();
void demonstrate_scientific_computing();
void demonstrate_error_recovery();
void demonstrate_memory_management();
void demonstrate_thread_safety();

int main() {
    std::cout << "=== Advanced Usage Examples ===" << std::endl;
    std::cout
        << "Demonstrating advanced patterns and optimization techniques.\n"
        << std::endl;

    try {
        demonstrate_performance_optimization();
        std::cout << std::endl;

        demonstrate_scientific_computing();
        std::cout << std::endl;

        demonstrate_error_recovery();
        std::cout << std::endl;

        demonstrate_memory_management();
        std::cout << std::endl;

        demonstrate_thread_safety();

    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n=== Advanced examples completed! ===" << std::endl;
    return 0;
}

/**
 * @brief Demonstrates performance optimization techniques
 */
void demonstrate_performance_optimization() {
    std::cout << "--- Performance Optimization ---" << std::endl;

    // 1. Parser reuse vs recreation
    const int iterations = 10000;

    // Benchmark: Creating new parser each time (inefficient)
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        calculator::ExpressionParser<double> parser;
        parser.set("x", static_cast<double>(i));
        double result = parser.eval("x * 2 + 1");
        (void)result;  // Suppress unused variable warning
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time_recreate =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Benchmark: Reusing parser (efficient)
    calculator::ExpressionParser<double> reused_parser;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        reused_parser.set("x", static_cast<double>(i));
        double result = reused_parser.eval("x * 2 + 1");
        (void)result;  // Suppress unused variable warning
    }
    end = std::chrono::high_resolution_clock::now();
    auto time_reuse =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Parser recreation: " << time_recreate.count() << " μs"
              << std::endl;
    std::cout << "Parser reuse: " << time_reuse.count() << " μs" << std::endl;
    std::cout << "Speedup: "
              << (double)time_recreate.count() / time_reuse.count() << "x"
              << std::endl;

    // 2. Expression caching
    std::unordered_map<std::string, double> expression_cache;

    auto cached_eval = [&](const std::string& expr) -> double {
        auto it = expression_cache.find(expr);
        if (it != expression_cache.end()) {
            return it->second;
        }

        double result = calculator::eval(expr);
        expression_cache[expr] = result;
        return result;
    };

    std::cout << "\nExpression caching example:" << std::endl;
    std::cout << "First evaluation: " << cached_eval("2 + 3 * 4") << std::endl;
    std::cout << "Cached evaluation: " << cached_eval("2 + 3 * 4") << std::endl;
    std::cout << "Cache size: " << expression_cache.size() << std::endl;
}

/**
 * @brief Demonstrates scientific computing applications
 */
void demonstrate_scientific_computing() {
    std::cout << "--- Scientific Computing ---" << std::endl;

    calculator::ExpressionParser<double> parser;

    // Set up mathematical functions and constants
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("cos", [](double x) { return std::cos(x); });
    parser.set("exp", [](double x) { return std::exp(x); });
    parser.set("log", [](double x) { return std::log(x); });
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    parser.set("abs", [](double x) { return std::abs(x); });
    parser.set("pi", M_PI);
    parser.set("e", M_E);

    // 1. Numerical integration using Simpson's rule
    auto simpson_integrate = [&](const std::string& function, double a,
                                 double b, int n = 1000) {
        if (n % 2 != 0)
            n++;  // Ensure n is even

        double h = (b - a) / n;
        double sum = 0.0;

        for (int i = 0; i <= n; ++i) {
            double x = a + i * h;
            parser.set("x", x);
            double fx = parser.eval(function);

            if (i == 0 || i == n) {
                sum += fx;
            } else if (i % 2 == 1) {
                sum += 4 * fx;
            } else {
                sum += 2 * fx;
            }
        }

        return sum * h / 3.0;
    };

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Numerical integration examples:" << std::endl;
    std::cout << "∫₀¹ x² dx = " << simpson_integrate("x ** 2", 0.0, 1.0)
              << " (exact: 0.333333)" << std::endl;
    std::cout << "∫₀π sin(x) dx = " << simpson_integrate("sin(x)", 0.0, M_PI)
              << " (exact: 2.0)" << std::endl;

    // 2. Root finding using Newton's method
    auto newton_method = [&](const std::string& f, const std::string& df,
                             double x0, double tol = 1e-10) {
        double x = x0;
        for (int i = 0; i < 100; ++i) {
            parser.set("x", x);
            double fx = parser.eval(f);
            double dfx = parser.eval(df);

            if (std::abs(fx) < tol)
                break;
            if (std::abs(dfx) < tol)
                throw std::runtime_error("Derivative too small");

            x = x - fx / dfx;
        }
        return x;
    };

    std::cout << "\nRoot finding examples:" << std::endl;
    try {
        double root1 = newton_method("x ** 2 - 2", "2 * x", 1.5);
        std::cout << "Root of x² - 2 = 0: " << root1 << " (√2 ≈ "
                  << std::sqrt(2) << ")" << std::endl;

        double root2 = newton_method("cos(x)", "-sin(x)", 1.0);
        std::cout << "Root of cos(x) = 0: " << root2 << " (π/2 ≈ " << M_PI / 2
                  << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Root finding error: " << e.what() << std::endl;
    }

    // 3. Statistical calculations
    std::vector<double> data = {2.1, 2.3, 2.0, 2.4, 2.2,
                                2.5, 2.1, 2.3, 2.2, 2.4};

    // Calculate statistics using the calculator
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    parser.set("sum", sum);
    parser.set("n", static_cast<double>(data.size()));

    double mean = parser.eval("sum / n");
    parser.set("mean", mean);

    // Calculate variance
    double sum_squares = 0.0;
    for (double value : data) {
        parser.set("x", value);
        sum_squares += parser.eval("(x - mean) ** 2");
    }
    parser.set("sum_squares", sum_squares);

    double variance = parser.eval("sum_squares / (n - 1)");
    double std_dev = parser.eval("sqrt(" + std::to_string(variance) + ")");

    std::cout << "\nStatistical analysis:" << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Variance: " << variance << std::endl;
    std::cout << "Standard deviation: " << std_dev << std::endl;
}

/**
 * @brief Demonstrates error recovery strategies
 */
void demonstrate_error_recovery() {
    std::cout << "--- Error Recovery Strategies ---" << std::endl;

    // 1. Graceful degradation
    auto safe_eval_with_default = [](const std::string& expr,
                                     double default_value = 0.0) -> double {
        try {
            return calculator::eval(expr);
        } catch (const calculator::error& e) {
            std::cerr << "Calculator error (using default): " << e.what()
                      << std::endl;
            return default_value;
        } catch (const std::exception& e) {
            std::cerr << "Standard error (using default): " << e.what()
                      << std::endl;
            return default_value;
        }
    };

    std::cout << "Safe evaluation with defaults:" << std::endl;
    std::cout << "Valid: " << safe_eval_with_default("2 + 3") << std::endl;
    std::cout << "Invalid: " << safe_eval_with_default("2 + + 3", -1.0)
              << std::endl;

    // 2. Error categorization and handling
    auto categorized_eval =
        [](const std::string& expr) -> std::pair<double, std::string> {
        try {
            double result = calculator::eval(expr);
            return {result, "success"};
        } catch (const calculator::error& e) {
            std::string error_msg = e.what();

            if (error_msg.find("Division by zero") != std::string::npos) {
                return {std::numeric_limits<double>::infinity(),
                        "division_by_zero"};
            } else if (error_msg.find("Undefined variable") !=
                       std::string::npos) {
                return {0.0, "undefined_variable"};
            } else if (error_msg.find("Syntax error") != std::string::npos) {
                return {std::numeric_limits<double>::quiet_NaN(),
                        "syntax_error"};
            } else {
                return {0.0, "unknown_error"};
            }
        }
    };

    std::cout << "\nCategorized error handling:" << std::endl;
    std::vector<std::string> test_expressions = {"2 + 3", "5 / 0", "2 + + 3",
                                                 "unknown_var"};

    for (const auto& expr : test_expressions) {
        auto [result, category] = categorized_eval(expr);
        std::cout << "'" << expr << "' -> " << result << " (" << category << ")"
                  << std::endl;
    }

    // 3. Retry mechanism with expression correction
    auto auto_correct_eval = [](std::string expr) -> double {
        // Simple auto-correction attempts
        std::vector<std::string> corrections;
        corrections.push_back(expr);  // Original
        corrections.push_back(
            std::regex_replace(expr, std::regex(R"(\+\s*\+)"), "+"));  // Fix ++
        corrections.push_back(
            std::regex_replace(expr, std::regex(R"(-\s*-)"), "+"));  // Fix --

        for (const auto& corrected : corrections) {
            try {
                return calculator::eval(corrected);
            } catch (const calculator::error&) {
                continue;  // Try next correction
            }
        }

        throw std::runtime_error("Could not auto-correct expression: " + expr);
    };

    std::cout << "\nAuto-correction attempts:" << std::endl;
    try {
        std::cout << "auto_correct('2 + 3') = " << auto_correct_eval("2 + 3")
                  << std::endl;
        std::cout << "auto_correct('2 + + 3') = "
                  << auto_correct_eval("2 + + 3") << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Auto-correction failed: " << e.what() << std::endl;
    }
}

/**
 * @brief Demonstrates memory management patterns
 */
void demonstrate_memory_management() {
    std::cout << "--- Memory Management ---" << std::endl;

    // 1. RAII pattern with calculator wrapper
    class CalculatorRAII {
    private:
        std::unique_ptr<calculator::ExpressionParser<double>> parser_;

    public:
        CalculatorRAII()
            : parser_(
                  std::make_unique<calculator::ExpressionParser<double>>()) {
            setup_defaults();
        }

        ~CalculatorRAII() {
            // Automatic cleanup
            std::cout << "Calculator RAII destructor called" << std::endl;
        }

        // Move semantics
        CalculatorRAII(CalculatorRAII&&) = default;
        CalculatorRAII& operator=(CalculatorRAII&&) = default;

        // Disable copying
        CalculatorRAII(const CalculatorRAII&) = delete;
        CalculatorRAII& operator=(const CalculatorRAII&) = delete;

        double eval(const std::string& expr) { return parser_->eval(expr); }

        void set_variable(const std::string& name, double value) {
            parser_->set(name, value);
        }

    private:
        void setup_defaults() {
            parser_->set("pi", M_PI);
            parser_->set("e", M_E);
        }
    };

    std::cout << "RAII pattern demonstration:" << std::endl;
    {
        CalculatorRAII calc;
        calc.set_variable("x", 42.0);
        std::cout << "Result: " << calc.eval("x * 2") << std::endl;
    }  // Destructor called here

    // 2. Object pool pattern
    class CalculatorPool {
    private:
        std::vector<std::unique_ptr<calculator::ExpressionParser<double>>>
            pool_;
        std::mutex mutex_;

    public:
        CalculatorPool(size_t size) {
            for (size_t i = 0; i < size; ++i) {
                auto parser =
                    std::make_unique<calculator::ExpressionParser<double>>();
                parser->set("pi", M_PI);
                parser->set("e", M_E);
                pool_.push_back(std::move(parser));
            }
        }

        double eval(const std::string& expr) {
            std::lock_guard<std::mutex> lock(mutex_);

            if (!pool_.empty()) {
                auto parser = std::move(pool_.back());
                pool_.pop_back();

                double result = parser->eval(expr);

                pool_.push_back(std::move(parser));
                return result;
            }

            // Fallback: create temporary parser
            calculator::ExpressionParser<double> temp_parser;
            return temp_parser.eval(expr);
        }

        size_t available() const {
            std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));
            return pool_.size();
        }
    };

    std::cout << "\nObject pool demonstration:" << std::endl;
    CalculatorPool pool(3);
    std::cout << "Pool size: " << pool.available() << std::endl;
    std::cout << "Evaluation: " << pool.eval("2 + 3 * 4") << std::endl;
    std::cout << "Pool size after use: " << pool.available() << std::endl;
}

/**
 * @brief Demonstrates thread safety considerations
 */
void demonstrate_thread_safety() {
    std::cout << "--- Thread Safety ---" << std::endl;

    // 1. Thread-local storage
    thread_local calculator::ExpressionParser<double> tls_parser;

    auto thread_function = [](int thread_id) {
        // Each thread has its own parser
        tls_parser.set("thread_id", static_cast<double>(thread_id));
        tls_parser.set("pi", M_PI);

        double result = tls_parser.eval("thread_id * pi");
        std::cout << "Thread " << thread_id << " result: " << result
                  << std::endl;
    };

    std::cout << "Thread-local storage example:" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(thread_function, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    // 2. Parallel computation with futures
    auto parallel_eval = [](const std::vector<std::string>& expressions) {
        std::vector<std::future<double>> futures;

        for (const auto& expr : expressions) {
            futures.push_back(std::async(std::launch::async, [expr]() {
                calculator::ExpressionParser<double> local_parser;
                local_parser.set("pi", M_PI);
                return local_parser.eval(expr);
            }));
        }

        std::vector<double> results;
        for (auto& future : futures) {
            results.push_back(future.get());
        }

        return results;
    };

    std::cout << "\nParallel evaluation example:" << std::endl;
    std::vector<std::string> expressions = {"2 + 3", "4 * 5", "pi * 2",
                                            "10 / 2"};
    auto results = parallel_eval(expressions);

    for (size_t i = 0; i < expressions.size(); ++i) {
        std::cout << expressions[i] << " = " << results[i] << std::endl;
    }
}
