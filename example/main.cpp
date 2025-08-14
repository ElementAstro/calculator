/**
 * @file main.cpp
 * @brief Comprehensive examples demonstrating the Modern C++ Calculator library
 *
 * This file showcases various features of the calculator library including:
 * - Basic arithmetic operations
 * - Variables and functions
 * - Different numeric types
 * - Error handling
 * - Advanced mathematical operations
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "calculator.hpp"
#include "example_utils.hpp"

// Function prototypes
void demonstrate_basic_arithmetic();
void demonstrate_variables_and_functions();
void demonstrate_numeric_types();
void demonstrate_error_handling();
void demonstrate_advanced_mathematics();
void demonstrate_real_world_applications();

int main() {
    return calculator_utils::run_demo_program(
        "Modern C++ Calculator Library Examples",
        {
            demonstrate_basic_arithmetic,
            demonstrate_variables_and_functions,
            demonstrate_numeric_types,
            demonstrate_error_handling,
            demonstrate_advanced_mathematics,
            demonstrate_real_world_applications
        }
    );
}

/**
 * @brief Demonstrates basic arithmetic operations
 */
void demonstrate_basic_arithmetic() {
    calculator_utils::print_section_header("Basic Arithmetic Operations");

    // Simple arithmetic
    std::cout << "Simple arithmetic:" << std::endl;
    calculator_utils::eval_and_print<double>("2 + 3");
    calculator_utils::eval_and_print<double>("10 - 4");
    calculator_utils::eval_and_print<double>("5 * 6");
    calculator_utils::eval_and_print<double>("15 / 3");

    // Operator precedence
    std::cout << "\nOperator precedence:" << std::endl;
    calculator_utils::eval_and_print<double>("2 + 3 * 4");
    calculator_utils::eval_and_print<double>("(2 + 3) * 4");
    calculator_utils::eval_and_print<double>("2 ** 3 ** 2");

    // Scientific notation
    std::cout << "\nScientific notation:" << std::endl;
    calculator_utils::eval_and_print<double>("1.5e2");
    calculator_utils::eval_and_print<double>("2.5E-3");
    calculator_utils::eval_and_print<double>("6.02e23");
}

/**
 * @brief Demonstrates variables and custom functions
 */
void demonstrate_variables_and_functions() {
    calculator_utils::print_section_header("Variables and Functions");

    calculator::ExpressionParser<double> parser;

    // Set up mathematical environment
    calculator_utils::setup_full_math_environment(parser);

    // Set up variables
    parser.set("x", 5.0);
    parser.set("y", 10.0);
    parser.set("radius", 3.0);

    std::cout << "Using variables:" << std::endl;
    calculator_utils::parser_eval_and_print(parser, "x");
    calculator_utils::parser_eval_and_print(parser, "x + y");
    calculator_utils::parser_eval_and_print(parser, "x * y");

    std::cout << "\nUsing functions:" << std::endl;
    calculator_utils::parser_eval_and_print(parser, "square(5)");
    calculator_utils::parser_eval_and_print(parser, "cube(3)");
    calculator_utils::parser_eval_and_print(parser, "sqrt(16)");

    std::cout << "\nMathematical calculations:" << std::endl;
    calculator_utils::parser_eval_and_print(parser, "sin(pi/2)");
    calculator_utils::parser_eval_and_print(parser, "cos(0)");
    calculator_utils::parser_eval_and_print(parser, "log(e)");

    std::cout << "\nCircle area (π * r²):" << std::endl;
    double area = parser.eval("pi * square(radius)");
    std::cout << "Area = " << area << std::endl;
}

/**
 * @brief Demonstrates different numeric types
 */
void demonstrate_numeric_types() {
    calculator_utils::print_section_header("Different Numeric Types");

    // Integer operations
    std::cout << "Integer operations:" << std::endl;
    calculator_utils::eval_and_print<int>("10 / 3", "10 / 3 (int)");
    calculator_utils::eval_and_print<int>("10 % 3", "10 % 3 (int)");
    calculator_utils::eval_and_print<int>("5 & 3", "5 & 3 (bitwise AND)");
    calculator_utils::eval_and_print<int>("5 | 3", "5 | 3 (bitwise OR)");
    calculator_utils::eval_and_print<int>("8 >> 1", "8 >> 1 (right shift)");

    // Floating-point operations
    std::cout << "\nFloating-point operations:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    calculator_utils::eval_and_print<double>("10.0 / 3.0", "10.0 / 3.0 (double)");
    calculator_utils::eval_and_print<float>("10.0 / 3.0", "10.0 / 3.0 (float)");

    // Type-specific behavior
    std::cout << "\nType-specific behavior:" << std::endl;
    calculator_utils::eval_and_print<double>("2.5 ** 2", "2.5 ** 2 (double)");
    calculator_utils::eval_and_print<int>("2 ** 3", "2 ** 3 (int)");
}

/**
 * @brief Demonstrates error handling patterns
 */
void demonstrate_error_handling() {
    calculator_utils::print_section_header("Error Handling");

    std::vector<std::string> invalid_expressions = {
        "2 + + 3",      // Syntax error (note: this might actually work in some parsers)
        "(2 + 3",       // Unmatched parenthesis
        "unknown_var",  // Undefined variable
        ""              // Empty expression
    };

    calculator_utils::test_invalid_expressions(invalid_expressions);

    // Test division by zero separately with more robust error handling
    std::cout << "\nTesting division by zero (may cause termination):" << std::endl;
    std::cout << "Note: Division by zero handling depends on the calculator implementation." << std::endl;

    // Demonstrate safe evaluation function
    auto safe_eval = calculator_utils::create_safe_evaluator<double>(0.0);

    std::cout << "\nSafe evaluation examples:" << std::endl;
    std::cout << "safe_eval('2 + 3') = " << safe_eval("2 + 3") << std::endl;
    std::cout << "safe_eval('unknown_var') = " << safe_eval("unknown_var") << std::endl;
    std::cout << "Note: Division by zero testing skipped due to library exception handling." << std::endl;
}

/**
 * @brief Demonstrates advanced mathematical operations
 */
void demonstrate_advanced_mathematics() {
    calculator_utils::print_section_header("Advanced Mathematics");

    calculator::ExpressionParser<double> parser;

    // Set up mathematical environment
    calculator_utils::setup_full_math_environment(parser);
    parser.set("tan", [](double x) { return std::tan(x); });

    std::cout << std::fixed << std::setprecision(8);

    // Trigonometric identities
    std::cout << "Trigonometric identities:" << std::endl;
    calculator_utils::parser_eval_and_print(parser, "sin(pi/4) ** 2 + cos(pi/4) ** 2", "sin²(π/4) + cos²(π/4)");
    calculator_utils::parser_eval_and_print(parser, "tan(pi/4)", "tan(π/4)");

    // Exponential and logarithmic
    std::cout << "\nExponential and logarithmic:" << std::endl;
    calculator_utils::parser_eval_and_print(parser, "exp(log(5))", "e^(ln(5))");
    calculator_utils::parser_eval_and_print(parser, "log(exp(3))", "ln(e^3)");

    // Complex expressions
    std::cout << "\nComplex expressions:" << std::endl;
    calculator_utils::parser_eval_and_print(parser, "sqrt(sin(pi/2) ** 2 + cos(pi/2) ** 2)", "√(sin²(π/2) + cos²(π/2))");

    // Nested function calls
    std::cout << "\nNested function calls:" << std::endl;
    calculator_utils::parser_eval_and_print(parser, "sin(cos(pi/4))", "sin(cos(π/4))");
    calculator_utils::parser_eval_and_print(parser, "log(exp(abs(-2)))", "log(exp(abs(-2)))");
}

/**
 * @brief Demonstrates real-world applications
 */
void demonstrate_real_world_applications() {
    calculator_utils::print_section_header("Real-world Applications");

    calculator::ExpressionParser<double> parser;
    calculator_utils::setup_basic_math_constants(parser);
    calculator_utils::setup_common_math_functions(parser);
    calculator_utils::setup_trigonometric_functions(parser);

    // Physics: Projectile motion
    std::cout << "Physics - Projectile motion:" << std::endl;
    parser.set("g", 9.81);      // Gravity (m/s²)
    parser.set("v0", 20.0);     // Initial velocity (m/s)
    parser.set("angle", 45.0);  // Launch angle (degrees)
    parser.set("t", 2.0);       // Time (s)

    // Convert angle to radians and calculate components
    double angle_rad = parser.eval("angle * pi / 180");
    parser.set("angle_rad", angle_rad);

    double vx = parser.eval("v0 * cos(angle_rad)");
    double vy = parser.eval("v0 * sin(angle_rad)");
    parser.set("vx", vx);
    parser.set("vy", vy);

    // Position equations
    double x = parser.eval("vx * t");
    double y = parser.eval("vy * t - 0.5 * g * t ** 2");

    std::cout << "Position at t=2s: x=" << x << "m, y=" << y << "m" << std::endl;

    // Finance: Compound interest
    std::cout << "\nFinance - Compound interest:" << std::endl;
    parser.set("principal", 1000.0);  // Initial amount
    parser.set("rate", 0.05);         // 5% annual rate
    parser.set("time", 10.0);         // 10 years

    double compound_amount = parser.eval("principal * (1 + rate) ** time");
    double interest_earned = parser.eval("principal * (1 + rate) ** time - principal");

    std::cout << "After 10 years: $" << compound_amount << std::endl;
    std::cout << "Interest earned: $" << interest_earned << std::endl;

    // Engineering: Electrical power
    std::cout << "\nEngineering - Electrical power:" << std::endl;
    parser.set("voltage", 120.0);    // Volts
    parser.set("current", 5.0);      // Amperes
    parser.set("resistance", 24.0);  // Ohms

    double power_vi = parser.eval("voltage * current");           // P = VI
    double power_v2r = parser.eval("voltage ** 2 / resistance");  // P = V²/R
    double power_i2r = parser.eval("current ** 2 * resistance");  // P = I²R

    std::cout << "Power (P=VI): " << power_vi << "W" << std::endl;
    std::cout << "Power (P=V²/R): " << power_v2r << "W" << std::endl;
    std::cout << "Power (P=I²R): " << power_i2r << "W" << std::endl;
}