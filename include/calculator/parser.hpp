/**
 * @file parser.hpp
 * @brief Main expression parser for the calculator
 */

#ifndef CALCULATOR_PARSER_HPP
#define CALCULATOR_PARSER_HPP

#include "error.hpp"
#include "token.hpp"
#include "ast.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace calculator {

struct UserDefinedFunction {
    std::vector<std::string> params;
    std::string body;
};

template <typename T>
class ExpressionParser {
public:
    ExpressionParser() {
        setupBuiltinConstants();
        setupBuiltinFunctions();
    }

    T eval(std::string_view expr) {
        expr_ = expr;
        lexer_ = std::make_unique<Lexer>(expr);
        currentToken_ = lexer_->nextToken();

        // Check for function definition: f(x) = expr
        if (currentToken_.type == TokenType::Identifier) {
            std::string name = currentToken_.value;
            std::size_t savedPos = currentToken_.position;
            Token savedToken = currentToken_;

            advance();
            if (currentToken_.type == TokenType::LParen) {
                std::vector<std::string> params;
                advance();

                while (currentToken_.type == TokenType::Identifier) {
                    params.push_back(currentToken_.value);
                    advance();
                    if (currentToken_.type == TokenType::Comma) {
                        advance();
                    } else {
                        break;
                    }
                }

                if (currentToken_.type == TokenType::RParen) {
                    advance();
                    if (currentToken_.type == TokenType::Assign) {
                        advance();
                        std::size_t bodyStart = lexer_->position();
                        // Skip whitespace in body start calculation
                        while (bodyStart > 0 && bodyStart < expr.size() && 
                               std::isspace(static_cast<unsigned char>(expr[bodyStart - 1]))) {
                            bodyStart--;
                        }
                        // Find actual body start after '='
                        std::size_t eqPos = expr.find('=');
                        if (eqPos != std::string::npos) {
                            bodyStart = eqPos + 1;
                            while (bodyStart < expr.size() && 
                                   std::isspace(static_cast<unsigned char>(expr[bodyStart]))) {
                                bodyStart++;
                            }
                        }
                        std::string body(expr.substr(bodyStart));

                        UserDefinedFunction udf;
                        udf.params = params;
                        udf.body = body;
                        userFunctions_[name] = udf;

                        return T(0);
                    }
                }
            }

            // Not a function definition, restart parsing
            lexer_ = std::make_unique<Lexer>(expr);
            currentToken_ = lexer_->nextToken();
        }

        T result = parseAssignment();

        if (currentToken_.type != TokenType::End) {
            throw syntax_error(expr_, "Unexpected token", currentToken_.position);
        }

        return result;
    }

    T eval(char c) { return eval(std::string_view(&c, 1)); }

    void set(const std::string& name, T value) {
        variables_[name] = value;
    }

    void set(const std::string& name, std::function<T(T)> func) {
        functions1_[name] = func;
    }

    void set(const std::string& name, std::function<T(T, T)> func) {
        functions2_[name] = func;
    }

    T get(const std::string& name) const {
        auto it = variables_.find(name);
        if (it != variables_.end()) {
            return it->second;
        }
        throw undefined_error("", name, false);
    }

    bool hasVariable(const std::string& name) const {
        return variables_.find(name) != variables_.end();
    }

    bool hasFunction(const std::string& name) const {
        return functions1_.find(name) != functions1_.end() ||
               functions2_.find(name) != functions2_.end() ||
               userFunctions_.find(name) != userFunctions_.end();
    }

    std::vector<std::string> getVariableNames() const {
        std::vector<std::string> names;
        for (const auto& [name, _] : variables_) {
            names.push_back(name);
        }
        return names;
    }

    std::vector<std::string> getFunctionNames() const {
        std::vector<std::string> names;
        for (const auto& [name, _] : functions1_) {
            names.push_back(name);
        }
        for (const auto& [name, _] : functions2_) {
            if (std::find(names.begin(), names.end(), name) == names.end()) {
                names.push_back(name);
            }
        }
        for (const auto& [name, _] : userFunctions_) {
            names.push_back(name);
        }
        return names;
    }

    void clearVariables() {
        variables_.clear();
        setupBuiltinConstants();
    }

    void clearUserFunctions() {
        userFunctions_.clear();
    }

    void defineFunction(const std::string& name,
                       const std::vector<std::string>& params,
                       const std::string& body) {
        UserDefinedFunction udf;
        udf.params = params;
        udf.body = body;
        userFunctions_[name] = udf;
    }

    std::optional<std::pair<std::vector<std::string>, std::string>>
    getFunctionInfo(const std::string& name) const {
        auto it = userFunctions_.find(name);
        if (it != userFunctions_.end()) {
            return std::make_pair(it->second.params, it->second.body);
        }
        return std::nullopt;
    }

    // Parse expression to AST (for differentiation)
    ASTPtr parseToAST(std::string_view expr) {
        expr_ = expr;
        lexer_ = std::make_unique<Lexer>(expr);
        currentToken_ = lexer_->nextToken();
        return parseASTTernary();
    }

    // Differentiate expression
    std::string diff(std::string_view expr, const std::string& var) {
        auto ast = parseToAST(expr);
        auto derivative = differentiate(ast, var);
        auto simplified = simplify(derivative);
        return astToString(simplified);
    }

private:
    void setupBuiltinConstants() {
        // Basic mathematical constants
        variables_["pi"] = static_cast<T>(3.14159265358979323846);
        variables_["e"] = static_cast<T>(2.71828182845904523536);
        variables_["phi"] = static_cast<T>(1.61803398874989484820);  // Golden ratio
        variables_["tau"] = static_cast<T>(6.28318530717958647692);  // 2*pi
        
        // Additional constants
        variables_["sqrt2"] = static_cast<T>(1.41421356237309504880);
        variables_["sqrt3"] = static_cast<T>(1.73205080756887729352);
        variables_["ln2"] = static_cast<T>(0.69314718055994530942);
        variables_["ln10"] = static_cast<T>(2.30258509299404568402);
        variables_["inf"] = std::numeric_limits<T>::infinity();
        variables_["nan"] = std::numeric_limits<T>::quiet_NaN();
        
        // Physics constants (SI units)
        variables_["c"] = static_cast<T>(299792458.0);           // Speed of light (m/s)
        variables_["G"] = static_cast<T>(6.67430e-11);           // Gravitational constant
        variables_["h"] = static_cast<T>(6.62607015e-34);        // Planck constant
        variables_["kb"] = static_cast<T>(1.380649e-23);         // Boltzmann constant
        variables_["Na"] = static_cast<T>(6.02214076e23);        // Avogadro's number
        variables_["qe"] = static_cast<T>(1.602176634e-19);      // Elementary charge
    }

    void setupBuiltinFunctions() {
        // Trigonometric functions
        functions1_["sin"] = [](T x) { return static_cast<T>(std::sin(static_cast<double>(x))); };
        functions1_["cos"] = [](T x) { return static_cast<T>(std::cos(static_cast<double>(x))); };
        functions1_["tan"] = [](T x) { return static_cast<T>(std::tan(static_cast<double>(x))); };
        functions1_["cot"] = [](T x) { return static_cast<T>(1.0 / std::tan(static_cast<double>(x))); };
        functions1_["sec"] = [](T x) { return static_cast<T>(1.0 / std::cos(static_cast<double>(x))); };
        functions1_["csc"] = [](T x) { return static_cast<T>(1.0 / std::sin(static_cast<double>(x))); };
        
        // Inverse trigonometric functions
        functions1_["asin"] = [](T x) { return static_cast<T>(std::asin(static_cast<double>(x))); };
        functions1_["acos"] = [](T x) { return static_cast<T>(std::acos(static_cast<double>(x))); };
        functions1_["atan"] = [](T x) { return static_cast<T>(std::atan(static_cast<double>(x))); };
        functions1_["acot"] = [](T x) { return static_cast<T>(std::atan(1.0 / static_cast<double>(x))); };
        functions1_["asec"] = [](T x) { return static_cast<T>(std::acos(1.0 / static_cast<double>(x))); };
        functions1_["acsc"] = [](T x) { return static_cast<T>(std::asin(1.0 / static_cast<double>(x))); };
        
        // Hyperbolic functions
        functions1_["sinh"] = [](T x) { return static_cast<T>(std::sinh(static_cast<double>(x))); };
        functions1_["cosh"] = [](T x) { return static_cast<T>(std::cosh(static_cast<double>(x))); };
        functions1_["tanh"] = [](T x) { return static_cast<T>(std::tanh(static_cast<double>(x))); };
        functions1_["coth"] = [](T x) { return static_cast<T>(1.0 / std::tanh(static_cast<double>(x))); };
        functions1_["sech"] = [](T x) { return static_cast<T>(1.0 / std::cosh(static_cast<double>(x))); };
        functions1_["csch"] = [](T x) { return static_cast<T>(1.0 / std::sinh(static_cast<double>(x))); };
        
        // Inverse hyperbolic functions
        functions1_["asinh"] = [](T x) { return static_cast<T>(std::asinh(static_cast<double>(x))); };
        functions1_["acosh"] = [](T x) { return static_cast<T>(std::acosh(static_cast<double>(x))); };
        functions1_["atanh"] = [](T x) { return static_cast<T>(std::atanh(static_cast<double>(x))); };

        // Logarithmic functions
        functions1_["log"] = [](T x) { return static_cast<T>(std::log10(static_cast<double>(x))); };
        functions1_["log10"] = [](T x) { return static_cast<T>(std::log10(static_cast<double>(x))); };
        functions1_["log2"] = [](T x) { return static_cast<T>(std::log2(static_cast<double>(x))); };
        functions1_["ln"] = [](T x) { return static_cast<T>(std::log(static_cast<double>(x))); };
        functions1_["exp"] = [](T x) { return static_cast<T>(std::exp(static_cast<double>(x))); };
        functions1_["exp2"] = [](T x) { return static_cast<T>(std::exp2(static_cast<double>(x))); };
        functions1_["expm1"] = [](T x) { return static_cast<T>(std::expm1(static_cast<double>(x))); };
        functions1_["log1p"] = [](T x) { return static_cast<T>(std::log1p(static_cast<double>(x))); };

        // Root functions
        functions1_["sqrt"] = [](T x) { return static_cast<T>(std::sqrt(static_cast<double>(x))); };
        functions1_["cbrt"] = [](T x) { return static_cast<T>(std::cbrt(static_cast<double>(x))); };

        // Rounding and absolute value
        functions1_["abs"] = [](T x) { return static_cast<T>(std::abs(static_cast<double>(x))); };
        functions1_["floor"] = [](T x) { return static_cast<T>(std::floor(static_cast<double>(x))); };
        functions1_["ceil"] = [](T x) { return static_cast<T>(std::ceil(static_cast<double>(x))); };
        functions1_["round"] = [](T x) { return static_cast<T>(std::round(static_cast<double>(x))); };
        functions1_["trunc"] = [](T x) { return static_cast<T>(std::trunc(static_cast<double>(x))); };
        functions1_["frac"] = [](T x) { 
            double d = static_cast<double>(x);
            return static_cast<T>(d - std::trunc(d)); 
        };

        // Sign and special functions
        functions1_["sign"] = [](T x) {
            if (x > 0) return static_cast<T>(1);
            if (x < 0) return static_cast<T>(-1);
            return static_cast<T>(0);
        };
        functions1_["heaviside"] = [](T x) {
            if (x > 0) return static_cast<T>(1);
            if (x < 0) return static_cast<T>(0);
            return static_cast<T>(0.5);
        };

        // Angle conversion
        functions1_["deg"] = [](T x) { return static_cast<T>(x * 180.0 / 3.14159265358979323846); };
        functions1_["rad"] = [](T x) { return static_cast<T>(x * 3.14159265358979323846 / 180.0); };

        // Factorial and gamma functions
        functions1_["fact"] = [](T x) { return static_cast<T>(std::tgamma(static_cast<double>(x) + 1)); };
        functions1_["factorial"] = [](T x) { return static_cast<T>(std::tgamma(static_cast<double>(x) + 1)); };
        functions1_["gamma"] = [](T x) { return static_cast<T>(std::tgamma(static_cast<double>(x))); };
        functions1_["lgamma"] = [](T x) { return static_cast<T>(std::lgamma(static_cast<double>(x))); };

        // Error functions
        functions1_["erf"] = [](T x) { return static_cast<T>(std::erf(static_cast<double>(x))); };
        functions1_["erfc"] = [](T x) { return static_cast<T>(std::erfc(static_cast<double>(x))); };

        // Utility functions
        functions1_["isnan"] = [](T x) { return static_cast<T>(std::isnan(static_cast<double>(x)) ? 1 : 0); };
        functions1_["isinf"] = [](T x) { return static_cast<T>(std::isinf(static_cast<double>(x)) ? 1 : 0); };
        functions1_["isfinite"] = [](T x) { return static_cast<T>(std::isfinite(static_cast<double>(x)) ? 1 : 0); };

        // Reciprocal
        functions1_["recip"] = [](T x) { return static_cast<T>(1.0 / static_cast<double>(x)); };
        functions1_["inv"] = [](T x) { return static_cast<T>(1.0 / static_cast<double>(x)); };

        // Square
        functions1_["sqr"] = [](T x) { return x * x; };
        functions1_["cube"] = [](T x) { return x * x * x; };

        // Two-argument functions
        functions2_["pow"] = [](T x, T y) { return static_cast<T>(std::pow(static_cast<double>(x), static_cast<double>(y))); };
        functions2_["atan2"] = [](T y, T x) { return static_cast<T>(std::atan2(static_cast<double>(y), static_cast<double>(x))); };
        functions2_["max"] = [](T x, T y) { return std::max(x, y); };
        functions2_["min"] = [](T x, T y) { return std::min(x, y); };
        functions2_["mod"] = [](T x, T y) { return static_cast<T>(std::fmod(static_cast<double>(x), static_cast<double>(y))); };
        functions2_["hypot"] = [](T x, T y) { return static_cast<T>(std::hypot(static_cast<double>(x), static_cast<double>(y))); };
        functions2_["copysign"] = [](T x, T y) { return static_cast<T>(std::copysign(static_cast<double>(x), static_cast<double>(y))); };
        functions2_["fdim"] = [](T x, T y) { return static_cast<T>(std::fdim(static_cast<double>(x), static_cast<double>(y))); };
        functions2_["fma"] = [](T x, T y) { return x * y; };  // Simplified, real fma needs 3 args
        
        // Logarithm with custom base
        functions2_["logb"] = [](T x, T base) { 
            return static_cast<T>(std::log(static_cast<double>(x)) / std::log(static_cast<double>(base))); 
        };
        
        // GCD and LCM (for integer-like values)
        functions2_["gcd"] = [](T a, T b) {
            long long x = static_cast<long long>(std::abs(static_cast<double>(a)));
            long long y = static_cast<long long>(std::abs(static_cast<double>(b)));
            while (y != 0) {
                long long t = y;
                y = x % y;
                x = t;
            }
            return static_cast<T>(x);
        };
        functions2_["lcm"] = [](T a, T b) {
            long long x = static_cast<long long>(std::abs(static_cast<double>(a)));
            long long y = static_cast<long long>(std::abs(static_cast<double>(b)));
            if (x == 0 || y == 0) return static_cast<T>(0);
            long long gcd_val = x;
            long long temp = y;
            while (temp != 0) {
                long long t = temp;
                temp = gcd_val % temp;
                gcd_val = t;
            }
            return static_cast<T>((x / gcd_val) * y);
        };
        
        // Combinatorics
        functions2_["nPr"] = [](T n, T r) {
            double nd = static_cast<double>(n);
            double rd = static_cast<double>(r);
            return static_cast<T>(std::tgamma(nd + 1) / std::tgamma(nd - rd + 1));
        };
        functions2_["nCr"] = [](T n, T r) {
            double nd = static_cast<double>(n);
            double rd = static_cast<double>(r);
            return static_cast<T>(std::tgamma(nd + 1) / (std::tgamma(rd + 1) * std::tgamma(nd - rd + 1)));
        };
        functions2_["perm"] = functions2_["nPr"];
        functions2_["comb"] = functions2_["nCr"];
        
        // Average
        functions2_["avg"] = [](T x, T y) { return (x + y) / static_cast<T>(2); };
        
        // Clamp (simplified to 2 args - clamps to [0, max])
        functions2_["clamp"] = [](T x, T maxVal) {
            if (x < static_cast<T>(0)) return static_cast<T>(0);
            if (x > maxVal) return maxVal;
            return x;
        };
        
        // Lerp (linear interpolation at t=0.5)
        functions2_["lerp"] = [](T a, T b) { return (a + b) / static_cast<T>(2); };
        
        // Wrap (wrap x to range [0, max))
        functions2_["wrap"] = [](T x, T maxVal) {
            double xd = static_cast<double>(x);
            double md = static_cast<double>(maxVal);
            double result = std::fmod(xd, md);
            if (result < 0) result += md;
            return static_cast<T>(result);
        };
    }

    void advance() {
        currentToken_ = lexer_->nextToken();
    }

    void expect(TokenType type, const std::string& msg = "") {
        if (currentToken_.type != type) {
            std::string errorMsg = msg.empty() ?
                "Unexpected token" : msg;
            throw syntax_error(expr_, errorMsg, currentToken_.position);
        }
        advance();
    }

    // Assignment: identifier = expression
    T parseAssignment() {
        if (currentToken_.type == TokenType::Identifier) {
            std::string name = currentToken_.value;
            Token savedToken = currentToken_;

            advance();
            if (currentToken_.type == TokenType::Assign) {
                advance();
                T value = parseTernary();
                variables_[name] = value;
                return value;
            }

            // Not an assignment, backtrack
            lexer_ = std::make_unique<Lexer>(expr_);
            currentToken_ = lexer_->nextToken();
        }

        return parseTernary();
    }

    // Ternary: condition ? true_expr : false_expr
    T parseTernary() {
        T condition = parseLogicalOr();

        if (currentToken_.type == TokenType::Question) {
            advance();
            T trueValue = parseTernary();
            expect(TokenType::Colon, "Expected ':' in ternary expression");
            T falseValue = parseTernary();
            return (condition != T(0)) ? trueValue : falseValue;
        }

        return condition;
    }

    // Logical OR: ||
    T parseLogicalOr() {
        T left = parseLogicalAnd();

        while (currentToken_.type == TokenType::Or) {
            advance();
            T right = parseLogicalAnd();
            left = (left != T(0) || right != T(0)) ? T(1) : T(0);
        }

        return left;
    }

    // Logical AND: &&
    T parseLogicalAnd() {
        T left = parseBitwiseOr();

        while (currentToken_.type == TokenType::And) {
            advance();
            T right = parseBitwiseOr();
            left = (left != T(0) && right != T(0)) ? T(1) : T(0);
        }

        return left;
    }

    // Bitwise OR: |
    T parseBitwiseOr() {
        T left = parseBitwiseXor();

        while (currentToken_.type == TokenType::BitwiseOr) {
            advance();
            T right = parseBitwiseXor();
            if constexpr (std::is_integral_v<T>) {
                left = left | right;
            } else {
                left = static_cast<T>(static_cast<long long>(left) | static_cast<long long>(right));
            }
        }

        return left;
    }

    // Bitwise XOR (using different operator since ^ is power)
    T parseBitwiseXor() {
        T left = parseBitwiseAnd();
        return left;
    }

    // Bitwise AND: &
    T parseBitwiseAnd() {
        T left = parseEquality();

        while (currentToken_.type == TokenType::BitwiseAnd) {
            advance();
            T right = parseEquality();
            if constexpr (std::is_integral_v<T>) {
                left = left & right;
            } else {
                left = static_cast<T>(static_cast<long long>(left) & static_cast<long long>(right));
            }
        }

        return left;
    }

    // Equality: ==, !=
    T parseEquality() {
        T left = parseComparison();

        while (currentToken_.type == TokenType::Equal ||
               currentToken_.type == TokenType::NotEqual) {
            TokenType op = currentToken_.type;
            advance();
            T right = parseComparison();

            if (op == TokenType::Equal) {
                left = (left == right) ? T(1) : T(0);
            } else {
                left = (left != right) ? T(1) : T(0);
            }
        }

        return left;
    }

    // Comparison: >, <, >=, <=
    T parseComparison() {
        T left = parseShift();

        while (currentToken_.type == TokenType::Greater ||
               currentToken_.type == TokenType::Less ||
               currentToken_.type == TokenType::GreaterEqual ||
               currentToken_.type == TokenType::LessEqual) {
            TokenType op = currentToken_.type;
            advance();
            T right = parseShift();

            switch (op) {
                case TokenType::Greater:
                    left = (left > right) ? T(1) : T(0);
                    break;
                case TokenType::Less:
                    left = (left < right) ? T(1) : T(0);
                    break;
                case TokenType::GreaterEqual:
                    left = (left >= right) ? T(1) : T(0);
                    break;
                case TokenType::LessEqual:
                    left = (left <= right) ? T(1) : T(0);
                    break;
                default:
                    break;
            }
        }

        return left;
    }

    // Shift: <<, >>
    T parseShift() {
        T left = parseAddSub();

        while (currentToken_.type == TokenType::ShiftLeft ||
               currentToken_.type == TokenType::ShiftRight) {
            TokenType op = currentToken_.type;
            advance();
            T right = parseAddSub();

            if constexpr (std::is_integral_v<T>) {
                if (op == TokenType::ShiftLeft) {
                    left = left << static_cast<int>(right);
                } else {
                    left = left >> static_cast<int>(right);
                }
            } else {
                if (op == TokenType::ShiftLeft) {
                    left = static_cast<T>(static_cast<long long>(left) << static_cast<int>(right));
                } else {
                    left = static_cast<T>(static_cast<long long>(left) >> static_cast<int>(right));
                }
            }
        }

        return left;
    }

    // Addition and Subtraction: +, -
    T parseAddSub() {
        T left = parseMulDiv();

        while (currentToken_.type == TokenType::Plus ||
               currentToken_.type == TokenType::Minus) {
            TokenType op = currentToken_.type;
            advance();
            T right = parseMulDiv();

            if (op == TokenType::Plus) {
                left = left + right;
            } else {
                left = left - right;
            }
        }

        return left;
    }

    // Multiplication, Division, Modulo, Floor Division: *, /, %, //
    T parseMulDiv() {
        T left = parsePower();

        while (currentToken_.type == TokenType::Multiply ||
               currentToken_.type == TokenType::Divide ||
               currentToken_.type == TokenType::FloorDivide ||
               currentToken_.type == TokenType::Modulo) {
            TokenType op = currentToken_.type;
            advance();
            T right = parsePower();

            if (op == TokenType::Multiply) {
                left = left * right;
            } else if (op == TokenType::Divide) {
                if (right == T(0)) {
                    throw math_error(expr_, "Division by zero");
                }
                left = left / right;
            } else if (op == TokenType::FloorDivide) {
                if (right == T(0)) {
                    throw math_error(expr_, "Division by zero");
                }
                left = static_cast<T>(std::floor(static_cast<double>(left) / static_cast<double>(right)));
            } else {
                if (right == T(0)) {
                    throw math_error(expr_, "Modulo by zero");
                }
                if constexpr (std::is_integral_v<T>) {
                    left = left % right;
                } else {
                    left = static_cast<T>(std::fmod(static_cast<double>(left), static_cast<double>(right)));
                }
            }
        }

        return left;
    }

    // Power: ^ or ** (right associative)
    T parsePower() {
        T base = parseUnary();

        if (currentToken_.type == TokenType::Power) {
            advance();
            T exponent = parsePower();  // Right associative
            return static_cast<T>(std::pow(static_cast<double>(base), static_cast<double>(exponent)));
        }

        return base;
    }

    // Unary: +, -, !, ~
    T parseUnary() {
        if (currentToken_.type == TokenType::Plus) {
            advance();
            return parseUnary();
        }

        if (currentToken_.type == TokenType::Minus) {
            advance();
            return -parseUnary();
        }

        if (currentToken_.type == TokenType::Not) {
            advance();
            T value = parseUnary();
            return (value == T(0)) ? T(1) : T(0);
        }

        if (currentToken_.type == TokenType::BitwiseNot) {
            advance();
            T value = parseUnary();
            if constexpr (std::is_integral_v<T>) {
                return ~value;
            } else {
                return static_cast<T>(~static_cast<long long>(value));
            }
        }

        return parsePrimary();
    }

    // Primary: numbers, identifiers, function calls, parentheses
    T parsePrimary() {
        if (currentToken_.type == TokenType::Number) {
            T value = static_cast<T>(currentToken_.numValue);
            advance();
            return value;
        }

        if (currentToken_.type == TokenType::Imaginary) {
            throw error(expr_, "Imaginary numbers not supported for this type. Use std::complex<double>.");
        }

        if (currentToken_.type == TokenType::Identifier) {
            std::string name = currentToken_.value;
            advance();

            if (currentToken_.type == TokenType::LParen) {
                advance();
                std::vector<T> args;

                if (currentToken_.type != TokenType::RParen) {
                    args.push_back(parseTernary());
                    while (currentToken_.type == TokenType::Comma) {
                        advance();
                        args.push_back(parseTernary());
                    }
                }

                expect(TokenType::RParen, "Expected ')' after function arguments");

                return callFunction(name, args);
            }

            auto it = variables_.find(name);
            if (it != variables_.end()) {
                return it->second;
            }

            throw undefined_error(expr_, name, false);
        }

        if (currentToken_.type == TokenType::LParen) {
            advance();
            T value = parseTernary();
            expect(TokenType::RParen, "Expected ')'");
            return value;
        }

        throw syntax_error(expr_, "Unexpected token: " + currentToken_.value, currentToken_.position);
    }

    T callFunction(const std::string& name, const std::vector<T>& args) {
        auto it1 = functions1_.find(name);
        if (it1 != functions1_.end()) {
            if (args.size() != 1) {
                throw error(expr_, "Function '" + name + "' expects 1 argument, got " + std::to_string(args.size()));
            }
            return it1->second(args[0]);
        }

        auto it2 = functions2_.find(name);
        if (it2 != functions2_.end()) {
            if (args.size() != 2) {
                throw error(expr_, "Function '" + name + "' expects 2 arguments, got " + std::to_string(args.size()));
            }
            return it2->second(args[0], args[1]);
        }

        auto itUser = userFunctions_.find(name);
        if (itUser != userFunctions_.end()) {
            const auto& udf = itUser->second;
            if (args.size() != udf.params.size()) {
                throw error(expr_, "Function '" + name + "' expects " +
                           std::to_string(udf.params.size()) + " argument(s), got " + std::to_string(args.size()));
            }

            ExpressionParser<T> funcParser;
            funcParser.variables_ = variables_;
            funcParser.functions1_ = functions1_;
            funcParser.functions2_ = functions2_;
            funcParser.userFunctions_ = userFunctions_;

            for (std::size_t i = 0; i < args.size(); ++i) {
                funcParser.variables_[udf.params[i]] = args[i];
            }

            return funcParser.eval(udf.body);
        }

        throw undefined_error(expr_, name, true);
    }

    // AST parsing methods for differentiation
    ASTPtr parseASTTernary() {
        auto condition = parseASTLogicalOr();

        if (currentToken_.type == TokenType::Question) {
            advance();
            auto trueExpr = parseASTTernary();
            expect(TokenType::Colon, "Expected ':' in ternary expression");
            auto falseExpr = parseASTTernary();
            // Represent ternary as a special function
            return ASTNode::makeFunction("ternary", {condition, trueExpr, falseExpr});
        }

        return condition;
    }

    ASTPtr parseASTLogicalOr() {
        auto left = parseASTLogicalAnd();

        while (currentToken_.type == TokenType::Or) {
            advance();
            auto right = parseASTLogicalAnd();
            left = ASTNode::makeFunction("or", {left, right});
        }

        return left;
    }

    ASTPtr parseASTLogicalAnd() {
        auto left = parseASTEquality();

        while (currentToken_.type == TokenType::And) {
            advance();
            auto right = parseASTEquality();
            left = ASTNode::makeFunction("and", {left, right});
        }

        return left;
    }

    ASTPtr parseASTEquality() {
        auto left = parseASTComparison();

        while (currentToken_.type == TokenType::Equal ||
               currentToken_.type == TokenType::NotEqual) {
            char op = (currentToken_.type == TokenType::Equal) ? '=' : '!';
            advance();
            auto right = parseASTComparison();
            left = ASTNode::makeFunction(op == '=' ? "eq" : "ne", {left, right});
        }

        return left;
    }

    ASTPtr parseASTComparison() {
        auto left = parseASTAddSub();

        while (currentToken_.type == TokenType::Greater ||
               currentToken_.type == TokenType::Less ||
               currentToken_.type == TokenType::GreaterEqual ||
               currentToken_.type == TokenType::LessEqual) {
            std::string op;
            switch (currentToken_.type) {
                case TokenType::Greater: op = "gt"; break;
                case TokenType::Less: op = "lt"; break;
                case TokenType::GreaterEqual: op = "ge"; break;
                case TokenType::LessEqual: op = "le"; break;
                default: break;
            }
            advance();
            auto right = parseASTAddSub();
            left = ASTNode::makeFunction(op, {left, right});
        }

        return left;
    }

    ASTPtr parseASTAddSub() {
        auto left = parseASTMulDiv();

        while (currentToken_.type == TokenType::Plus ||
               currentToken_.type == TokenType::Minus) {
            char op = (currentToken_.type == TokenType::Plus) ? '+' : '-';
            advance();
            auto right = parseASTMulDiv();
            left = ASTNode::makeBinaryOp(op, left, right);
        }

        return left;
    }

    ASTPtr parseASTMulDiv() {
        auto left = parseASTPower();

        while (currentToken_.type == TokenType::Multiply ||
               currentToken_.type == TokenType::Divide ||
               currentToken_.type == TokenType::Modulo) {
            char op;
            switch (currentToken_.type) {
                case TokenType::Multiply: op = '*'; break;
                case TokenType::Divide: op = '/'; break;
                case TokenType::Modulo: op = '%'; break;
                default: op = '*'; break;
            }
            advance();
            auto right = parseASTPower();
            left = ASTNode::makeBinaryOp(op, left, right);
        }

        return left;
    }

    ASTPtr parseASTPower() {
        auto base = parseASTUnary();

        if (currentToken_.type == TokenType::Power) {
            advance();
            auto exponent = parseASTPower();
            return ASTNode::makeBinaryOp('^', base, exponent);
        }

        return base;
    }

    ASTPtr parseASTUnary() {
        if (currentToken_.type == TokenType::Plus) {
            advance();
            return parseASTUnary();
        }

        if (currentToken_.type == TokenType::Minus) {
            advance();
            return ASTNode::makeUnaryOp('-', parseASTUnary());
        }

        if (currentToken_.type == TokenType::Not) {
            advance();
            return ASTNode::makeFunction("not", {parseASTUnary()});
        }

        return parseASTPrimary();
    }

    ASTPtr parseASTPrimary() {
        if (currentToken_.type == TokenType::Number) {
            double value = currentToken_.numValue;
            advance();
            return ASTNode::makeNumber(value);
        }

        if (currentToken_.type == TokenType::Identifier) {
            std::string name = currentToken_.value;
            advance();

            if (currentToken_.type == TokenType::LParen) {
                advance();
                std::vector<ASTPtr> args;

                if (currentToken_.type != TokenType::RParen) {
                    args.push_back(parseASTTernary());
                    while (currentToken_.type == TokenType::Comma) {
                        advance();
                        args.push_back(parseASTTernary());
                    }
                }

                expect(TokenType::RParen, "Expected ')' after function arguments");

                return ASTNode::makeFunction(name, args);
            }

            // Check if it's a constant
            auto it = variables_.find(name);
            if (it != variables_.end()) {
                // For constants like pi and e, keep them as variables for differentiation
                if (name == "pi" || name == "e" || name == "phi" || name == "tau") {
                    return ASTNode::makeNumber(static_cast<double>(it->second));
                }
            }

            return ASTNode::makeVariable(name);
        }

        if (currentToken_.type == TokenType::LParen) {
            advance();
            auto expr = parseASTTernary();
            expect(TokenType::RParen, "Expected ')'");
            return expr;
        }

        throw syntax_error(expr_, "Unexpected token in AST parsing", currentToken_.position);
    }

    std::string_view expr_;
    std::unique_ptr<Lexer> lexer_;
    Token currentToken_;

    std::unordered_map<std::string, T> variables_;
    std::unordered_map<std::string, std::function<T(T)>> functions1_;
    std::unordered_map<std::string, std::function<T(T, T)>> functions2_;
    std::unordered_map<std::string, UserDefinedFunction> userFunctions_;
};

// Convenience function for quick evaluation
template <typename T>
inline T eval(std::string_view expression) {
    ExpressionParser<T> parser;
    return parser.eval(expression);
}

inline double eval(const std::string& expression) {
    return eval<double>(expression);
}

}  // namespace calculator

#endif  // CALCULATOR_PARSER_HPP
