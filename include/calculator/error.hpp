/**
 * @file error.hpp
 * @brief Error handling for the calculator library
 */

#ifndef CALCULATOR_ERROR_HPP
#define CALCULATOR_ERROR_HPP

#include <stdexcept>
#include <string>
#include <string_view>

namespace calculator {

class error : public std::runtime_error {
public:
    error(std::string_view expr, std::string_view message)
        : std::runtime_error(std::string(message)), expr_(expr) {}

    error(const std::string& message)
        : std::runtime_error(message), expr_("") {}

    std::string_view expression() const noexcept { return expr_; }

private:
    std::string expr_;
};

class syntax_error : public error {
public:
    syntax_error(std::string_view expr, std::string_view message, std::size_t position)
        : error(expr, std::string(message) + " at position " + std::to_string(position)),
          position_(position) {}

    std::size_t position() const noexcept { return position_; }

private:
    std::size_t position_;
};

class math_error : public error {
public:
    math_error(std::string_view expr, std::string_view message)
        : error(expr, message) {}
};

class undefined_error : public error {
public:
    undefined_error(std::string_view expr, const std::string& name, bool isFunction)
        : error(expr, (isFunction ? "Undefined function: " : "Undefined variable: ") + name),
          name_(name), isFunction_(isFunction) {}

    const std::string& name() const noexcept { return name_; }
    bool isFunction() const noexcept { return isFunction_; }

private:
    std::string name_;
    bool isFunction_;
};

}  // namespace calculator

#endif  // CALCULATOR_ERROR_HPP
