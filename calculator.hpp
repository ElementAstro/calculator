/**
 * @file calculator.hpp
 * @brief A comprehensive calculator implementation under C++17/20
 * @date 2024-09-24
 * @author Max Qian
 * @copyright Copyright (C) 2023-2024 Max Qian <lightapt.com>
 *
 * This file contains the implementation of a full-featured calculator that can
 * evaluate mathematical expressions. It supports:
 * - Basic arithmetic: +, -, *, /, %
 * - Power operations: ^ or **
 * - Comparison operators: >, <, >=, <=, ==, !=
 * - Logical operators: &&, ||, !
 * - Conditional expressions: condition ? true_value : false_value
 * - Variable definition and usage
 * - Built-in math functions: sin, cos, tan, sqrt, abs, log, ln, exp
 * - Math constants: pi, e, phi, tau
 * - User-defined functions: f(x) = x^2 + 2*x + 1
 * - Complex number support
 * - Symbolic differentiation
 * - Bitwise operations for integer types
 * - REPL with history support
 */

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

// Include all calculator modules
#include "include/calculator/error.hpp"
#include "include/calculator/token.hpp"
#include "include/calculator/ast.hpp"
#include "include/calculator/parser.hpp"
#include "include/calculator/complex_parser.hpp"
#include "include/calculator/repl.hpp"

#endif  // CALCULATOR_HPP
