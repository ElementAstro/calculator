/**
 * @file repl.hpp
 * @brief REPL (Read-Eval-Print Loop) with history support
 */

#ifndef CALCULATOR_REPL_HPP
#define CALCULATOR_REPL_HPP

#include "parser.hpp"
#include "complex_parser.hpp"

#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace calculator {

template <typename T>
class REPL {
public:
    REPL() : maxHistorySize_(100), running_(false) {}

    void run(std::istream& in = std::cin, std::ostream& out = std::cout) {
        running_ = true;
        printWelcome(out);

        std::string line;
        while (running_ && std::getline(in, line)) {
            if (line.empty()) {
                out << "> ";
                continue;
            }

            processCommand(line, out);

            if (running_) {
                out << "> ";
            }
        }
    }

    void stop() {
        running_ = false;
    }

    ExpressionParser<T>& parser() {
        return parser_;
    }

    const std::deque<std::string>& history() const {
        return history_;
    }

    void clearHistory() {
        history_.clear();
    }

    void setMaxHistorySize(std::size_t size) {
        maxHistorySize_ = size;
        while (history_.size() > maxHistorySize_) {
            history_.pop_front();
        }
    }

    bool saveHistory(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) return false;

        for (const auto& entry : history_) {
            file << entry << "\n";
        }
        return true;
    }

    bool loadHistory(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) return false;

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                addToHistory(line);
            }
        }
        return true;
    }

private:
    void printWelcome(std::ostream& out) {
        out << "Calculator v2.0 - Advanced Expression Calculator\n";
        out << "Type 'help' for available commands, 'quit' to exit.\n";
        out << "> ";
    }

    void addToHistory(const std::string& entry) {
        if (history_.empty() || history_.back() != entry) {
            history_.push_back(entry);
            if (history_.size() > maxHistorySize_) {
                history_.pop_front();
            }
        }
    }

    void processCommand(const std::string& line, std::ostream& out) {
        std::string trimmed = trim(line);

        if (trimmed.empty()) {
            return;
        }

        // Check for special commands
        if (trimmed == "quit" || trimmed == "exit" || trimmed == "q") {
            out << "Goodbye!\n";
            running_ = false;
            return;
        }

        if (trimmed == "help" || trimmed == "?") {
            printHelp(out);
            return;
        }

        if (trimmed == "history" || trimmed == "hist") {
            printHistory(out);
            return;
        }

        if (trimmed == "clear") {
            parser_.clearVariables();
            parser_.clearUserFunctions();
            out << "All variables and user functions cleared.\n";
            return;
        }

        if (trimmed == "vars" || trimmed == "variables") {
            printVariables(out);
            return;
        }

        if (trimmed == "funcs" || trimmed == "functions") {
            printFunctions(out);
            return;
        }

        if (trimmed.substr(0, 5) == "diff ") {
            processDiff(trimmed.substr(5), out);
            return;
        }

        if (trimmed.substr(0, 5) == "save ") {
            std::string filename = trim(trimmed.substr(5));
            if (saveHistory(filename)) {
                out << "History saved to " << filename << "\n";
            } else {
                out << "Error: Could not save history to " << filename << "\n";
            }
            return;
        }

        if (trimmed.substr(0, 5) == "load ") {
            std::string filename = trim(trimmed.substr(5));
            if (loadHistory(filename)) {
                out << "History loaded from " << filename << "\n";
            } else {
                out << "Error: Could not load history from " << filename << "\n";
            }
            return;
        }

        // Regular expression evaluation
        addToHistory(trimmed);

        try {
            T result = parser_.eval(trimmed);
            printResult(result, out);
        } catch (const error& e) {
            out << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            out << "Error: " << e.what() << "\n";
        }
    }

    void printHelp(std::ostream& out) {
        out << "\n=== Calculator Help ===\n\n";
        out << "Basic Operations:\n";
        out << "  +, -, *, /     Arithmetic operators\n";
        out << "  //             Floor division (integer division)\n";
        out << "  %              Modulo\n";
        out << "  ^ or **        Power (right associative)\n";
        out << "  ()             Parentheses\n\n";

        out << "Comparison Operators:\n";
        out << "  >, <, >=, <=   Comparison (returns 1 or 0)\n";
        out << "  ==, !=         Equality (returns 1 or 0)\n\n";

        out << "Logical Operators:\n";
        out << "  &&             Logical AND\n";
        out << "  ||             Logical OR\n";
        out << "  !              Logical NOT\n\n";

        out << "Bitwise Operators (integers):\n";
        out << "  &              Bitwise AND\n";
        out << "  |              Bitwise OR\n";
        out << "  ~              Bitwise NOT\n";
        out << "  <<, >>         Shift left/right\n\n";

        out << "Conditional Expression:\n";
        out << "  condition ? true_value : false_value\n\n";

        out << "Constants:\n";
        out << "  Math: pi, e, phi, tau, sqrt2, sqrt3, ln2, ln10\n";
        out << "  Special: inf, nan\n";
        out << "  Physics: c (light), G, h (Planck), kb, Na, qe\n\n";

        out << "Trigonometric Functions:\n";
        out << "  sin, cos, tan, cot, sec, csc\n";
        out << "  asin, acos, atan, acot, asec, acsc\n";
        out << "  sinh, cosh, tanh, coth, sech, csch\n";
        out << "  asinh, acosh, atanh\n\n";

        out << "Mathematical Functions:\n";
        out << "  sqrt, cbrt, sqr, cube, abs, sign\n";
        out << "  log (base 10), ln, log2, exp, exp2\n";
        out << "  floor, ceil, round, trunc, frac\n";
        out << "  fact/factorial, gamma, lgamma\n";
        out << "  erf, erfc (error functions)\n";
        out << "  deg, rad (angle conversion)\n";
        out << "  recip/inv (reciprocal)\n";
        out << "  heaviside (step function)\n\n";

        out << "Two-Argument Functions:\n";
        out << "  pow(x,y), hypot(x,y), atan2(y,x)\n";
        out << "  max(x,y), min(x,y), mod(x,y)\n";
        out << "  gcd(a,b), lcm(a,b)\n";
        out << "  nCr(n,r)/comb, nPr(n,r)/perm\n";
        out << "  logb(x,base), avg(x,y)\n";
        out << "  clamp(x,max), wrap(x,max)\n\n";

        out << "User-Defined Functions:\n";
        out << "  f(x) = x^2 + 2*x + 1\n";
        out << "  g(x, y) = x^2 + y^2\n";
        out << "  f(3)           Call function\n\n";

        out << "Commands:\n";
        out << "  help, ?        Show this help\n";
        out << "  quit, exit, q  Exit calculator\n";
        out << "  history, hist  Show command history\n";
        out << "  vars           Show defined variables\n";
        out << "  funcs          Show defined functions\n";
        out << "  clear          Clear all variables and functions\n";
        out << "  diff <expr>    Differentiate expression (w.r.t. x)\n";
        out << "  save <file>    Save history to file\n";
        out << "  load <file>    Load history from file\n\n";

        out << "Examples:\n";
        out << "  2 + 3 * 4              = 14\n";
        out << "  7 // 3                 = 2 (floor division)\n";
        out << "  sqrt(16) + 2^3         = 12\n";
        out << "  sin(pi/2)              = 1\n";
        out << "  fact(5)                = 120\n";
        out << "  nCr(10, 3)             = 120\n";
        out << "  gcd(48, 18)            = 6\n";
        out << "  10 > 5 ? 1 : 0         = 1\n";
        out << "  f(x) = x^2; f(3)       = 9\n";
        out << "  diff x^3 + 2*x         = 3 * x ^ 2 + 2\n\n";
    }

    void printHistory(std::ostream& out) {
        if (history_.empty()) {
            out << "History is empty.\n";
            return;
        }

        out << "\n=== Command History ===\n";
        std::size_t index = 1;
        for (const auto& entry : history_) {
            out << "  " << index++ << ": " << entry << "\n";
        }
        out << "\n";
    }

    void printVariables(std::ostream& out) {
        auto names = parser_.getVariableNames();
        if (names.empty()) {
            out << "No variables defined.\n";
            return;
        }

        out << "\n=== Defined Variables ===\n";
        for (const auto& name : names) {
            out << "  " << name << " = " << parser_.get(name) << "\n";
        }
        out << "\n";
    }

    void printFunctions(std::ostream& out) {
        auto names = parser_.getFunctionNames();
        if (names.empty()) {
            out << "No functions defined.\n";
            return;
        }

        out << "\n=== Available Functions ===\n";
        out << "Built-in: ";
        bool first = true;
        for (const auto& name : names) {
            auto info = parser_.getFunctionInfo(name);
            if (!info) {  // Built-in function
                if (!first) out << ", ";
                out << name;
                first = false;
            }
        }
        out << "\n\nUser-defined:\n";
        for (const auto& name : names) {
            auto info = parser_.getFunctionInfo(name);
            if (info) {
                out << "  " << name << "(";
                for (std::size_t i = 0; i < info->first.size(); ++i) {
                    if (i > 0) out << ", ";
                    out << info->first[i];
                }
                out << ") = " << info->second << "\n";
            }
        }
        out << "\n";
    }

    void processDiff(const std::string& expr, std::ostream& out) {
        std::string trimmedExpr = trim(expr);
        std::string var = "x";

        // Check if variable is specified: "diff x^2 y" means diff w.r.t. y
        std::size_t lastSpace = trimmedExpr.rfind(' ');
        if (lastSpace != std::string::npos) {
            std::string possibleVar = trimmedExpr.substr(lastSpace + 1);
            if (possibleVar.size() == 1 && std::isalpha(possibleVar[0])) {
                var = possibleVar;
                trimmedExpr = trim(trimmedExpr.substr(0, lastSpace));
            }
        }

        try {
            std::string derivative = parser_.diff(trimmedExpr, var);
            out << "d/d" << var << "(" << trimmedExpr << ") = " << derivative << "\n";
        } catch (const error& e) {
            out << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            out << "Error: " << e.what() << "\n";
        }
    }

    void printResult(T result, std::ostream& out) {
        out << result << "\n";
    }

    static std::string trim(const std::string& str) {
        std::size_t start = 0;
        std::size_t end = str.size();

        while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
            start++;
        }
        while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
            end--;
        }

        return str.substr(start, end - start);
    }

    ExpressionParser<T> parser_;
    std::deque<std::string> history_;
    std::size_t maxHistorySize_;
    bool running_;
};

// Specialization for complex numbers
template <>
inline void REPL<std::complex<double>>::printResult(std::complex<double> result, std::ostream& out) {
    out << formatComplex(result) << "\n";
}

// Specialization for complex - no diff support
template <>
inline void REPL<std::complex<double>>::processDiff(const std::string&, std::ostream& out) {
    out << "Differentiation is not supported for complex numbers.\n";
}

}  // namespace calculator

#endif  // CALCULATOR_REPL_HPP
