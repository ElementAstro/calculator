/**
 * @file token.hpp
 * @brief Token types and Lexer for the calculator
 */

#ifndef CALCULATOR_TOKEN_HPP
#define CALCULATOR_TOKEN_HPP

#include <cctype>
#include <string>
#include <string_view>

namespace calculator {

enum class TokenType {
    Number,
    Identifier,
    Plus,
    Minus,
    Multiply,
    Divide,
    FloorDivide,  // //
    Modulo,
    Power,
    LParen,
    RParen,
    LBracket,     // [
    RBracket,     // ]
    Comma,
    Semicolon,    // ;
    Assign,
    PlusAssign,   // +=
    MinusAssign,  // -=
    MulAssign,    // *=
    DivAssign,    // /=
    Greater,
    Less,
    GreaterEqual,
    LessEqual,
    Equal,
    NotEqual,
    And,
    Or,
    Not,
    Question,
    Colon,
    BitwiseOr,
    BitwiseXor,
    BitwiseAnd,
    BitwiseNot,
    ShiftLeft,
    ShiftRight,
    Imaginary,
    End,
    Invalid
};

struct Token {
    TokenType type;
    std::string value;
    double numValue = 0;
    std::size_t position = 0;
};

class Lexer {
public:
    explicit Lexer(std::string_view input) : input_(input), pos_(0) {}

    Token nextToken() {
        skipWhitespace();

        if (pos_ >= input_.size()) {
            return {TokenType::End, "", 0, pos_};
        }

        std::size_t startPos = pos_;
        char c = input_[pos_];

        // Numbers
        if (std::isdigit(c) || (c == '.' && pos_ + 1 < input_.size() && std::isdigit(input_[pos_ + 1]))) {
            return parseNumber(startPos);
        }

        // Identifiers and keywords
        if (std::isalpha(c) || c == '_') {
            return parseIdentifier(startPos);
        }

        // Operators
        return parseOperator(startPos);
    }

    std::size_t position() const { return pos_; }
    std::string_view input() const { return input_; }

    void reset() { pos_ = 0; }
    void setPosition(std::size_t pos) { pos_ = pos; }

private:
    void skipWhitespace() {
        while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
            pos_++;
        }
    }

    Token parseNumber(std::size_t startPos) {
        std::string numStr;
        bool hasDecimal = false;
        bool hasExponent = false;

        // Check for hex
        if (input_[pos_] == '0' && pos_ + 1 < input_.size() &&
            (input_[pos_ + 1] == 'x' || input_[pos_ + 1] == 'X')) {
            numStr = "0x";
            pos_ += 2;
            while (pos_ < input_.size() && std::isxdigit(static_cast<unsigned char>(input_[pos_]))) {
                numStr += input_[pos_++];
            }
            if (numStr.size() == 2) {
                return {TokenType::Invalid, numStr, 0, startPos};
            }
            double val = static_cast<double>(std::stoll(numStr, nullptr, 16));
            return {TokenType::Number, numStr, val, startPos};
        }

        while (pos_ < input_.size()) {
            char c = input_[pos_];
            if (std::isdigit(static_cast<unsigned char>(c))) {
                numStr += c;
                pos_++;
            } else if (c == '.' && !hasDecimal && !hasExponent) {
                hasDecimal = true;
                numStr += c;
                pos_++;
            } else if ((c == 'e' || c == 'E') && !hasExponent && !numStr.empty()) {
                hasExponent = true;
                numStr += c;
                pos_++;
                if (pos_ < input_.size() && (input_[pos_] == '+' || input_[pos_] == '-')) {
                    numStr += input_[pos_++];
                }
            } else {
                break;
            }
        }

        // Check for imaginary unit
        if (pos_ < input_.size() && input_[pos_] == 'i') {
            pos_++;
            double val = numStr.empty() ? 1.0 : std::stod(numStr);
            return {TokenType::Imaginary, numStr + "i", val, startPos};
        }

        if (numStr.empty() || numStr == ".") {
            return {TokenType::Invalid, numStr, 0, startPos};
        }

        double val = std::stod(numStr);
        return {TokenType::Number, numStr, val, startPos};
    }

    Token parseIdentifier(std::size_t startPos) {
        std::string id;
        while (pos_ < input_.size() &&
               (std::isalnum(static_cast<unsigned char>(input_[pos_])) || input_[pos_] == '_')) {
            id += input_[pos_++];
        }

        // Check for imaginary unit 'i' alone
        if (id == "i") {
            return {TokenType::Imaginary, "i", 1.0, startPos};
        }

        return {TokenType::Identifier, id, 0, startPos};
    }

    Token parseOperator(std::size_t startPos) {
        char c = input_[pos_++];

        switch (c) {
            case '+':
                if (pos_ < input_.size() && input_[pos_] == '=') {
                    pos_++;
                    return {TokenType::PlusAssign, "+=", 0, startPos};
                }
                return {TokenType::Plus, "+", 0, startPos};
            case '-':
                if (pos_ < input_.size() && input_[pos_] == '=') {
                    pos_++;
                    return {TokenType::MinusAssign, "-=", 0, startPos};
                }
                return {TokenType::Minus, "-", 0, startPos};
            case '/':
                if (pos_ < input_.size() && input_[pos_] == '/') {
                    pos_++;
                    return {TokenType::FloorDivide, "//", 0, startPos};
                }
                if (pos_ < input_.size() && input_[pos_] == '=') {
                    pos_++;
                    return {TokenType::DivAssign, "/=", 0, startPos};
                }
                return {TokenType::Divide, "/", 0, startPos};
            case '%': return {TokenType::Modulo, "%", 0, startPos};
            case '(': return {TokenType::LParen, "(", 0, startPos};
            case ')': return {TokenType::RParen, ")", 0, startPos};
            case '[': return {TokenType::LBracket, "[", 0, startPos};
            case ']': return {TokenType::RBracket, "]", 0, startPos};
            case ',': return {TokenType::Comma, ",", 0, startPos};
            case ';': return {TokenType::Semicolon, ";", 0, startPos};
            case '?': return {TokenType::Question, "?", 0, startPos};
            case ':': return {TokenType::Colon, ":", 0, startPos};
            case '~': return {TokenType::BitwiseNot, "~", 0, startPos};

            case '*':
                if (pos_ < input_.size() && input_[pos_] == '*') {
                    pos_++;
                    return {TokenType::Power, "**", 0, startPos};
                }
                return {TokenType::Multiply, "*", 0, startPos};

            case '^':
                return {TokenType::Power, "^", 0, startPos};

            case '=':
                if (pos_ < input_.size() && input_[pos_] == '=') {
                    pos_++;
                    return {TokenType::Equal, "==", 0, startPos};
                }
                return {TokenType::Assign, "=", 0, startPos};

            case '!':
                if (pos_ < input_.size() && input_[pos_] == '=') {
                    pos_++;
                    return {TokenType::NotEqual, "!=", 0, startPos};
                }
                return {TokenType::Not, "!", 0, startPos};

            case '>':
                if (pos_ < input_.size() && input_[pos_] == '=') {
                    pos_++;
                    return {TokenType::GreaterEqual, ">=", 0, startPos};
                }
                if (pos_ < input_.size() && input_[pos_] == '>') {
                    pos_++;
                    return {TokenType::ShiftRight, ">>", 0, startPos};
                }
                return {TokenType::Greater, ">", 0, startPos};

            case '<':
                if (pos_ < input_.size() && input_[pos_] == '=') {
                    pos_++;
                    return {TokenType::LessEqual, "<=", 0, startPos};
                }
                if (pos_ < input_.size() && input_[pos_] == '<') {
                    pos_++;
                    return {TokenType::ShiftLeft, "<<", 0, startPos};
                }
                return {TokenType::Less, "<", 0, startPos};

            case '&':
                if (pos_ < input_.size() && input_[pos_] == '&') {
                    pos_++;
                    return {TokenType::And, "&&", 0, startPos};
                }
                return {TokenType::BitwiseAnd, "&", 0, startPos};

            case '|':
                if (pos_ < input_.size() && input_[pos_] == '|') {
                    pos_++;
                    return {TokenType::Or, "||", 0, startPos};
                }
                return {TokenType::BitwiseOr, "|", 0, startPos};

            default:
                return {TokenType::Invalid, std::string(1, c), 0, startPos};
        }
    }

    std::string_view input_;
    std::size_t pos_;
};

}  // namespace calculator

#endif  // CALCULATOR_TOKEN_HPP
