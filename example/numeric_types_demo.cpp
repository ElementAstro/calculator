/**
 * @file numeric_types_demo.cpp
 * @brief Comprehensive demonstration of different numeric types in the
 * calculator library
 *
 * This example demonstrates:
 * - Integer operations and bitwise arithmetic
 * - Floating-point precision differences
 * - Type-specific behaviors and limitations
 * - Performance characteristics of different types
 * - Best practices for type selection
 */

#include <bitset>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../calculator.hpp"

// Forward declarations
void demonstrate_integer_operations();
void demonstrate_floating_point_operations();
void demonstrate_type_specific_behaviors();
void demonstrate_precision_differences();
void demonstrate_performance_comparison();
void demonstrate_best_practices();

int main() {
    std::cout << "=== Numeric Types Demonstration ===" << std::endl;
    std::cout << "Exploring different numeric types and their behaviors.\n"
              << std::endl;

    try {
        demonstrate_integer_operations();
        std::cout << std::endl;

        demonstrate_floating_point_operations();
        std::cout << std::endl;

        demonstrate_type_specific_behaviors();
        std::cout << std::endl;

        demonstrate_precision_differences();
        std::cout << std::endl;

        demonstrate_performance_comparison();
        std::cout << std::endl;

        demonstrate_best_practices();

    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n=== Numeric types examples completed! ===" << std::endl;
    return 0;
}

/**
 * @brief Demonstrates integer operations and bitwise arithmetic
 */
void demonstrate_integer_operations() {
    std::cout << "--- Integer Operations ---" << std::endl;

    // Basic arithmetic with integers
    std::cout << "Basic integer arithmetic:" << std::endl;
    std::cout << "10 + 5 = " << calculator::eval<int>("10 + 5") << std::endl;
    std::cout << "10 - 5 = " << calculator::eval<int>("10 - 5") << std::endl;
    std::cout << "10 * 5 = " << calculator::eval<int>("10 * 5") << std::endl;
    std::cout << "10 / 5 = " << calculator::eval<int>("10 / 5") << std::endl;

    // Integer division behavior (truncation)
    std::cout << "\nInteger division (truncation toward zero):" << std::endl;
    std::cout << "7 / 3 = " << calculator::eval<int>("7 / 3")
              << std::endl;  // 2
    std::cout << "-7 / 3 = " << calculator::eval<int>("-7 / 3")
              << std::endl;  // -2
    std::cout << "7 / -3 = " << calculator::eval<int>("7 / -3")
              << std::endl;  // -2
    std::cout << "-7 / -3 = " << calculator::eval<int>("-7 / -3")
              << std::endl;  // 2

    // Modulo operations
    std::cout << "\nModulo operations:" << std::endl;
    std::cout << "7 % 3 = " << calculator::eval<int>("7 % 3")
              << std::endl;  // 1
    std::cout << "-7 % 3 = " << calculator::eval<int>("-7 % 3")
              << std::endl;  // -1
    std::cout << "7 % -3 = " << calculator::eval<int>("7 % -3")
              << std::endl;  // 1
    std::cout << "-7 % -3 = " << calculator::eval<int>("-7 % -3")
              << std::endl;  // -1

    // Bitwise operations
    std::cout << "\nBitwise operations:" << std::endl;
    std::cout << "Binary representations:" << std::endl;
    std::cout << "5 = 101₂, 3 = 011₂" << std::endl;

    std::cout << "5 | 3 (OR) = " << calculator::eval<int>("5 | 3")
              << std::endl;  // 7 (111₂)
    std::cout << "5 & 3 (AND) = " << calculator::eval<int>("5 & 3")
              << std::endl;  // 1 (001₂)
    std::cout << "5 ^ 3 (XOR) = " << calculator::eval<int>("5 ^ 3")
              << std::endl;  // 6 (110₂)
    std::cout << "~5 (NOT) = " << calculator::eval<int>("~5")
              << std::endl;  // -6 (two's complement)

    // Bit shifting
    std::cout << "\nBit shifting operations:" << std::endl;
    std::cout << "8 << 1 (left shift) = " << calculator::eval<int>("8 << 1")
              << std::endl;  // 16
    std::cout << "8 << 2 (left shift) = " << calculator::eval<int>("8 << 2")
              << std::endl;  // 32
    std::cout << "16 >> 1 (right shift) = " << calculator::eval<int>("16 >> 1")
              << std::endl;  // 8
    std::cout << "16 >> 2 (right shift) = " << calculator::eval<int>("16 >> 2")
              << std::endl;  // 4

    // Complex bitwise expressions
    std::cout << "\nComplex bitwise expressions:" << std::endl;
    std::cout << "(5 | 3) & (7 ^ 2) = "
              << calculator::eval<int>("(5 | 3) & (7 ^ 2)") << std::endl;
    std::cout << "~(5 & 3) | (2 << 1) = "
              << calculator::eval<int>("~(5 & 3) | (2 << 1)") << std::endl;

    // Integer limits
    std::cout << "\nInteger limits:" << std::endl;
    std::cout << "INT_MAX = " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "INT_MIN = " << std::numeric_limits<int>::min() << std::endl;

    try {
        int large_result = calculator::eval<int>("2000000000");
        std::cout << "Large integer: " << large_result << std::endl;
    } catch (const calculator::error& e) {
        std::cout << "Error with large integer: " << e.what() << std::endl;
    }
}

/**
 * @brief Demonstrates floating-point operations
 */
void demonstrate_floating_point_operations() {
    std::cout << "--- Floating-Point Operations ---" << std::endl;

    // Basic floating-point arithmetic
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Basic floating-point arithmetic:" << std::endl;
    std::cout << "10.5 + 5.3 = " << calculator::eval<double>("10.5 + 5.3")
              << std::endl;
    std::cout << "10.7 - 5.2 = " << calculator::eval<double>("10.7 - 5.2")
              << std::endl;
    std::cout << "3.14 * 2.0 = " << calculator::eval<double>("3.14 * 2.0")
              << std::endl;
    std::cout << "10.0 / 3.0 = " << calculator::eval<double>("10.0 / 3.0")
              << std::endl;

    // Scientific notation
    std::cout << "\nScientific notation:" << std::endl;
    std::cout << "1.5e2 = " << calculator::eval<double>("1.5e2") << std::endl;
    std::cout << "2.5E-3 = " << calculator::eval<double>("2.5E-3") << std::endl;
    std::cout << "6.02e23 = " << calculator::eval<double>("6.02e23")
              << std::endl;
    std::cout << "1.38e-23 = " << calculator::eval<double>("1.38e-23")
              << std::endl;

    // Exponentiation with floating-point
    std::cout << "\nExponentiation:" << std::endl;
    std::cout << "2.5 ** 2 = " << calculator::eval<double>("2.5 ** 2")
              << std::endl;
    std::cout << "8.0 ** (1.0/3.0) = "
              << calculator::eval<double>("8.0 ** (1.0/3.0)")
              << std::endl;  // Cube root
    std::cout << "16.0 ** 0.5 = " << calculator::eval<double>("16.0 ** 0.5")
              << std::endl;  // Square root
    std::cout << "2.0 ** -1 = " << calculator::eval<double>("2.0 ** -1")
              << std::endl;  // Reciprocal

    // Float vs Double precision
    std::cout << "\nFloat vs Double precision:" << std::endl;
    std::cout << std::setprecision(10);

    float float_result = calculator::eval<float>("1.0 / 3.0");
    double double_result = calculator::eval<double>("1.0 / 3.0");

    std::cout << "1/3 as float:  " << float_result << std::endl;
    std::cout << "1/3 as double: " << double_result << std::endl;

    // Very large and small numbers
    std::cout << "\nVery large and small numbers:" << std::endl;
    std::cout << std::scientific;
    std::cout << "1e100 = " << calculator::eval<double>("1e100") << std::endl;
    std::cout << "1e-100 = " << calculator::eval<double>("1e-100") << std::endl;
    std::cout << "1e308 = " << calculator::eval<double>("1e308") << std::endl;

    // Floating-point limits
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nFloating-point limits:" << std::endl;
    std::cout << "Float max: " << std::numeric_limits<float>::max()
              << std::endl;
    std::cout << "Float min: " << std::numeric_limits<float>::min()
              << std::endl;
    std::cout << "Double max: " << std::numeric_limits<double>::max()
              << std::endl;
    std::cout << "Double min: " << std::numeric_limits<double>::min()
              << std::endl;
}

/**
 * @brief Demonstrates type-specific behaviors and limitations
 */
void demonstrate_type_specific_behaviors() {
    std::cout << "--- Type-Specific Behaviors ---" << std::endl;

    // Operations supported by each type
    std::cout << "Operations supported by each type:" << std::endl;

    std::cout << "\nInteger type supports:" << std::endl;
    std::cout << "  ✓ Arithmetic: +, -, *, /, %" << std::endl;
    std::cout << "  ✓ Bitwise: |, &, ^, <<, >>, ~" << std::endl;
    std::cout << "  ✓ Exponentiation: **" << std::endl;
    std::cout << "  ✓ Scientific notation: e, E" << std::endl;

    std::cout << "\nFloat/Double types support:" << std::endl;
    std::cout << "  ✓ Arithmetic: +, -, *, /" << std::endl;
    std::cout << "  ✗ Modulo: % (not supported)" << std::endl;
    std::cout << "  ✗ Bitwise: |, &, ^, <<, >>, ~ (not supported)" << std::endl;
    std::cout << "  ✓ Exponentiation: **" << std::endl;
    std::cout << "  ✓ Scientific notation: e, E" << std::endl;

    // Demonstrate unsupported operations
    std::cout << "\nDemonstrating unsupported operations:" << std::endl;

    // Try modulo with floating-point (should fail)
    try {
        double result = calculator::eval<double>("7.5 % 2.3");
        std::cout << "Unexpected success: 7.5 % 2.3 = " << result << std::endl;
    } catch (const calculator::error& e) {
        std::cout << "Expected error: Modulo with double - " << e.what()
                  << std::endl;
    }

    // Try bitwise with floating-point (should fail)
    try {
        double result = calculator::eval<double>("5.5 & 3.2");
        std::cout << "Unexpected success: 5.5 & 3.2 = " << result << std::endl;
    } catch (const calculator::error& e) {
        std::cout << "Expected error: Bitwise AND with double - " << e.what()
                  << std::endl;
    }

    // Try float literal in integer context (should fail)
    try {
        int result = calculator::eval<int>("3.14");
        std::cout << "Unexpected success: 3.14 as int = " << result
                  << std::endl;
    } catch (const calculator::error& e) {
        std::cout << "Expected error: Float literal in int context - "
                  << e.what() << std::endl;
    }

    // Type coercion examples
    std::cout << "\nType behavior examples:" << std::endl;
    std::cout << "Integer division: 10 / 3 = "
              << calculator::eval<int>("10 / 3") << std::endl;
    std::cout << "Float division: 10.0 / 3.0 = "
              << calculator::eval<double>("10.0 / 3.0") << std::endl;

    std::cout << "Integer exponentiation: 2 ** 3 = "
              << calculator::eval<int>("2 ** 3") << std::endl;
    std::cout << "Float exponentiation: 2.0 ** 3.0 = "
              << calculator::eval<double>("2.0 ** 3.0") << std::endl;
}

/**
 * @brief Demonstrates precision differences between types
 */
void demonstrate_precision_differences() {
    std::cout << "--- Precision Differences ---" << std::endl;

    // Precision comparison
    std::cout << std::setprecision(15);
    std::cout << "Precision comparison for 1/3:" << std::endl;

    float float_third = calculator::eval<float>("1.0 / 3.0");
    double double_third = calculator::eval<double>("1.0 / 3.0");

    std::cout << "Float (7 digits):  " << float_third << std::endl;
    std::cout << "Double (15 digits): " << double_third << std::endl;
    std::cout << "Exact value:        0.333333333333333..." << std::endl;

    // Precision loss in large numbers
    std::cout << "\nPrecision loss in large numbers:" << std::endl;

    float large_float = calculator::eval<float>("1234567.0 + 1.0");
    float large_float_base = calculator::eval<float>("1234567.0");

    std::cout << "Float: 1234567.0 + 1.0 = " << large_float << std::endl;
    std::cout << "Float: 1234567.0 = " << large_float_base << std::endl;
    std::cout << "Difference: " << (large_float - large_float_base)
              << std::endl;

    if (large_float == large_float_base) {
        std::cout << "Precision lost: addition of 1.0 has no effect!"
                  << std::endl;
    }

    // Accumulation errors
    std::cout << "\nAccumulation errors:" << std::endl;

    calculator::ExpressionParser<float> float_parser;
    calculator::ExpressionParser<double> double_parser;

    float_parser.set("sum", 0.0f);
    double_parser.set("sum", 0.0);

    // Add 0.1 ten times
    for (int i = 0; i < 10; ++i) {
        float float_sum = float_parser.eval("sum + 0.1");
        double double_sum = double_parser.eval("sum + 0.1");

        float_parser.set("sum", float_sum);
        double_parser.set("sum", double_sum);
    }

    float final_float_sum = float_parser.eval("sum");
    double final_double_sum = double_parser.eval("sum");

    std::cout << "Adding 0.1 ten times:" << std::endl;
    std::cout << "Float result:  " << final_float_sum << std::endl;
    std::cout << "Double result: " << final_double_sum << std::endl;
    std::cout << "Expected:      1.0" << std::endl;

    // Mathematical constants precision
    std::cout << "\nMathematical constants precision:" << std::endl;
    std::cout << "π (M_PI):      " << M_PI << std::endl;
    std::cout << "π as float:    " << static_cast<float>(M_PI) << std::endl;
    std::cout << "e (M_E):       " << M_E << std::endl;
    std::cout << "e as float:    " << static_cast<float>(M_E) << std::endl;
}

/**
 * @brief Demonstrates performance comparison between types
 */
void demonstrate_performance_comparison() {
    std::cout << "--- Performance Comparison ---" << std::endl;

    const int iterations = 100000;

    // Benchmark integer operations
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        int result = calculator::eval<int>("123 + 456");
        (void)result;  // Suppress unused variable warning
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto int_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Benchmark float operations
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        float result = calculator::eval<float>("123.0 + 456.0");
        (void)result;  // Suppress unused variable warning
    }
    end = std::chrono::high_resolution_clock::now();
    auto float_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Benchmark double operations
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        double result = calculator::eval<double>("123.0 + 456.0");
        (void)result;  // Suppress unused variable warning
    }
    end = std::chrono::high_resolution_clock::now();
    auto double_time =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Performance comparison (" << iterations
              << " iterations):" << std::endl;
    std::cout << "Integer operations: " << int_time.count() << " μs"
              << std::endl;
    std::cout << "Float operations:   " << float_time.count() << " μs"
              << std::endl;
    std::cout << "Double operations:  " << double_time.count() << " μs"
              << std::endl;

    std::cout << "\nRelative performance:" << std::endl;
    std::cout << "Float vs Int:    "
              << (double)float_time.count() / int_time.count() << "x"
              << std::endl;
    std::cout << "Double vs Int:   "
              << (double)double_time.count() / int_time.count() << "x"
              << std::endl;
    std::cout << "Double vs Float: "
              << (double)double_time.count() / float_time.count() << "x"
              << std::endl;

    // Memory usage comparison
    std::cout << "\nMemory usage:" << std::endl;
    std::cout << "int:    " << sizeof(int) << " bytes" << std::endl;
    std::cout << "float:  " << sizeof(float) << " bytes" << std::endl;
    std::cout << "double: " << sizeof(double) << " bytes" << std::endl;
}

/**
 * @brief Demonstrates best practices for type selection
 */
void demonstrate_best_practices() {
    std::cout << "--- Best Practices for Type Selection ---" << std::endl;

    std::cout << "Type selection guidelines:" << std::endl;
    std::cout << "\n1. Use int for:" << std::endl;
    std::cout << "   - Counting and indexing" << std::endl;
    std::cout << "   - Bitwise operations" << std::endl;
    std::cout << "   - Exact integer arithmetic" << std::endl;
    std::cout << "   - Performance-critical integer calculations" << std::endl;

    std::cout << "\n2. Use float for:" << std::endl;
    std::cout
        << "   - Graphics and game development (when precision is sufficient)"
        << std::endl;
    std::cout << "   - Memory-constrained applications" << std::endl;
    std::cout << "   - GPU computations (many GPUs prefer float)" << std::endl;

    std::cout << "\n3. Use double for:" << std::endl;
    std::cout << "   - Scientific calculations" << std::endl;
    std::cout << "   - Financial calculations" << std::endl;
    std::cout << "   - General-purpose floating-point arithmetic" << std::endl;
    std::cout << "   - When precision is more important than memory/speed"
              << std::endl;

    // Practical examples
    std::cout << "\nPractical examples:" << std::endl;

    // Counting example (use int)
    calculator::ExpressionParser<int> counter;
    counter.set("items", 42);
    counter.set("new_items", 8);
    int total_items = counter.eval("items + new_items");
    std::cout << "Counting items (int): " << total_items << std::endl;

    // Financial calculation (use double)
    calculator::ExpressionParser<double> finance;
    finance.set("principal", 1000.0);
    finance.set("rate", 0.05);
    finance.set("time", 10.0);
    double compound_interest = finance.eval("principal * (1 + rate) ** time");
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Compound interest (double): $" << compound_interest
              << std::endl;

    // Graphics coordinate (use float)
    calculator::ExpressionParser<float> graphics;
    graphics.set("screen_width", 1920.0f);
    graphics.set("screen_height", 1080.0f);
    float center_x = graphics.eval("screen_width / 2.0");
    float center_y = graphics.eval("screen_height / 2.0");
    std::cout << std::setprecision(1);
    std::cout << "Screen center (float): (" << center_x << ", " << center_y
              << ")" << std::endl;

    // Bitwise flags (use int)
    calculator::ExpressionParser<int> flags;
    flags.set("READ", 1);     // 001
    flags.set("WRITE", 2);    // 010
    flags.set("EXECUTE", 4);  // 100
    int permissions = flags.eval("READ | WRITE | EXECUTE");
    std::cout << "File permissions (int): " << permissions
              << " (binary: " << std::bitset<3>(permissions) << ")"
              << std::endl;

    std::cout << "\nKey takeaways:" << std::endl;
    std::cout << "- Choose the most appropriate type for your use case"
              << std::endl;
    std::cout << "- Consider precision requirements vs. performance needs"
              << std::endl;
    std::cout << "- Be aware of type-specific limitations" << std::endl;
    std::cout << "- Test with representative data to verify behavior"
              << std::endl;
}
