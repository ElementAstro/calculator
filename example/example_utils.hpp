/**
 * @file example_utils.hpp
 * @brief Utility functions to reduce code duplication across calculator examples and tests
 * 
 * This header provides common functionality used across multiple calculator demonstration
 * and test files, including:
 * - Mathematical setup functions for constants and functions
 * - Output formatting utilities
 * - Error handling patterns
 * - Performance benchmarking utilities
 * - Common evaluation patterns
 */

#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

#include "calculator.hpp"

namespace calculator_utils {

/**
 * @brief Prints a formatted section header
 */
inline void print_section_header(const std::string& title) {
    std::cout << "--- " << title << " ---" << std::endl;
}

/**
 * @brief Prints a formatted program completion message
 */
inline void print_completion_message(const std::string& program_name) {
    std::cout << "\n=== " << program_name << " ===" << std::endl;
}

/**
 * @brief Sets up basic mathematical constants in a parser
 */
template<typename T>
inline void setup_basic_math_constants(calculator::ExpressionParser<T>& parser) {
    parser.set("pi", static_cast<T>(M_PI));
    parser.set("e", static_cast<T>(M_E));
}

/**
 * @brief Sets up trigonometric functions in a parser
 */
template<typename T>
inline void setup_trigonometric_functions(calculator::ExpressionParser<T>& parser) {
    parser.set("sin", [](T x) { return static_cast<T>(std::sin(static_cast<double>(x))); });
    parser.set("cos", [](T x) { return static_cast<T>(std::cos(static_cast<double>(x))); });
    parser.set("tan", [](T x) { return static_cast<T>(std::tan(static_cast<double>(x))); });
}

/**
 * @brief Sets up logarithmic and exponential functions in a parser
 */
template<typename T>
inline void setup_logarithmic_functions(calculator::ExpressionParser<T>& parser) {
    parser.set("log", [](T x) { return static_cast<T>(std::log(static_cast<double>(x))); });
    parser.set("ln", [](T x) { return static_cast<T>(std::log(static_cast<double>(x))); });
    parser.set("exp", [](T x) { return static_cast<T>(std::exp(static_cast<double>(x))); });
}

/**
 * @brief Sets up common mathematical functions in a parser
 */
template<typename T>
inline void setup_common_math_functions(calculator::ExpressionParser<T>& parser) {
    parser.set("sqrt", [](T x) { return static_cast<T>(std::sqrt(static_cast<double>(x))); });
    parser.set("abs", [](T x) { return static_cast<T>(std::abs(static_cast<double>(x))); });
    parser.set("square", [](T x) { return x * x; });
    parser.set("cube", [](T x) { return x * x * x; });
}

/**
 * @brief Sets up a complete mathematical environment with constants and functions
 */
template<typename T>
inline void setup_full_math_environment(calculator::ExpressionParser<T>& parser) {
    setup_basic_math_constants(parser);
    setup_trigonometric_functions(parser);
    setup_logarithmic_functions(parser);
    setup_common_math_functions(parser);
}

/**
 * @brief Evaluates an expression and prints the result with optional description
 */
template<typename T>
inline void eval_and_print(const std::string& expression, const std::string& description = "") {
    try {
        T result = calculator::eval<T>(expression);
        if (description.empty()) {
            std::cout << expression << " = " << result << std::endl;
        } else {
            std::cout << description << " = " << result << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error evaluating '" << expression << "': " << e.what() << std::endl;
    }
}

/**
 * @brief Evaluates an expression using a parser and prints the result
 */
template<typename T>
inline void parser_eval_and_print(calculator::ExpressionParser<T>& parser, 
                                  const std::string& expression, 
                                  const std::string& description = "") {
    try {
        T result = parser.eval(expression);
        if (description.empty()) {
            std::cout << expression << " = " << result << std::endl;
        } else {
            std::cout << description << " = " << result << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error evaluating '" << expression << "': " << e.what() << std::endl;
    }
}

/**
 * @brief Safely evaluates an expression, returning a default value on error
 */
template<typename T>
inline T safe_eval(const std::string& expression, T default_value = T{}) {
    try {
        return calculator::eval<T>(expression);
    } catch (const std::exception&) {
        return default_value;
    }
}

/**
 * @brief Tests a list of invalid expressions and reports errors
 */
inline void test_invalid_expressions(const std::vector<std::string>& expressions) {
    for (const auto& expr : expressions) {
        try {
            double result = calculator::eval(expr);
            std::cout << "Unexpected success: " << expr << " = " << result << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Error in '" << expr << "': " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Standard error in '" << expr << "': " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Unknown error in '" << expr << "'" << std::endl;
        }
    }
}

/**
 * @brief Benchmarks an expression evaluation over multiple iterations
 */
template<typename T>
inline std::chrono::microseconds benchmark_eval(const std::string& expression, int iterations) {
    try {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            T result = calculator::eval<T>(expression);
            (void)result;  // Suppress unused variable warning
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    } catch (const std::exception& e) {
        std::cerr << "Benchmark error with expression '" << expression << "': " << e.what() << std::endl;
        return std::chrono::microseconds(0);
    }
}

/**
 * @brief Runs a demo program with consistent error handling and formatting
 */
inline int run_demo_program(const std::string& title, 
                           const std::vector<std::function<void()>>& demo_functions) {
    std::cout << "=== " << title << " ===" << std::endl;
    std::cout << "This program demonstrates various features of the calculator library.\n" << std::endl;

    try {
        for (size_t i = 0; i < demo_functions.size(); ++i) {
            demo_functions[i]();
            if (i < demo_functions.size() - 1) {
                std::cout << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    print_completion_message("All examples completed successfully!");
    return 0;
}

/**
 * @brief Creates a safe evaluation function with custom error handling
 */
template<typename T>
inline std::function<T(const std::string&)> create_safe_evaluator(T default_value = T{}) {
    return [default_value](const std::string& expression) -> T {
        try {
            return calculator::eval<T>(expression);
        } catch (const std::runtime_error& e) {
            std::cerr << "Safe eval error: " << e.what() << std::endl;
            return default_value;
        } catch (const std::exception& e) {
            std::cerr << "Safe eval error: " << e.what() << std::endl;
            return default_value;
        } catch (...) {
            std::cerr << "Safe eval: Unknown error occurred" << std::endl;
            return default_value;
        }
    };
}

} // namespace calculator_utils
