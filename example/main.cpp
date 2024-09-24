#include <iostream>

#include "calculator.hpp"

int main() {
    // 创建一个表达式解析器实例
    calculator::ExpressionParser<double> parser;

    // 设置变量
    parser.set("x", 5.0);
    parser.set("y", 10.0);

    // 设置函数
    parser.set("square", [](double x) { return x * x; });

    // 计算表达式
    try {
        double result1 = parser.eval("3 + 4 * 2 / (1 - 5) ** 2 ** 3");
        std::cout << "Result of expression 1: " << result1 << std::endl;

        double result2 = parser.eval("square(3) + x * y");
        std::cout << "Result of expression 2: " << result2 << std::endl;

        double result3 = parser.eval("x + y / 2");
        std::cout << "Result of expression 3: " << result3 << std::endl;
    } catch (const calculator::error &e) {
        std::cerr << "Error evaluating expression: " << e.what() << std::endl;
    }

    return 0;
}