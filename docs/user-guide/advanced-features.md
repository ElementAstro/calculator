# Advanced Features

This guide covers advanced usage patterns and optimization techniques for the Modern C++ Calculator library. Learn how to build complex mathematical systems, optimize performance, and handle sophisticated expression evaluation scenarios.

## Complex Expression Patterns

### Nested Function Calls

Build sophisticated mathematical expressions using nested function calls:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

int main() {
    calculator::ExpressionParser<double> parser;
    
    // Set up mathematical functions
    parser.set("sin", [](double x) { return std::sin(x); });
    parser.set("cos", [](double x) { return std::cos(x); });
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    parser.set("log", [](double x) { return std::log(x); });
    parser.set("exp", [](double x) { return std::exp(x); });
    
    // Set constants
    parser.set("pi", M_PI);
    parser.set("e", M_E);
    
    // Complex nested expressions
    double result1 = parser.eval("sin(cos(pi/4))");                    // sin(cos(π/4))
    double result2 = parser.eval("sqrt(sin(pi/2) ** 2 + cos(pi/2) ** 2)"); // Pythagorean identity
    double result3 = parser.eval("log(exp(5))");                       // Should equal 5
    double result4 = parser.eval("exp(log(10))");                      // Should equal 10
    
    std::cout << "sin(cos(π/4)) = " << result1 << std::endl;
    std::cout << "√(sin²(π/2) + cos²(π/2)) = " << result2 << std::endl;
    std::cout << "log(exp(5)) = " << result3 << std::endl;
    std::cout << "exp(log(10)) = " << result4 << std::endl;
    
    return 0;
}
```

### Multi-Variable Expressions

Handle expressions with multiple interdependent variables:

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_multivariable_system() {
    calculator::ExpressionParser<double> parser;
    
    // Physics: projectile motion
    parser.set("g", 9.81);          // Gravity
    parser.set("v0", 20.0);         // Initial velocity
    parser.set("angle", 45.0);      // Launch angle (degrees)
    parser.set("t", 2.0);           // Time
    
    // Convert angle to radians
    parser.set("pi", M_PI);
    parser.set("angle_rad", parser.eval("angle * pi / 180"));
    
    // Calculate velocity components
    parser.set("vx", parser.eval("v0 * cos(angle_rad)"));
    parser.set("vy", parser.eval("v0 * sin(angle_rad)"));
    
    // Position equations
    double x = parser.eval("vx * t");
    double y = parser.eval("vy * t - 0.5 * g * t ** 2");
    
    std::cout << "Projectile position at t=" << parser.eval("t") << "s:" << std::endl;
    std::cout << "x = " << x << " m" << std::endl;
    std::cout << "y = " << y << " m" << std::endl;
    
    // Calculate range and maximum height
    double flight_time = parser.eval("2 * vy / g");
    parser.set("flight_time", flight_time);
    
    double range = parser.eval("vx * flight_time");
    double max_height = parser.eval("vy ** 2 / (2 * g)");
    
    std::cout << "Range: " << range << " m" << std::endl;
    std::cout << "Max height: " << max_height << " m" << std::endl;
}
```

### Dynamic Expression Building

Build expressions programmatically based on runtime conditions:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <string>
#include <vector>

class DynamicFormulaBuilder {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    DynamicFormulaBuilder() {
        setup_functions();
    }
    
    void setup_functions() {
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("pow", [](double x) { return x; }); // Simplified
        parser_.set("abs", [](double x) { return std::abs(x); });
    }
    
    std::string build_polynomial(const std::vector<double>& coefficients) {
        std::string expression;
        
        for (size_t i = 0; i < coefficients.size(); ++i) {
            if (i > 0 && coefficients[i] >= 0) {
                expression += " + ";
            } else if (i > 0) {
                expression += " ";
            }
            
            expression += std::to_string(coefficients[i]);
            
            if (i > 0) {
                expression += " * x";
                if (i > 1) {
                    expression += " ** " + std::to_string(i);
                }
            }
        }
        
        return expression;
    }
    
    std::string build_distance_formula(int dimensions) {
        std::string expression = "sqrt(";
        
        for (int i = 1; i <= dimensions; ++i) {
            if (i > 1) expression += " + ";
            expression += "(x" + std::to_string(i) + " - y" + std::to_string(i) + ") ** 2";
        }
        
        expression += ")";
        return expression;
    }
    
    double evaluate(const std::string& expression) {
        return parser_.eval(expression);
    }
    
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
    }
};

int main() {
    DynamicFormulaBuilder builder;
    
    // Build and evaluate polynomial: 2 + 3x + x²
    std::vector<double> coeffs = {2.0, 3.0, 1.0};
    std::string poly = builder.build_polynomial(coeffs);
    
    builder.set_variable("x", 5.0);
    double poly_result = builder.evaluate(poly);
    
    std::cout << "Polynomial: " << poly << std::endl;
    std::cout << "Result at x=5: " << poly_result << std::endl;
    
    // Build and evaluate 3D distance formula
    std::string distance_3d = builder.build_distance_formula(3);
    
    builder.set_variable("x1", 1.0);
    builder.set_variable("y1", 2.0);
    builder.set_variable("x2", 4.0);
    builder.set_variable("y2", 6.0);
    builder.set_variable("x3", 0.0);
    builder.set_variable("y3", 3.0);
    
    double distance = builder.evaluate(distance_3d);
    
    std::cout << "3D Distance formula: " << distance_3d << std::endl;
    std::cout << "Distance: " << distance << std::endl;
    
    return 0;
}
```

## Performance Optimization

### Expression Caching

Cache frequently used expressions to improve performance:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <unordered_map>
#include <chrono>

class CachedCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    std::unordered_map<std::string, double> cache_;
    
public:
    CachedCalculator() {
        setup_functions();
    }
    
    void setup_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("log", [](double x) { return std::log(x); });
    }
    
    double evaluate_cached(const std::string& expression) {
        // Check cache first
        auto it = cache_.find(expression);
        if (it != cache_.end()) {
            return it->second;
        }
        
        // Evaluate and cache result
        double result = parser_.eval(expression);
        cache_[expression] = result;
        return result;
    }
    
    double evaluate_direct(const std::string& expression) {
        return parser_.eval(expression);
    }
    
    void clear_cache() {
        cache_.clear();
    }
    
    size_t cache_size() const {
        return cache_.size();
    }
    
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
        // Clear cache when variables change
        clear_cache();
    }
};

void benchmark_caching() {
    CachedCalculator calc;
    calc.set_variable("pi", M_PI);
    
    std::vector<std::string> expressions = {
        "sin(pi/2) + cos(0)",
        "sqrt(16) + log(2.718)",
        "sin(pi/4) * cos(pi/4)",
        "sqrt(sin(pi/2) ** 2 + cos(pi/2) ** 2)"
    };
    
    const int iterations = 10000;
    
    // Benchmark direct evaluation
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        for (const auto& expr : expressions) {
            calc.evaluate_direct(expr);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto direct_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Benchmark cached evaluation
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        for (const auto& expr : expressions) {
            calc.evaluate_cached(expr);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto cached_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Direct evaluation: " << direct_time.count() << " μs" << std::endl;
    std::cout << "Cached evaluation: " << cached_time.count() << " μs" << std::endl;
    std::cout << "Speedup: " << (double)direct_time.count() / cached_time.count() << "x" << std::endl;
    std::cout << "Cache size: " << calc.cache_size() << " entries" << std::endl;
}
```

### Batch Processing

Process multiple expressions efficiently:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>
#include <future>
#include <thread>

class BatchCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    BatchCalculator() {
        setup_common_functions();
    }
    
    void setup_common_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
    }
    
    std::vector<double> evaluate_batch(const std::vector<std::string>& expressions) {
        std::vector<double> results;
        results.reserve(expressions.size());
        
        for (const auto& expr : expressions) {
            try {
                results.push_back(parser_.eval(expr));
            } catch (const calculator::error& e) {
                std::cerr << "Error in expression '" << expr << "': " << e.what() << std::endl;
                results.push_back(std::numeric_limits<double>::quiet_NaN());
            }
        }
        
        return results;
    }
    
    // Parallel batch processing (each thread needs its own parser)
    std::vector<double> evaluate_batch_parallel(const std::vector<std::string>& expressions) {
        const size_t num_threads = std::thread::hardware_concurrency();
        const size_t chunk_size = expressions.size() / num_threads;
        
        std::vector<std::future<std::vector<double>>> futures;
        
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start = i * chunk_size;
            size_t end = (i == num_threads - 1) ? expressions.size() : (i + 1) * chunk_size;
            
            auto chunk = std::vector<std::string>(expressions.begin() + start, expressions.begin() + end);
            
            futures.push_back(std::async(std::launch::async, [chunk]() {
                BatchCalculator local_calc;
                return local_calc.evaluate_batch(chunk);
            }));
        }
        
        std::vector<double> results;
        for (auto& future : futures) {
            auto chunk_results = future.get();
            results.insert(results.end(), chunk_results.begin(), chunk_results.end());
        }
        
        return results;
    }
    
    void set_variable(const std::string& name, double value) {
        parser_.set(name, value);
    }
};

int main() {
    BatchCalculator calc;
    
    // Prepare batch of expressions
    std::vector<std::string> expressions = {
        "sin(pi/2)",
        "cos(0)",
        "sqrt(16)",
        "log(e)",
        "pi * 2",
        "e ** 2",
        "sin(pi/4) + cos(pi/4)",
        "sqrt(2) / 2"
    };
    
    // Sequential processing
    auto start = std::chrono::high_resolution_clock::now();
    auto results_seq = calc.evaluate_batch(expressions);
    auto end = std::chrono::high_resolution_clock::now();
    auto seq_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Parallel processing
    start = std::chrono::high_resolution_clock::now();
    auto results_par = calc.evaluate_batch_parallel(expressions);
    end = std::chrono::high_resolution_clock::now();
    auto par_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Sequential time: " << seq_time.count() << " μs" << std::endl;
    std::cout << "Parallel time: " << par_time.count() << " μs" << std::endl;
    
    // Display results
    for (size_t i = 0; i < expressions.size(); ++i) {
        std::cout << expressions[i] << " = " << results_seq[i] << std::endl;
    }
    
    return 0;
}
```

## Advanced Mathematical Systems

### Equation Solver Framework

Build a framework for solving mathematical equations:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <cmath>

class EquationSolver {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    EquationSolver() {
        setup_functions();
    }
    
    void setup_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("pi", M_PI);
    }
    
    // Simple bisection method for root finding
    double find_root(const std::string& expression, double a, double b, double tolerance = 1e-6) {
        const int max_iterations = 1000;
        
        for (int i = 0; i < max_iterations; ++i) {
            double c = (a + b) / 2.0;
            
            parser_.set("x", a);
            double fa = parser_.eval(expression);
            
            parser_.set("x", c);
            double fc = parser_.eval(expression);
            
            if (std::abs(fc) < tolerance || (b - a) / 2.0 < tolerance) {
                return c;
            }
            
            if (fa * fc < 0) {
                b = c;
            } else {
                a = c;
            }
        }
        
        return (a + b) / 2.0;
    }
    
    // Newton's method for root finding
    double newton_method(const std::string& f, const std::string& df, 
                        double x0, double tolerance = 1e-6) {
        const int max_iterations = 100;
        double x = x0;
        
        for (int i = 0; i < max_iterations; ++i) {
            parser_.set("x", x);
            double fx = parser_.eval(f);
            double dfx = parser_.eval(df);
            
            if (std::abs(fx) < tolerance) {
                return x;
            }
            
            if (std::abs(dfx) < tolerance) {
                throw std::runtime_error("Derivative too small");
            }
            
            x = x - fx / dfx;
        }
        
        return x;
    }
    
    // Numerical integration using Simpson's rule
    double integrate(const std::string& expression, double a, double b, int n = 1000) {
        if (n % 2 != 0) n++; // Ensure n is even
        
        double h = (b - a) / n;
        double sum = 0.0;
        
        // Simpson's rule
        for (int i = 0; i <= n; ++i) {
            double x = a + i * h;
            parser_.set("x", x);
            double fx = parser_.eval(expression);
            
            if (i == 0 || i == n) {
                sum += fx;
            } else if (i % 2 == 1) {
                sum += 4 * fx;
            } else {
                sum += 2 * fx;
            }
        }
        
        return sum * h / 3.0;
    }
};

int main() {
    EquationSolver solver;
    
    // Find root of x² - 2 = 0 (should be √2 ≈ 1.414)
    double root1 = solver.find_root("x ** 2 - 2", 1.0, 2.0);
    std::cout << "Root of x² - 2 = 0: " << root1 << std::endl;
    
    // Find root using Newton's method
    // f(x) = x² - 2, f'(x) = 2x
    double root2 = solver.newton_method("x ** 2 - 2", "2 * x", 1.5);
    std::cout << "Root using Newton's method: " << root2 << std::endl;
    
    // Integrate x² from 0 to 1 (should be 1/3 ≈ 0.333)
    double integral = solver.integrate("x ** 2", 0.0, 1.0);
    std::cout << "∫₀¹ x² dx = " << integral << std::endl;
    
    // Integrate sin(x) from 0 to π (should be 2)
    double integral_sin = solver.integrate("sin(x)", 0.0, M_PI);
    std::cout << "∫₀π sin(x) dx = " << integral_sin << std::endl;
    
    return 0;
}
```

## Memory Management and Optimization

### Smart Variable Management

Implement efficient variable storage and retrieval:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <unordered_map>
#include <memory>

class OptimizedCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    std::unordered_map<std::string, double> variable_cache_;
    
public:
    OptimizedCalculator() {
        setup_core_functions();
    }
    
    void setup_core_functions() {
        // Only set up essential functions to minimize memory usage
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("log", [](double x) { return std::log(x); });
    }
    
    void set_variable(const std::string& name, double value) {
        variable_cache_[name] = value;
        parser_.set(name, value);
    }
    
    double get_variable(const std::string& name) const {
        auto it = variable_cache_.find(name);
        if (it != variable_cache_.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable not found: " + name);
    }
    
    void remove_variable(const std::string& name) {
        variable_cache_.erase(name);
        // Note: calculator library doesn't support removing variables
        // In a real implementation, you might need to recreate the parser
    }
    
    std::vector<std::string> list_variables() const {
        std::vector<std::string> names;
        for (const auto& pair : variable_cache_) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    double evaluate(const std::string& expression) {
        return parser_.eval(expression);
    }
    
    void clear_variables() {
        variable_cache_.clear();
        // Recreate parser to clear variables
        parser_ = calculator::ExpressionParser<double>();
        setup_core_functions();
    }
    
    size_t memory_usage_estimate() const {
        size_t size = sizeof(*this);
        for (const auto& pair : variable_cache_) {
            size += pair.first.size() + sizeof(pair.second);
        }
        return size;
    }
};

int main() {
    OptimizedCalculator calc;
    
    // Set up variables
    calc.set_variable("pi", M_PI);
    calc.set_variable("e", M_E);
    calc.set_variable("golden_ratio", (1.0 + std::sqrt(5.0)) / 2.0);
    
    // List variables
    auto variables = calc.list_variables();
    std::cout << "Variables: ";
    for (const auto& name : variables) {
        std::cout << name << "=" << calc.get_variable(name) << " ";
    }
    std::cout << std::endl;
    
    // Perform calculations
    double result1 = calc.evaluate("sin(pi/2)");
    double result2 = calc.evaluate("log(e)");
    double result3 = calc.evaluate("golden_ratio ** 2 - golden_ratio - 1");
    
    std::cout << "sin(π/2) = " << result1 << std::endl;
    std::cout << "log(e) = " << result2 << std::endl;
    std::cout << "φ² - φ - 1 = " << result3 << std::endl;
    
    std::cout << "Estimated memory usage: " << calc.memory_usage_estimate() << " bytes" << std::endl;
    
    return 0;
}
```

## Next Steps

- Review [Error Handling](error-handling.md) for robust error management
- Explore [Examples](../examples/advanced-examples.md) for real-world applications
- Check [API Reference](../api-reference/) for complete technical documentation
- See [Performance Reference](../reference/limitations.md) for optimization guidelines
