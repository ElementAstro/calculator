/**
 * @file types.cpp
 * @brief Demonstration of different numeric types in the calculator library
 *
 * This example demonstrates:
 * - Integer operations and bitwise arithmetic
 * - Floating-point precision differences
 * - Type-specific behaviors and limitations
 * - Complex number support
 */

#include <bitset>
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "../calculator.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

template <typename T>
void eval_and_print(const std::string& expr, const std::string& desc = "") {
    try {
        calculator::ExpressionParser<T> parser;
        T result = parser.eval(expr);
        std::cout << (desc.empty() ? expr : desc) << " = " << result << std::endl;
    } catch (const calculator::error& e) {
        std::cout << expr << " -> Error: " << e.what() << std::endl;
    }
}

void demonstrate_integer_operations() {
    std::cout << "=== Integer Operations ===" << std::endl;

    std::cout << "\n--- Basic Arithmetic ---" << std::endl;
    eval_and_print<int>("123 + 456");
    eval_and_print<int>("1000 - 250");
    eval_and_print<int>("12 * 34");
    eval_and_print<int>("100 / 7", "100 / 7 (integer division)");
    eval_and_print<int>("100 % 7", "100 % 7 (modulo)");

    std::cout << "\n--- Bitwise Operations ---" << std::endl;
    eval_and_print<int>("5 & 3", "5 & 3 (AND)");
    eval_and_print<int>("5 | 3", "5 | 3 (OR)");
    eval_and_print<int>("~0", "~0 (NOT)");
    eval_and_print<int>("8 << 2", "8 << 2 (left shift)");
    eval_and_print<int>("32 >> 3", "32 >> 3 (right shift)");

    std::cout << "\n--- Power Operations ---" << std::endl;
    eval_and_print<int>("2 ** 10");
    eval_and_print<int>("3 ** 5");
    eval_and_print<int>("2^8");

    std::cout << "\n--- Hexadecimal ---" << std::endl;
    eval_and_print<int>("0xFF");
    eval_and_print<int>("0x100");
    eval_and_print<int>("0xFF & 0x0F");
}

void demonstrate_floating_point_operations() {
    std::cout << "\n=== Floating-Point Operations ===" << std::endl;

    std::cout << "\n--- Basic Arithmetic ---" << std::endl;
    eval_and_print<double>("3.14 * 2.0");
    eval_and_print<double>("10.0 / 3.0");

    std::cout << "\n--- Scientific Notation ---" << std::endl;
    eval_and_print<double>("1.5e2");
    eval_and_print<double>("2.5E-3");
    eval_and_print<double>("6.02e23", "Avogadro's number");

    std::cout << "\n--- Power Operations ---" << std::endl;
    eval_and_print<double>("2.5 ** 2");
    eval_and_print<double>("8.0 ** (1.0/3.0)", "cube root of 8");
    eval_and_print<double>("16.0 ** 0.5", "square root of 16");
    eval_and_print<double>("2.0 ** -1", "reciprocal of 2");

    std::cout << "\n--- Built-in Functions ---" << std::endl;
    calculator::ExpressionParser<double> parser;
    std::cout << "sin(pi/2) = " << parser.eval("sin(pi/2)") << std::endl;
    std::cout << "cos(0) = " << parser.eval("cos(0)") << std::endl;
    std::cout << "sqrt(2) = " << parser.eval("sqrt(2)") << std::endl;
    std::cout << "ln(e) = " << parser.eval("ln(e)") << std::endl;
    std::cout << "exp(1) = " << parser.eval("exp(1)") << std::endl;
}

void demonstrate_precision_differences() {
    std::cout << "\n=== Precision Differences ===" << std::endl;

    std::cout << "\n--- Float vs Double ---" << std::endl;
    std::cout << std::setprecision(15);

    calculator::ExpressionParser<float> float_parser;
    calculator::ExpressionParser<double> double_parser;

    std::cout << "1.0/3.0 (float):  " << float_parser.eval("1.0 / 3.0") << std::endl;
    std::cout << "1.0/3.0 (double): " << double_parser.eval("1.0 / 3.0") << std::endl;

    std::cout << "\n--- Floating Point Limits ---" << std::endl;
    std::cout << "Float max:  " << std::numeric_limits<float>::max() << std::endl;
    std::cout << "Double max: " << std::numeric_limits<double>::max() << std::endl;
    std::cout << "Float epsilon:  " << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << "Double epsilon: " << std::numeric_limits<double>::epsilon() << std::endl;

    std::cout << "\n--- Mathematical Constants ---" << std::endl;
    std::cout << "pi = " << double_parser.eval("pi") << std::endl;
    std::cout << "e = " << double_parser.eval("e") << std::endl;
    std::cout << "tau = " << double_parser.eval("tau") << std::endl;
    std::cout << "phi = " << double_parser.eval("phi") << std::endl;
}

void demonstrate_complex_numbers() {
    std::cout << "\n=== Complex Number Operations ===" << std::endl;

    calculator::ExpressionParser<std::complex<double>> parser;

    std::cout << "\n--- Basic Complex Arithmetic ---" << std::endl;
    auto z1 = parser.eval("3 + 4*i");
    std::cout << "z1 = 3 + 4i = " << calculator::formatComplex(z1) << std::endl;

    auto z2 = parser.eval("1 + 2*i");
    std::cout << "z2 = 1 + 2i = " << calculator::formatComplex(z2) << std::endl;

    parser.eval("z1 = 3 + 4*i");
    parser.eval("z2 = 1 + 2*i");

    auto sum = parser.eval("z1 + z2");
    std::cout << "z1 + z2 = " << calculator::formatComplex(sum) << std::endl;

    auto product = parser.eval("z1 * z2");
    std::cout << "z1 * z2 = " << calculator::formatComplex(product) << std::endl;

    std::cout << "\n--- Complex Functions ---" << std::endl;
    auto abs_z1 = parser.eval("abs(z1)");
    std::cout << "|z1| = " << abs_z1.real() << std::endl;

    auto conj_z1 = parser.eval("conj(z1)");
    std::cout << "conj(z1) = " << calculator::formatComplex(conj_z1) << std::endl;

    std::cout << "\n--- Imaginary Unit ---" << std::endl;
    auto i_squared = parser.eval("i * i");
    std::cout << "i^2 = " << calculator::formatComplex(i_squared) << std::endl;

    auto euler = parser.eval("exp(i * pi)");
    std::cout << "e^(i*pi) = " << calculator::formatComplex(euler) << " (Euler's identity)" << std::endl;
}

void demonstrate_type_comparison() {
    std::cout << "\n=== Type Comparison ===" << std::endl;

    std::cout << "\n--- Same Expression, Different Types ---" << std::endl;
    std::cout << "Expression: 10 / 3" << std::endl;
    eval_and_print<int>("10 / 3", "  int result");
    eval_and_print<double>("10 / 3", "  double result");

    std::cout << "\nExpression: 2 ** 10" << std::endl;
    eval_and_print<int>("2 ** 10", "  int result");
    eval_and_print<long long>("2 ** 10", "  long long result");
    eval_and_print<double>("2 ** 10", "  double result");
}

int main() {
    std::cout << "=== Numeric Types Demonstration ===" << std::endl;
    std::cout << "Showing different numeric type behaviors in the calculator.\n" << std::endl;

    try {
        demonstrate_integer_operations();
        demonstrate_floating_point_operations();
        demonstrate_precision_differences();
        demonstrate_complex_numbers();
        demonstrate_type_comparison();

    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n=== Demonstration completed! ===" << std::endl;
    return 0;
}
