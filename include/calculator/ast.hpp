/**
 * @file ast.hpp
 * @brief AST nodes for symbolic differentiation and expression representation
 */

#ifndef CALCULATOR_AST_HPP
#define CALCULATOR_AST_HPP

#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

namespace calculator {

struct ASTNode;
using ASTPtr = std::shared_ptr<ASTNode>;

enum class ASTNodeType {
    Number,
    Variable,
    BinaryOp,
    UnaryOp,
    Function
};

struct ASTNode {
    ASTNodeType type;
    double value = 0;
    std::string name;
    char op = 0;
    ASTPtr left;
    ASTPtr right;
    std::vector<ASTPtr> args;

    static ASTPtr makeNumber(double val) {
        auto node = std::make_shared<ASTNode>();
        node->type = ASTNodeType::Number;
        node->value = val;
        return node;
    }

    static ASTPtr makeVariable(const std::string& name) {
        auto node = std::make_shared<ASTNode>();
        node->type = ASTNodeType::Variable;
        node->name = name;
        return node;
    }

    static ASTPtr makeBinaryOp(char op, ASTPtr left, ASTPtr right) {
        auto node = std::make_shared<ASTNode>();
        node->type = ASTNodeType::BinaryOp;
        node->op = op;
        node->left = std::move(left);
        node->right = std::move(right);
        return node;
    }

    static ASTPtr makeUnaryOp(char op, ASTPtr operand) {
        auto node = std::make_shared<ASTNode>();
        node->type = ASTNodeType::UnaryOp;
        node->op = op;
        node->left = std::move(operand);
        return node;
    }

    static ASTPtr makeFunction(const std::string& name, std::vector<ASTPtr> args) {
        auto node = std::make_shared<ASTNode>();
        node->type = ASTNodeType::Function;
        node->name = name;
        node->args = std::move(args);
        return node;
    }

    ASTPtr clone() const {
        auto node = std::make_shared<ASTNode>();
        node->type = type;
        node->value = value;
        node->name = name;
        node->op = op;
        if (left) node->left = left->clone();
        if (right) node->right = right->clone();
        for (const auto& arg : args) {
            node->args.push_back(arg->clone());
        }
        return node;
    }
};

// Check if AST node is a constant number
inline bool isConstant(const ASTPtr& node) {
    return node && node->type == ASTNodeType::Number;
}

// Check if AST node is zero
inline bool isZero(const ASTPtr& node) {
    return isConstant(node) && node->value == 0;
}

// Check if AST node is one
inline bool isOne(const ASTPtr& node) {
    return isConstant(node) && node->value == 1;
}

// Simplify AST expression
inline ASTPtr simplify(const ASTPtr& node) {
    if (!node) return nullptr;

    switch (node->type) {
        case ASTNodeType::Number:
        case ASTNodeType::Variable:
            return node->clone();

        case ASTNodeType::UnaryOp: {
            auto simplified = simplify(node->left);
            if (node->op == '-' && isConstant(simplified)) {
                return ASTNode::makeNumber(-simplified->value);
            }
            if (node->op == '-' && simplified->type == ASTNodeType::UnaryOp && simplified->op == '-') {
                return simplified->left;
            }
            return ASTNode::makeUnaryOp(node->op, simplified);
        }

        case ASTNodeType::BinaryOp: {
            auto left = simplify(node->left);
            auto right = simplify(node->right);

            // Constant folding
            if (isConstant(left) && isConstant(right)) {
                double l = left->value;
                double r = right->value;
                switch (node->op) {
                    case '+': return ASTNode::makeNumber(l + r);
                    case '-': return ASTNode::makeNumber(l - r);
                    case '*': return ASTNode::makeNumber(l * r);
                    case '/': return r != 0 ? ASTNode::makeNumber(l / r) : ASTNode::makeBinaryOp('/', left, right);
                    case '^': return ASTNode::makeNumber(std::pow(l, r));
                }
            }

            // Addition simplifications
            if (node->op == '+') {
                if (isZero(left)) return right;
                if (isZero(right)) return left;
            }

            // Subtraction simplifications
            if (node->op == '-') {
                if (isZero(right)) return left;
                if (isZero(left)) return ASTNode::makeUnaryOp('-', right);
            }

            // Multiplication simplifications
            if (node->op == '*') {
                if (isZero(left) || isZero(right)) return ASTNode::makeNumber(0);
                if (isOne(left)) return right;
                if (isOne(right)) return left;
            }

            // Division simplifications
            if (node->op == '/') {
                if (isZero(left)) return ASTNode::makeNumber(0);
                if (isOne(right)) return left;
            }

            // Power simplifications
            if (node->op == '^') {
                if (isZero(right)) return ASTNode::makeNumber(1);
                if (isOne(right)) return left;
                if (isZero(left)) return ASTNode::makeNumber(0);
            }

            return ASTNode::makeBinaryOp(node->op, left, right);
        }

        case ASTNodeType::Function: {
            std::vector<ASTPtr> simplifiedArgs;
            for (const auto& arg : node->args) {
                simplifiedArgs.push_back(simplify(arg));
            }
            return ASTNode::makeFunction(node->name, simplifiedArgs);
        }
    }

    return node->clone();
}

// Convert AST to string representation
inline std::string astToString(const ASTPtr& node) {
    if (!node) return "";

    switch (node->type) {
        case ASTNodeType::Number: {
            std::ostringstream oss;
            if (node->value == static_cast<int>(node->value)) {
                oss << static_cast<int>(node->value);
            } else {
                oss << node->value;
            }
            return oss.str();
        }

        case ASTNodeType::Variable:
            return node->name;

        case ASTNodeType::UnaryOp:
            if (node->op == '-') {
                std::string inner = astToString(node->left);
                if (node->left->type == ASTNodeType::BinaryOp) {
                    return "-(" + inner + ")";
                }
                return "-" + inner;
            }
            return std::string(1, node->op) + astToString(node->left);

        case ASTNodeType::BinaryOp: {
            std::string left = astToString(node->left);
            std::string right = astToString(node->right);

            bool needLeftParen = false;
            bool needRightParen = false;

            // Determine if parentheses are needed based on precedence
            if (node->left->type == ASTNodeType::BinaryOp) {
                char leftOp = node->left->op;
                if ((node->op == '*' || node->op == '/' || node->op == '^') &&
                    (leftOp == '+' || leftOp == '-')) {
                    needLeftParen = true;
                }
            }

            if (node->right->type == ASTNodeType::BinaryOp) {
                char rightOp = node->right->op;
                if ((node->op == '*' || node->op == '/' || node->op == '^') &&
                    (rightOp == '+' || rightOp == '-')) {
                    needRightParen = true;
                }
                if (node->op == '-' && (rightOp == '+' || rightOp == '-')) {
                    needRightParen = true;
                }
                if (node->op == '/' && (rightOp == '*' || rightOp == '/')) {
                    needRightParen = true;
                }
            }

            if (needLeftParen) left = "(" + left + ")";
            if (needRightParen) right = "(" + right + ")";

            return left + " " + node->op + " " + right;
        }

        case ASTNodeType::Function: {
            std::string result = node->name + "(";
            for (size_t i = 0; i < node->args.size(); ++i) {
                if (i > 0) result += ", ";
                result += astToString(node->args[i]);
            }
            result += ")";
            return result;
        }
    }

    return "";
}

// Symbolic differentiation
inline ASTPtr differentiate(const ASTPtr& node, const std::string& var) {
    if (!node) return ASTNode::makeNumber(0);

    switch (node->type) {
        case ASTNodeType::Number:
            return ASTNode::makeNumber(0);

        case ASTNodeType::Variable:
            return ASTNode::makeNumber(node->name == var ? 1 : 0);

        case ASTNodeType::UnaryOp:
            if (node->op == '-') {
                return ASTNode::makeUnaryOp('-', differentiate(node->left, var));
            }
            return differentiate(node->left, var);

        case ASTNodeType::BinaryOp: {
            auto du = differentiate(node->left, var);
            auto dv = differentiate(node->right, var);

            switch (node->op) {
                case '+':
                    // d(u + v) = du + dv
                    return ASTNode::makeBinaryOp('+', du, dv);

                case '-':
                    // d(u - v) = du - dv
                    return ASTNode::makeBinaryOp('-', du, dv);

                case '*':
                    // d(u * v) = u*dv + v*du (product rule)
                    return ASTNode::makeBinaryOp('+',
                        ASTNode::makeBinaryOp('*', node->left->clone(), dv),
                        ASTNode::makeBinaryOp('*', node->right->clone(), du));

                case '/':
                    // d(u / v) = (v*du - u*dv) / v^2 (quotient rule)
                    return ASTNode::makeBinaryOp('/',
                        ASTNode::makeBinaryOp('-',
                            ASTNode::makeBinaryOp('*', node->right->clone(), du),
                            ASTNode::makeBinaryOp('*', node->left->clone(), dv)),
                        ASTNode::makeBinaryOp('^', node->right->clone(), ASTNode::makeNumber(2)));

                case '^':
                    // For u^v where v is constant: d(u^n) = n * u^(n-1) * du
                    // For general case: d(u^v) = u^v * (v' * ln(u) + v * u'/u)
                    if (isConstant(node->right)) {
                        double n = node->right->value;
                        return ASTNode::makeBinaryOp('*',
                            ASTNode::makeBinaryOp('*',
                                ASTNode::makeNumber(n),
                                ASTNode::makeBinaryOp('^', node->left->clone(), ASTNode::makeNumber(n - 1))),
                            du);
                    } else {
                        // General power rule: u^v * (v' * ln(u) + v * u'/u)
                        auto lnU = ASTNode::makeFunction("ln", {node->left->clone()});
                        return ASTNode::makeBinaryOp('*',
                            node->clone(),
                            ASTNode::makeBinaryOp('+',
                                ASTNode::makeBinaryOp('*', dv, lnU),
                                ASTNode::makeBinaryOp('*', node->right->clone(),
                                    ASTNode::makeBinaryOp('/', du, node->left->clone()))));
                    }
            }
            break;
        }

        case ASTNodeType::Function: {
            if (node->args.empty()) return ASTNode::makeNumber(0);

            auto arg = node->args[0];
            auto darg = differentiate(arg, var);

            // Chain rule: d(f(g(x))) = f'(g(x)) * g'(x)
            ASTPtr derivative;

            if (node->name == "sin") {
                // d(sin(u)) = cos(u) * du
                derivative = ASTNode::makeFunction("cos", {arg->clone()});
            } else if (node->name == "cos") {
                // d(cos(u)) = -sin(u) * du
                derivative = ASTNode::makeUnaryOp('-', ASTNode::makeFunction("sin", {arg->clone()}));
            } else if (node->name == "tan") {
                // d(tan(u)) = sec^2(u) * du = 1/cos^2(u) * du
                derivative = ASTNode::makeBinaryOp('/',
                    ASTNode::makeNumber(1),
                    ASTNode::makeBinaryOp('^', ASTNode::makeFunction("cos", {arg->clone()}), ASTNode::makeNumber(2)));
            } else if (node->name == "exp") {
                // d(exp(u)) = exp(u) * du
                derivative = ASTNode::makeFunction("exp", {arg->clone()});
            } else if (node->name == "ln" || node->name == "log") {
                // d(ln(u)) = 1/u * du
                derivative = ASTNode::makeBinaryOp('/', ASTNode::makeNumber(1), arg->clone());
            } else if (node->name == "sqrt") {
                // d(sqrt(u)) = 1/(2*sqrt(u)) * du
                derivative = ASTNode::makeBinaryOp('/',
                    ASTNode::makeNumber(1),
                    ASTNode::makeBinaryOp('*', ASTNode::makeNumber(2), ASTNode::makeFunction("sqrt", {arg->clone()})));
            } else if (node->name == "abs") {
                // d(|u|) = u/|u| * du = sign(u) * du
                derivative = ASTNode::makeFunction("sign", {arg->clone()});
            } else if (node->name == "asin") {
                // d(asin(u)) = 1/sqrt(1-u^2) * du
                derivative = ASTNode::makeBinaryOp('/',
                    ASTNode::makeNumber(1),
                    ASTNode::makeFunction("sqrt", {
                        ASTNode::makeBinaryOp('-', ASTNode::makeNumber(1),
                            ASTNode::makeBinaryOp('^', arg->clone(), ASTNode::makeNumber(2)))}));
            } else if (node->name == "acos") {
                // d(acos(u)) = -1/sqrt(1-u^2) * du
                derivative = ASTNode::makeUnaryOp('-',
                    ASTNode::makeBinaryOp('/',
                        ASTNode::makeNumber(1),
                        ASTNode::makeFunction("sqrt", {
                            ASTNode::makeBinaryOp('-', ASTNode::makeNumber(1),
                                ASTNode::makeBinaryOp('^', arg->clone(), ASTNode::makeNumber(2)))})));
            } else if (node->name == "atan") {
                // d(atan(u)) = 1/(1+u^2) * du
                derivative = ASTNode::makeBinaryOp('/',
                    ASTNode::makeNumber(1),
                    ASTNode::makeBinaryOp('+', ASTNode::makeNumber(1),
                        ASTNode::makeBinaryOp('^', arg->clone(), ASTNode::makeNumber(2))));
            } else {
                // Unknown function - return as-is with derivative notation
                return ASTNode::makeBinaryOp('*',
                    ASTNode::makeFunction(node->name + "'", {arg->clone()}),
                    darg);
            }

            return ASTNode::makeBinaryOp('*', derivative, darg);
        }
    }

    return ASTNode::makeNumber(0);
}

}  // namespace calculator

#endif  // CALCULATOR_AST_HPP
