/**
 * @file error_handling_demo.cpp
 * @brief Comprehensive error handling examples for the Modern C++ Calculator
 * library
 *
 * This example demonstrates:
 * - Different types of errors and how to handle them
 * - Error recovery strategies
 * - Robust error reporting
 * - Production-ready error handling patterns
 */

#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "../calculator.hpp"

// Error result types
struct ErrorInfo {
    std::string expression;
    std::string error_message;
    std::string error_type;
    bool recoverable;
};

using EvalResult = std::variant<double, ErrorInfo>;

// Forward declarations
void demonstrate_basic_error_handling();
void demonstrate_error_categorization();
void demonstrate_robust_error_handling();
void demonstrate_production_patterns();

int main() {
    std::cout << "=== Error Handling Demonstration ===" << std::endl;
    std::cout << "Comprehensive examples of error handling patterns.\n"
              << std::endl;

    try {
        demonstrate_basic_error_handling();
        std::cout << std::endl;

        demonstrate_error_categorization();
        std::cout << std::endl;

        demonstrate_robust_error_handling();
        std::cout << std::endl;

        demonstrate_production_patterns();

    } catch (const std::exception& e) {
        std::cerr << "Unexpected error in main: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n=== Error handling examples completed! ===" << std::endl;
    return 0;
}

/**
 * @brief Demonstrates basic error handling patterns
 */
void demonstrate_basic_error_handling() {
    std::cout << "--- Basic Error Handling ---" << std::endl;

    // 1. Simple try-catch pattern
    std::cout << "1. Simple try-catch pattern:" << std::endl;

    std::vector<std::string> test_expressions = {
        "2 + 3",        // Valid
        "5 / 0",        // Division by zero
        "2 + + 3",      // Syntax error
        "(2 + 3",       // Unmatched parenthesis
        "unknown_var",  // Undefined variable
        "",             // Empty expression
        "1..2",         // Invalid number format
    };

    for (const auto& expr : test_expressions) {
        try {
            double result = calculator::eval(expr);
            std::cout << "  ✓ '" << expr << "' = " << result << std::endl;
        } catch (const calculator::error& e) {
            std::cout << "  ✗ '" << expr << "' -> Error: " << e.what()
                      << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  ✗ '" << expr
                      << "' -> Unexpected error: " << e.what() << std::endl;
        }
    }

    // 2. Safe evaluation function
    std::cout << "\n2. Safe evaluation with default values:" << std::endl;

    auto safe_eval = [](const std::string& expr,
                        double default_val = 0.0) -> double {
        try {
            return calculator::eval(expr);
        } catch (const calculator::error&) {
            return default_val;
        }
    };

    std::cout << "  safe_eval('2 + 3') = " << safe_eval("2 + 3") << std::endl;
    std::cout << "  safe_eval('5 / 0', -1) = " << safe_eval("5 / 0", -1.0)
              << std::endl;
    std::cout << "  safe_eval('invalid', 999) = " << safe_eval("invalid", 999.0)
              << std::endl;

    // 3. Optional-based approach
    std::cout << "\n3. Optional-based error handling:" << std::endl;

    auto optional_eval = [](const std::string& expr) -> std::optional<double> {
        try {
            return calculator::eval(expr);
        } catch (const calculator::error&) {
            return std::nullopt;
        }
    };

    auto print_optional_result = [](const std::string& expr,
                                    const std::optional<double>& result) {
        if (result) {
            std::cout << "  '" << expr << "' = " << *result << std::endl;
        } else {
            std::cout << "  '" << expr << "' = <error>" << std::endl;
        }
    };

    print_optional_result("2 + 3", optional_eval("2 + 3"));
    print_optional_result("5 / 0", optional_eval("5 / 0"));
    print_optional_result("invalid", optional_eval("invalid"));
}

/**
 * @brief Demonstrates error categorization and specific handling
 */
void demonstrate_error_categorization() {
    std::cout << "--- Error Categorization ---" << std::endl;

    // Error categorization function
    auto categorize_error = [](const std::string& expr) -> ErrorInfo {
        try {
            double result = calculator::eval(expr);
            return {expr, "No error", "success", true};
        } catch (const calculator::error& e) {
            std::string error_msg = e.what();

            if (error_msg.find("Division by zero") != std::string::npos) {
                return {expr, error_msg, "division_by_zero", false};
            } else if (error_msg.find("Undefined variable") !=
                       std::string::npos) {
                return {expr, error_msg, "undefined_variable", true};
            } else if (error_msg.find("Undefined function") !=
                       std::string::npos) {
                return {expr, error_msg, "undefined_function", true};
            } else if (error_msg.find("Syntax error") != std::string::npos) {
                return {expr, error_msg, "syntax_error", true};
            } else if (error_msg.find("Unexpected token") !=
                       std::string::npos) {
                return {expr, error_msg, "parse_error", true};
            } else {
                return {expr, error_msg, "unknown_error", false};
            }
        } catch (const std::exception& e) {
            return {expr, e.what(), "system_error", false};
        }
    };

    std::vector<std::string> error_test_cases = {
        "2 + 3",     // Success
        "5 / 0",     // Division by zero
        "x + 5",     // Undefined variable
        "sqrt(16)",  // Undefined function
        "2 + + 3",   // Syntax error
        "(2 + 3",    // Parse error
        "",          // Empty expression
    };

    std::cout << "Error categorization results:" << std::endl;
    std::cout << std::left << std::setw(15) << "Expression" << std::setw(20)
              << "Error Type" << std::setw(12) << "Recoverable"
              << "Message" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (const auto& expr : error_test_cases) {
        auto error_info = categorize_error(expr);
        std::cout << std::left << std::setw(15) << ("'" + expr + "'")
                  << std::setw(20) << error_info.error_type << std::setw(12)
                  << (error_info.recoverable ? "Yes" : "No")
                  << error_info.error_message << std::endl;
    }

    // Specific error handling based on type
    std::cout << "\nSpecific error handling strategies:" << std::endl;

    auto handle_specific_error =
        [](const ErrorInfo& error) -> std::optional<double> {
        if (error.error_type == "success") {
            return calculator::eval(error.expression);
        }

        if (error.error_type == "undefined_variable") {
            std::cout << "  Attempting to resolve undefined variable..."
                      << std::endl;
            calculator::ExpressionParser<double> parser;
            parser.set("x", 10.0);  // Default value for x
            parser.set("y", 5.0);   // Default value for y
            try {
                return parser.eval(error.expression);
            } catch (...) {
                return std::nullopt;
            }
        }

        if (error.error_type == "undefined_function") {
            std::cout << "  Attempting to add missing function..." << std::endl;
            calculator::ExpressionParser<double> parser;
            parser.set("sqrt", [](double x) { return std::sqrt(x); });
            parser.set("sin", [](double x) { return std::sin(x); });
            parser.set("cos", [](double x) { return std::cos(x); });
            try {
                return parser.eval(error.expression);
            } catch (...) {
                return std::nullopt;
            }
        }

        if (error.error_type == "division_by_zero") {
            std::cout << "  Division by zero detected - returning infinity"
                      << std::endl;
            return std::numeric_limits<double>::infinity();
        }

        return std::nullopt;
    };

    for (const auto& expr : error_test_cases) {
        auto error_info = categorize_error(expr);
        auto recovery_result = handle_specific_error(error_info);

        if (recovery_result) {
            std::cout << "  Recovered '" << expr << "' = " << *recovery_result
                      << std::endl;
        } else if (error_info.error_type != "success") {
            std::cout << "  Could not recover '" << expr << "'" << std::endl;
        }
    }
}

/**
 * @brief Demonstrates robust error handling with detailed reporting
 */
void demonstrate_robust_error_handling() {
    std::cout << "--- Robust Error Handling ---" << std::endl;

    // Comprehensive error handler class
    class RobustCalculator {
    private:
        mutable calculator::ExpressionParser<double> parser_;
        mutable std::vector<ErrorInfo> error_log_;

    public:
        RobustCalculator() { setup_defaults(); }

        EvalResult evaluate(const std::string& expression) const {
            try {
                double result = parser_.eval(expression);
                return result;
            } catch (const calculator::error& e) {
                ErrorInfo error{expression, e.what(),
                                categorize_calculator_error(e.what()), true};
                error_log_.push_back(error);
                return error;
            } catch (const std::exception& e) {
                ErrorInfo error{expression, e.what(), "system_error", false};
                error_log_.push_back(error);
                return error;
            }
        }

        void set_variable(const std::string& name, double value) {
            parser_.set(name, value);
        }

        void set_function(const std::string& name,
                          std::function<double(double)> func) {
            parser_.set(name, func);
        }

        std::vector<ErrorInfo> get_error_log() const { return error_log_; }

        void clear_error_log() { error_log_.clear(); }

        size_t error_count() const { return error_log_.size(); }

    private:
        void setup_defaults() {
            parser_.set("pi", M_PI);
            parser_.set("e", M_E);
            parser_.set("sqrt", [](double x) { return std::sqrt(x); });
            parser_.set("sin", [](double x) { return std::sin(x); });
            parser_.set("cos", [](double x) { return std::cos(x); });
        }

        std::string categorize_calculator_error(
            const std::string& error_msg) const {
            if (error_msg.find("Division by zero") != std::string::npos)
                return "division_by_zero";
            if (error_msg.find("Undefined variable") != std::string::npos)
                return "undefined_variable";
            if (error_msg.find("Undefined function") != std::string::npos)
                return "undefined_function";
            if (error_msg.find("Syntax error") != std::string::npos)
                return "syntax_error";
            return "unknown_calculator_error";
        }
    };

    // Test the robust calculator
    RobustCalculator calc;
    calc.set_variable("x", 5.0);

    std::vector<std::string> test_expressions = {
        "x + 10",     // Valid
        "sqrt(16)",   // Valid with function
        "y + 5",      // Undefined variable
        "5 / 0",      // Division by zero
        "2 + + 3",    // Syntax error
        "sin(pi/2)",  // Valid with constants
    };

    std::cout << "Robust calculator evaluation:" << std::endl;

    for (const auto& expr : test_expressions) {
        auto result = calc.evaluate(expr);

        if (std::holds_alternative<double>(result)) {
            double value = std::get<double>(result);
            std::cout << "  ✓ '" << expr << "' = " << value << std::endl;
        } else {
            ErrorInfo error = std::get<ErrorInfo>(result);
            std::cout << "  ✗ '" << expr << "' -> " << error.error_type << ": "
                      << error.error_message << std::endl;
        }
    }

    // Error log analysis
    std::cout << "\nError log analysis:" << std::endl;
    std::cout << "Total errors: " << calc.error_count() << std::endl;

    auto error_log = calc.get_error_log();
    std::unordered_map<std::string, int> error_counts;

    for (const auto& error : error_log) {
        error_counts[error.error_type]++;
    }

    std::cout << "Error breakdown:" << std::endl;
    for (const auto& [type, count] : error_counts) {
        std::cout << "  " << type << ": " << count << std::endl;
    }
}

/**
 * @brief Demonstrates production-ready error handling patterns
 */
void demonstrate_production_patterns() {
    std::cout << "--- Production Error Handling Patterns ---" << std::endl;

    // Production calculator with comprehensive error handling
    class ProductionCalculator {
    private:
        mutable calculator::ExpressionParser<double> parser_;
        mutable size_t success_count_ = 0;
        mutable size_t error_count_ = 0;

    public:
        struct EvaluationResult {
            bool success;
            double value;
            std::string error_message;
            std::string error_category;
            std::chrono::microseconds execution_time;
        };

        ProductionCalculator() { setup_production_environment(); }

        EvaluationResult evaluate(const std::string& expression) const {
            auto start_time = std::chrono::high_resolution_clock::now();

            EvaluationResult result;
            result.success = false;
            result.value = 0.0;

            try {
                result.value = parser_.eval(expression);
                result.success = true;
                result.error_category = "none";
                success_count_++;

            } catch (const calculator::error& e) {
                result.error_message = e.what();
                result.error_category = categorize_error(e.what());
                error_count_++;

            } catch (const std::exception& e) {
                result.error_message = "System error: " + std::string(e.what());
                result.error_category = "system_error";
                error_count_++;

            } catch (...) {
                result.error_message = "Unknown error occurred";
                result.error_category = "unknown_error";
                error_count_++;
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            result.execution_time =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    end_time - start_time);

            return result;
        }

        void set_variable(const std::string& name, double value) {
            parser_.set(name, value);
        }

        double get_success_rate() const {
            size_t total = success_count_ + error_count_;
            return total > 0 ? static_cast<double>(success_count_) / total
                             : 0.0;
        }

        size_t get_success_count() const { return success_count_; }
        size_t get_error_count() const { return error_count_; }

    private:
        void setup_production_environment() {
            // Set up comprehensive mathematical environment
            parser_.set("pi", M_PI);
            parser_.set("e", M_E);
            parser_.set("sqrt", [](double x) {
                if (x < 0)
                    throw std::domain_error("Square root of negative number");
                return std::sqrt(x);
            });
            parser_.set("log", [](double x) {
                if (x <= 0)
                    throw std::domain_error("Logarithm of non-positive number");
                return std::log(x);
            });
            parser_.set("sin", [](double x) { return std::sin(x); });
            parser_.set("cos", [](double x) { return std::cos(x); });
        }

        std::string categorize_error(const std::string& error_msg) const {
            if (error_msg.find("Division by zero") != std::string::npos)
                return "math_error";
            if (error_msg.find("Undefined") != std::string::npos)
                return "reference_error";
            if (error_msg.find("Syntax") != std::string::npos)
                return "syntax_error";
            if (error_msg.find("domain_error") != std::string::npos)
                return "domain_error";
            return "unknown_calculator_error";
        }
    };

    // Test production calculator
    ProductionCalculator prod_calc;
    prod_calc.set_variable("x", 10.0);
    prod_calc.set_variable("y", 5.0);

    std::vector<std::string> production_tests = {
        "x + y",        // Valid
        "sqrt(x)",      // Valid
        "log(x)",       // Valid
        "sin(pi/2)",    // Valid
        "x / y",        // Valid
        "sqrt(-1)",     // Domain error
        "log(0)",       // Domain error
        "x / 0",        // Division by zero
        "unknown_var",  // Reference error
        "2 + + 3",      // Syntax error
    };

    std::cout << "Production calculator testing:" << std::endl;
    std::cout << std::left << std::setw(20) << "Expression" << std::setw(10)
              << "Success" << std::setw(15) << "Value/Error" << std::setw(15)
              << "Category"
              << "Time (μs)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (const auto& expr : production_tests) {
        auto result = prod_calc.evaluate(expr);

        std::cout << std::left << std::setw(20) << ("'" + expr + "'")
                  << std::setw(10) << (result.success ? "✓" : "✗");

        if (result.success) {
            std::cout << std::setw(15) << result.value;
        } else {
            std::cout << std::setw(15) << "Error";
        }

        std::cout << std::setw(15) << result.error_category
                  << result.execution_time.count() << std::endl;

        if (!result.success) {
            std::cout << "    Error: " << result.error_message << std::endl;
        }
    }

    // Production statistics
    std::cout << "\nProduction statistics:" << std::endl;
    std::cout << "Success rate: " << (prod_calc.get_success_rate() * 100) << "%"
              << std::endl;
    std::cout << "Successful evaluations: " << prod_calc.get_success_count()
              << std::endl;
    std::cout << "Failed evaluations: " << prod_calc.get_error_count()
              << std::endl;
}
