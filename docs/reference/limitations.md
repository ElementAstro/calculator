# Limitations

This page documents the current limitations of the Modern C++ Calculator library, along with workarounds and planned improvements. Understanding these limitations helps you design robust applications and avoid common pitfalls.

## Current Limitations

### 1. Single-Parameter Functions Only

**Limitation**: Custom functions can only accept one parameter.

```cpp
// ❌ Not supported: Multi-parameter functions
// parser.set("max", [](double a, double b) { return std::max(a, b); });

// ✅ Workaround: Use variables
calculator::ExpressionParser<double> parser;
parser.set("a", 5.0);
parser.set("b", 3.0);
parser.set("max_ab", [&parser](double unused) {
    double a = /* get from parser somehow */;  // Complex workaround needed
    double b = /* get from parser somehow */;
    return std::max(a, b);
});
```

**Workarounds**:

```cpp
// Option 1: Use variables for additional parameters
void demonstrate_multi_param_workaround() {
    calculator::ExpressionParser<double> parser;
    
    // Set up variables
    parser.set("x", 5.0);
    parser.set("y", 3.0);
    
    // Define function that uses variables
    parser.set("distance", [&parser](double z) {
        // Access x and y from the parser context (implementation-specific)
        // This is a conceptual example - actual implementation may vary
        return std::sqrt(parser.eval("x ** 2 + y ** 2") + z * z);
    });
    
    double result = parser.eval("distance(4)");  // 3D distance
}

// Option 2: Create specialized calculators
class GeometryCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    GeometryCalculator() {
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
    }
    
    double distance_2d(double x1, double y1, double x2, double y2) {
        parser_.set("dx", x2 - x1);
        parser_.set("dy", y2 - y1);
        return parser_.eval("sqrt(dx ** 2 + dy ** 2)");
    }
    
    double distance_3d(double x1, double y1, double z1, double x2, double y2, double z2) {
        parser_.set("dx", x2 - x1);
        parser_.set("dy", y2 - y1);
        parser_.set("dz", z2 - z1);
        return parser_.eval("sqrt(dx ** 2 + dy ** 2 + dz ** 2)");
    }
};
```

### 2. No Variable Removal

**Limitation**: Once set, variables cannot be removed from the parser.

```cpp
// ❌ Not supported: Variable removal
calculator::ExpressionParser<double> parser;
parser.set("temp_var", 42.0);
// parser.remove("temp_var");  // No such method exists
```

**Workarounds**:

```cpp
// Option 1: Create new parser instance
void demonstrate_variable_removal_workaround() {
    auto create_fresh_parser = []() {
        calculator::ExpressionParser<double> parser;
        // Set up standard functions and constants
        parser.set("pi", M_PI);
        parser.set("e", M_E);
        parser.set("sin", [](double x) { return std::sin(x); });
        parser.set("cos", [](double x) { return std::cos(x); });
        return parser;
    };
    
    // Use fresh parser when needed
    auto parser1 = create_fresh_parser();
    parser1.set("temp_var", 42.0);
    double result1 = parser1.eval("temp_var * 2");
    
    // Create new parser without temp_var
    auto parser2 = create_fresh_parser();
    // temp_var is not available here
}

// Option 2: Use scoped calculators
class ScopedCalculator {
private:
    std::unique_ptr<calculator::ExpressionParser<double>> parser_;
    
public:
    ScopedCalculator() {
        reset();
    }
    
    void reset() {
        parser_ = std::make_unique<calculator::ExpressionParser<double>>();
        setup_defaults();
    }
    
    void setup_defaults() {
        parser_->set("pi", M_PI);
        parser_->set("e", M_E);
        parser_->set("sin", [](double x) { return std::sin(x); });
        parser_->set("cos", [](double x) { return std::cos(x); });
    }
    
    void set_variable(const std::string& name, double value) {
        parser_->set(name, value);
    }
    
    double eval(const std::string& expression) {
        return parser_->eval(expression);
    }
    
    void clear_variables() {
        reset();  // Nuclear option: recreate parser
    }
};
```

### 3. No Complex Numbers

**Limitation**: Only real numbers are supported.

```cpp
// ❌ Not supported: Complex number operations
// calculator::eval("(3 + 4i) * (1 - 2i)");
// calculator::eval("sqrt(-1)");
```

**Workarounds**:

```cpp
// Option 1: Implement complex arithmetic manually
class ComplexCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    ComplexCalculator() {
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
    }
    
    struct Complex {
        double real, imag;
        
        Complex operator+(const Complex& other) const {
            return {real + other.real, imag + other.imag};
        }
        
        Complex operator*(const Complex& other) const {
            return {
                real * other.real - imag * other.imag,
                real * other.imag + imag * other.real
            };
        }
        
        double magnitude() const {
            parser_.set("r", real);
            parser_.set("i", imag);
            return parser_.eval("sqrt(r ** 2 + i ** 2)");
        }
    };
    
    Complex add(const Complex& a, const Complex& b) {
        return a + b;
    }
    
    Complex multiply(const Complex& a, const Complex& b) {
        return a * b;
    }
};

// Option 2: Use separate real and imaginary parts
void demonstrate_complex_workaround() {
    calculator::ExpressionParser<double> parser;
    parser.set("sqrt", [](double x) { return std::sqrt(x); });
    
    // Represent complex number (3 + 4i)
    parser.set("a_real", 3.0);
    parser.set("a_imag", 4.0);
    
    // Represent complex number (1 - 2i)
    parser.set("b_real", 1.0);
    parser.set("b_imag", -2.0);
    
    // Complex multiplication: (a + bi)(c + di) = (ac - bd) + (ad + bc)i
    double result_real = parser.eval("a_real * b_real - a_imag * b_imag");  // 11
    double result_imag = parser.eval("a_real * b_imag + a_imag * b_real");  // -2
    
    std::cout << "Result: " << result_real << " + " << result_imag << "i" << std::endl;
}
```

### 4. No Matrix Operations

**Limitation**: No built-in support for matrices or vectors.

```cpp
// ❌ Not supported: Matrix operations
// calculator::eval("[[1,2],[3,4]] * [[5,6],[7,8]]");
```

**Workarounds**:

```cpp
// Option 1: Implement matrix operations externally
class MatrixCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    MatrixCalculator() {
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
    }
    
    std::vector<std::vector<double>> multiply_2x2(
        const std::vector<std::vector<double>>& a,
        const std::vector<std::vector<double>>& b) {
        
        std::vector<std::vector<double>> result(2, std::vector<double>(2));
        
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                parser_.set("sum", 0.0);
                for (int k = 0; k < 2; ++k) {
                    parser_.set("a_elem", a[i][k]);
                    parser_.set("b_elem", b[k][j]);
                    parser_.set("sum", parser_.eval("sum + a_elem * b_elem"));
                }
                result[i][j] = parser_.eval("sum");
            }
        }
        
        return result;
    }
    
    double determinant_2x2(const std::vector<std::vector<double>>& matrix) {
        parser_.set("a", matrix[0][0]);
        parser_.set("b", matrix[0][1]);
        parser_.set("c", matrix[1][0]);
        parser_.set("d", matrix[1][1]);
        
        return parser_.eval("a * d - b * c");
    }
};

// Option 2: Use individual element calculations
void demonstrate_matrix_workaround() {
    calculator::ExpressionParser<double> parser;
    
    // Matrix A = [[1, 2], [3, 4]]
    parser.set("a11", 1.0); parser.set("a12", 2.0);
    parser.set("a21", 3.0); parser.set("a22", 4.0);
    
    // Matrix B = [[5, 6], [7, 8]]
    parser.set("b11", 5.0); parser.set("b12", 6.0);
    parser.set("b21", 7.0); parser.set("b22", 8.0);
    
    // Matrix multiplication C = A * B
    double c11 = parser.eval("a11 * b11 + a12 * b21");  // 19
    double c12 = parser.eval("a11 * b12 + a12 * b22");  // 22
    double c21 = parser.eval("a21 * b11 + a22 * b21");  // 43
    double c22 = parser.eval("a21 * b12 + a22 * b22");  // 50
    
    std::cout << "Result matrix: [[" << c11 << ", " << c12 << "], ["
              << c21 << ", " << c22 << "]]" << std::endl;
}
```

### 5. Limited String/Symbol Support

**Limitation**: No string manipulation or symbolic mathematics.

```cpp
// ❌ Not supported: String operations
// calculator::eval("'hello' + 'world'");

// ❌ Not supported: Symbolic math
// calculator::eval("d/dx(x^2)");  // Derivatives
// calculator::eval("integrate(x^2, x)");  // Integrals
```

**Workarounds**:

```cpp
// Option 1: Implement symbolic differentiation
class SymbolicCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    SymbolicCalculator() {
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
    }
    
    // Numerical derivative using finite differences
    double derivative(const std::string& expression, double x, double h = 1e-8) {
        parser_.set("x", x + h);
        double f_plus_h = parser_.eval(expression);
        
        parser_.set("x", x - h);
        double f_minus_h = parser_.eval(expression);
        
        return (f_plus_h - f_minus_h) / (2 * h);
    }
    
    // Numerical integration using Simpson's rule
    double integrate(const std::string& expression, double a, double b, int n = 1000) {
        if (n % 2 != 0) n++;  // Ensure n is even
        
        double h = (b - a) / n;
        double sum = 0.0;
        
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
```

### 6. No Built-in Constants

**Limitation**: Mathematical constants must be manually defined.

```cpp
// ❌ Not available by default: Mathematical constants
// calculator::eval("pi * 2");  // Error: undefined variable 'pi'
// calculator::eval("e ** 2");  // Error: undefined variable 'e'
```

**Workarounds**:

```cpp
// Option 1: Define constants manually
void setup_mathematical_constants(calculator::ExpressionParser<double>& parser) {
    // Basic constants
    parser.set("pi", M_PI);
    parser.set("e", M_E);
    parser.set("sqrt2", M_SQRT2);
    parser.set("sqrt3", std::sqrt(3.0));
    parser.set("ln2", M_LN2);
    parser.set("ln10", M_LN10);
    
    // Physical constants
    parser.set("c", 299792458.0);              // Speed of light (m/s)
    parser.set("h", 6.62607015e-34);           // Planck constant (J⋅s)
    parser.set("k", 1.380649e-23);             // Boltzmann constant (J/K)
    parser.set("Na", 6.02214076e23);           // Avogadro's number (1/mol)
    parser.set("R", 8.314462618);              // Gas constant (J/(mol⋅K))
    parser.set("g", 9.80665);                  // Standard gravity (m/s²)
}

// Option 2: Create a constants library
class MathConstants {
public:
    static void setup_basic(calculator::ExpressionParser<double>& parser) {
        parser.set("pi", M_PI);
        parser.set("e", M_E);
        parser.set("phi", (1.0 + std::sqrt(5.0)) / 2.0);  // Golden ratio
    }
    
    static void setup_physics(calculator::ExpressionParser<double>& parser) {
        parser.set("c", 299792458.0);
        parser.set("h", 6.62607015e-34);
        parser.set("k_B", 1.380649e-23);
        parser.set("N_A", 6.02214076e23);
    }
    
    static void setup_all(calculator::ExpressionParser<double>& parser) {
        setup_basic(parser);
        setup_physics(parser);
    }
};
```

### 7. Thread Safety

**Limitation**: Parser instances are not thread-safe.

```cpp
// ❌ Not safe: Sharing parser between threads
calculator::ExpressionParser<double> shared_parser;

void thread_function() {
    // This is not thread-safe!
    shared_parser.set("x", 42.0);
    double result = shared_parser.eval("x * 2");
}
```

**Workarounds**:

```cpp
// Option 1: Thread-local parsers
thread_local calculator::ExpressionParser<double> local_parser;

void safe_thread_function() {
    // Each thread has its own parser
    local_parser.set("x", 42.0);
    double result = local_parser.eval("x * 2");
}

// Option 2: Parser per thread
class ThreadSafeCalculator {
private:
    std::mutex mutex_;
    calculator::ExpressionParser<double> parser_;
    
public:
    double eval(const std::string& expression) {
        std::lock_guard<std::mutex> lock(mutex_);
        return parser_.eval(expression);
    }
    
    void set_variable(const std::string& name, double value) {
        std::lock_guard<std::mutex> lock(mutex_);
        parser_.set(name, value);
    }
};

// Option 3: Factory pattern for thread safety
class CalculatorFactory {
public:
    static calculator::ExpressionParser<double> create_parser() {
        calculator::ExpressionParser<double> parser;
        
        // Set up common functions and constants
        parser.set("pi", M_PI);
        parser.set("e", M_E);
        parser.set("sin", [](double x) { return std::sin(x); });
        parser.set("cos", [](double x) { return std::cos(x); });
        parser.set("sqrt", [](double x) { return std::sqrt(x); });
        
        return parser;
    }
};

void thread_safe_usage() {
    // Each thread creates its own parser
    auto parser = CalculatorFactory::create_parser();
    parser.set("x", 42.0);
    double result = parser.eval("x * 2");
}
```

## Performance Limitations

### 1. Expression Parsing Overhead

**Limitation**: Each evaluation parses the expression from scratch.

```cpp
// ❌ Inefficient: Repeated parsing
for (int i = 0; i < 1000000; ++i) {
    double result = calculator::eval("sin(x) + cos(x)");  // Parses every time
}
```

**Workarounds**:

```cpp
// Option 1: Minimize expression complexity
calculator::ExpressionParser<double> parser;
parser.set("sin", [](double x) { return std::sin(x); });
parser.set("cos", [](double x) { return std::cos(x); });

// Pre-calculate constant parts
parser.set("constant_part", parser.eval("pi / 4"));

for (int i = 0; i < 1000000; ++i) {
    parser.set("x", i * 0.001);
    double result = parser.eval("sin(x) + cos(x)");
}

// Option 2: Cache results when possible
std::unordered_map<std::string, double> expression_cache;

double cached_eval(const std::string& expr) {
    auto it = expression_cache.find(expr);
    if (it != expression_cache.end()) {
        return it->second;
    }
    
    double result = calculator::eval(expr);
    expression_cache[expr] = result;
    return result;
}
```

### 2. Memory Usage

**Limitation**: Each parser instance maintains its own symbol table.

```cpp
// ❌ Memory inefficient: Many parser instances
std::vector<calculator::ExpressionParser<double>> parsers(1000);
// Each parser has its own symbol table
```

**Workarounds**:

```cpp
// Option 1: Reuse parser instances
class CalculatorPool {
private:
    std::vector<std::unique_ptr<calculator::ExpressionParser<double>>> pool_;
    std::mutex mutex_;
    
public:
    CalculatorPool(size_t size) {
        for (size_t i = 0; i < size; ++i) {
            auto parser = std::make_unique<calculator::ExpressionParser<double>>();
            setup_parser(*parser);
            pool_.push_back(std::move(parser));
        }
    }
    
    void setup_parser(calculator::ExpressionParser<double>& parser) {
        parser.set("pi", M_PI);
        parser.set("e", M_E);
        // ... other common setup
    }
    
    double eval(const std::string& expression) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!pool_.empty()) {
            auto parser = std::move(pool_.back());
            pool_.pop_back();
            
            double result = parser->eval(expression);
            
            pool_.push_back(std::move(parser));
            return result;
        }
        
        // Fallback: create temporary parser
        calculator::ExpressionParser<double> temp_parser;
        setup_parser(temp_parser);
        return temp_parser.eval(expression);
    }
};
```

## Planned Improvements

### Future Versions

The following limitations are planned to be addressed in future versions:

1. **Multi-parameter functions** (v2.0)
2. **Variable removal/scoping** (v1.5)
3. **Complex number support** (v2.0)
4. **Basic matrix operations** (v2.5)
5. **Expression compilation/caching** (v1.5)
6. **Thread-safe parser option** (v1.5)
7. **Built-in mathematical constants** (v1.2)

### Workaround Libraries

Consider these external libraries for advanced mathematical operations:

- **Eigen**: Matrix operations and linear algebra
- **Boost.Math**: Advanced mathematical functions
- **SymEngine**: Symbolic mathematics
- **MPFR**: Arbitrary precision arithmetic

## Best Practices

### 1. Design Around Limitations

```cpp
// Good: Design with limitations in mind
class ScientificCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    ScientificCalculator() {
        setup_all_functions();
        setup_all_constants();
    }
    
    // Provide high-level interface that hides limitations
    double distance_3d(double x1, double y1, double z1, double x2, double y2, double z2);
    double complex_magnitude(double real, double imag);
    std::vector<double> solve_quadratic(double a, double b, double c);
};
```

### 2. Use Composition

```cpp
// Good: Combine calculator with other libraries
class AdvancedMathSystem {
private:
    calculator::ExpressionParser<double> calc_;
    // Add other specialized libraries as needed
    
public:
    // Use calculator for expressions, other tools for specialized tasks
    double evaluate_expression(const std::string& expr);
    ComplexNumber evaluate_complex(const std::string& expr);
    Matrix evaluate_matrix(const std::string& expr);
};
```

### 3. Document Limitations

```cpp
/**
 * @brief Evaluates mathematical expressions with limitations
 * @note This function has the following limitations:
 *       - Single-parameter functions only
 *       - No complex numbers
 *       - No matrix operations
 *       - Not thread-safe
 * @param expression The mathematical expression to evaluate
 * @return The result of the evaluation
 */
double safe_eval(const std::string& expression);
```

## See Also

- [Operators Reference](operators.md) - Complete operator documentation
- [Numeric Types](numeric-types.md) - Type-specific behaviors and limitations
- [Advanced Features](../user-guide/advanced-features.md) - Workarounds and optimization techniques
- [API Reference](../api-reference/) - Complete technical documentation
