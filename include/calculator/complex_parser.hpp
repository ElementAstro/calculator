/**
 * @file complex_parser.hpp
 * @brief Complex number support for the calculator
 */

#ifndef CALCULATOR_COMPLEX_PARSER_HPP
#define CALCULATOR_COMPLEX_PARSER_HPP

#include "error.hpp"
#include "token.hpp"
#include "parser.hpp"

#include <complex>
#include <cmath>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace calculator {

struct ComplexUserDefinedFunction {
    std::vector<std::string> params;
    std::string body;
};

// Full specialization for complex<double>
template <>
class ExpressionParser<std::complex<double>> {
public:
    using T = std::complex<double>;

    ExpressionParser() {
        setupBuiltinConstants();
        setupBuiltinFunctions();
    }

    T eval(std::string_view expr) {
        expr_ = expr;
        lexer_ = std::make_unique<Lexer>(expr);
        currentToken_ = lexer_->nextToken();

        // Check for function definition
        if (currentToken_.type == TokenType::Identifier) {
            std::string name = currentToken_.value;
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
                        std::size_t eqPos = expr.find('=');
                        std::size_t bodyStart = eqPos + 1;
                        while (bodyStart < expr.size() && 
                               std::isspace(static_cast<unsigned char>(expr[bodyStart]))) {
                            bodyStart++;
                        }
                        std::string body(expr.substr(bodyStart));

                        ComplexUserDefinedFunction udf;
                        udf.params = params;
                        udf.body = body;
                        userFunctions_[name] = udf;

                        return T(0);
                    }
                }
            }

            lexer_ = std::make_unique<Lexer>(expr);
            currentToken_ = lexer_->nextToken();
        }

        T result = parseAssignment();

        if (currentToken_.type != TokenType::End) {
            throw syntax_error(expr_, "Unexpected token", currentToken_.position);
        }

        return result;
    }

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

    void defineFunction(const std::string& name,
                       const std::vector<std::string>& params,
                       const std::string& body) {
        ComplexUserDefinedFunction udf;
        udf.params = params;
        udf.body = body;
        userFunctions_[name] = udf;
    }

private:
    void setupBuiltinConstants() {
        variables_["pi"] = T(3.14159265358979323846, 0);
        variables_["e"] = T(2.71828182845904523536, 0);
        variables_["i"] = T(0, 1);
        variables_["phi"] = T(1.61803398874989484820, 0);
        variables_["tau"] = T(6.28318530717958647692, 0);
    }

    void setupBuiltinFunctions() {
        functions1_["sin"] = [](T x) { return std::sin(x); };
        functions1_["cos"] = [](T x) { return std::cos(x); };
        functions1_["tan"] = [](T x) { return std::tan(x); };
        functions1_["asin"] = [](T x) { return std::asin(x); };
        functions1_["acos"] = [](T x) { return std::acos(x); };
        functions1_["atan"] = [](T x) { return std::atan(x); };
        functions1_["sinh"] = [](T x) { return std::sinh(x); };
        functions1_["cosh"] = [](T x) { return std::cosh(x); };
        functions1_["tanh"] = [](T x) { return std::tanh(x); };

        functions1_["exp"] = [](T x) { return std::exp(x); };
        functions1_["log"] = [](T x) { return std::log(x); };
        functions1_["ln"] = [](T x) { return std::log(x); };
        functions1_["log10"] = [](T x) { return std::log10(x); };

        functions1_["sqrt"] = [](T x) { return std::sqrt(x); };

        functions1_["abs"] = [](T x) { return T(std::abs(x), 0); };
        functions1_["arg"] = [](T x) { return T(std::arg(x), 0); };
        functions1_["conj"] = [](T x) { return std::conj(x); };
        functions1_["real"] = [](T x) { return T(x.real(), 0); };
        functions1_["imag"] = [](T x) { return T(x.imag(), 0); };
        functions1_["norm"] = [](T x) { return T(std::norm(x), 0); };

        functions2_["pow"] = [](T x, T y) { return std::pow(x, y); };
    }

    void advance() {
        currentToken_ = lexer_->nextToken();
    }

    void expect(TokenType type, const std::string& msg = "") {
        if (currentToken_.type != type) {
            std::string errorMsg = msg.empty() ? "Unexpected token" : msg;
            throw syntax_error(expr_, errorMsg, currentToken_.position);
        }
        advance();
    }

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

            lexer_ = std::make_unique<Lexer>(expr_);
            currentToken_ = lexer_->nextToken();
        }

        return parseTernary();
    }

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

    T parseLogicalOr() {
        T left = parseLogicalAnd();

        while (currentToken_.type == TokenType::Or) {
            advance();
            T right = parseLogicalAnd();
            left = (left != T(0) || right != T(0)) ? T(1) : T(0);
        }

        return left;
    }

    T parseLogicalAnd() {
        T left = parseEquality();

        while (currentToken_.type == TokenType::And) {
            advance();
            T right = parseEquality();
            left = (left != T(0) && right != T(0)) ? T(1) : T(0);
        }

        return left;
    }

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

    T parseComparison() {
        T left = parseAddSub();

        while (currentToken_.type == TokenType::Greater ||
               currentToken_.type == TokenType::Less ||
               currentToken_.type == TokenType::GreaterEqual ||
               currentToken_.type == TokenType::LessEqual) {
            TokenType op = currentToken_.type;
            advance();
            T right = parseAddSub();

            double leftMag = std::abs(left);
            double rightMag = std::abs(right);

            switch (op) {
                case TokenType::Greater:
                    left = (leftMag > rightMag) ? T(1) : T(0);
                    break;
                case TokenType::Less:
                    left = (leftMag < rightMag) ? T(1) : T(0);
                    break;
                case TokenType::GreaterEqual:
                    left = (leftMag >= rightMag) ? T(1) : T(0);
                    break;
                case TokenType::LessEqual:
                    left = (leftMag <= rightMag) ? T(1) : T(0);
                    break;
                default:
                    break;
            }
        }

        return left;
    }

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

    T parseMulDiv() {
        T left = parsePower();

        while (currentToken_.type == TokenType::Multiply ||
               currentToken_.type == TokenType::Divide) {
            TokenType op = currentToken_.type;
            advance();
            T right = parsePower();

            if (op == TokenType::Multiply) {
                left = left * right;
            } else {
                if (right == T(0)) {
                    throw math_error(expr_, "Division by zero");
                }
                left = left / right;
            }
        }

        return left;
    }

    T parsePower() {
        T base = parseUnary();

        if (currentToken_.type == TokenType::Power) {
            advance();
            T exponent = parsePower();
            return std::pow(base, exponent);
        }

        return base;
    }

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

        return parsePrimary();
    }

    T parsePrimary() {
        if (currentToken_.type == TokenType::Number) {
            T value(currentToken_.numValue, 0);
            advance();
            return value;
        }

        if (currentToken_.type == TokenType::Imaginary) {
            T value(0, currentToken_.numValue);
            advance();
            return value;
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
                throw error(expr_, "Function '" + name + "' expects 1 argument");
            }
            return it1->second(args[0]);
        }

        auto it2 = functions2_.find(name);
        if (it2 != functions2_.end()) {
            if (args.size() != 2) {
                throw error(expr_, "Function '" + name + "' expects 2 arguments");
            }
            return it2->second(args[0], args[1]);
        }

        auto itUser = userFunctions_.find(name);
        if (itUser != userFunctions_.end()) {
            const auto& udf = itUser->second;
            if (args.size() != udf.params.size()) {
                throw error(expr_, "Function '" + name + "' expects " +
                           std::to_string(udf.params.size()) + " argument(s)");
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

    std::string_view expr_;
    std::unique_ptr<Lexer> lexer_;
    Token currentToken_;

    std::unordered_map<std::string, T> variables_;
    std::unordered_map<std::string, std::function<T(T)>> functions1_;
    std::unordered_map<std::string, std::function<T(T, T)>> functions2_;
    std::unordered_map<std::string, ComplexUserDefinedFunction> userFunctions_;
};

// Helper function to format complex numbers
inline std::string formatComplex(const std::complex<double>& c) {
    std::ostringstream oss;
    double re = c.real();
    double im = c.imag();

    if (im == 0) {
        oss << re;
    } else if (re == 0) {
        if (im == 1) {
            oss << "i";
        } else if (im == -1) {
            oss << "-i";
        } else {
            oss << im << "i";
        }
    } else {
        oss << re;
        if (im > 0) {
            if (im == 1) {
                oss << " + i";
            } else {
                oss << " + " << im << "i";
            }
        } else {
            if (im == -1) {
                oss << " - i";
            } else {
                oss << " - " << (-im) << "i";
            }
        }
    }

    return oss.str();
}

}  // namespace calculator

#endif  // CALCULATOR_COMPLEX_PARSER_HPP
