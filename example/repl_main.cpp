/**
 * @file repl_main.cpp
 * @brief REPL (Read-Eval-Print Loop) main entry point
 */

#include "calculator.hpp"
#include <iostream>

int main() {
    calculator::REPL<double> repl;
    repl.run();
    return 0;
}
