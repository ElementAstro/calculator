#include "calculator.hpp"

#include <iostream>

int main() {
    std::string expr = "12.34 + 56.78";
    double result = calculator::eval(expr);
    std::cout << "Result: " << result << std::endl;

    try {
        std::string expr2 = "12.34 + 56.78";
        int result2 = calculator::eval<int>(expr2);
        std::cout << "Result: " << result2 << std::endl;
    } catch (const calculator::error& e) {
        std::cerr << e.what() << std::endl;
    }
  
    return 0;
}