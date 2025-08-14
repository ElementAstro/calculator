# ExpressionParser Class

The `ExpressionParser<T>` class is the core component of the calculator library. It provides a template-based, high-performance expression parser and evaluator that supports variables, functions, and comprehensive mathematical operations.

## Class Declaration

```cpp
template <typename T>
class ExpressionParser {
public:
    // Evaluation methods
    T eval(std::string_view expr);
    T eval(char c);
    
    // Variable management
    void set(const std::string &name, T value);
    
    // Function management
    void set(const std::string &name, std::function<T(T)> func);
    
private:
    // Internal implementation details...
};
```

## Template Parameters

| Parameter | Description | Constraints |
|-----------|-------------|-------------|
| `T` | Numeric type for calculations | Must be arithmetic type (`int`, `float`, `double`) |

### Supported Types

- **`int`**: Integer arithmetic with full bitwise operation support
- **`float`**: Single-precision floating-point arithmetic
- **`double`**: Double-precision floating-point arithmetic (recommended)

## Public Methods

### eval(std::string_view expr)

Evaluates a mathematical expression and returns the result.

```cpp
T eval(std::string_view expr);
```

**Parameters:**

- `expr`: The mathematical expression to evaluate

**Returns:**

- Result of the expression evaluation as type `T`

**Throws:**

- `calculator::error`: On syntax errors or evaluation failures

**Example:**

```cpp
calculator::ExpressionParser<double> parser;
double result = parser.eval("3 + 4 * 2");  // Returns 11.0
```

### eval(char c)

Evaluates a single character as an expression.

```cpp
T eval(char c);
```

**Parameters:**

- `c`: Single character to evaluate (typically a digit)

**Returns:**

- Numeric value of the character

**Example:**

```cpp
calculator::ExpressionParser<int> parser;
int result = parser.eval('5');  // Returns 5
```

### set(const std::string &name, T value)

Sets a variable that can be used in expressions.

```cpp
void set(const std::string &name, T value);
```

**Parameters:**

- `name`: Variable name (must be valid identifier)
- `value`: Variable value

**Example:**

```cpp
calculator::ExpressionParser<double> parser;
parser.set("x", 10.0);
parser.set("pi", 3.14159);
double result = parser.eval("x * pi");  // Returns ~31.4159
```

### set(const std::string &name, std::function<T(T)> func)

Sets a custom function that can be called in expressions.

```cpp
void set(const std::string &name, std::function<T(T)> func);
```

**Parameters:**

- `name`: Function name (must be valid identifier)
- `func`: Function object taking one parameter of type `T` and returning type `T`

**Example:**

```cpp
calculator::ExpressionParser<double> parser;
parser.set("square", [](double x) { return x * x; });
parser.set("sqrt", [](double x) { return std::sqrt(x); });
double result = parser.eval("sqrt(square(5))");  // Returns 5.0
```

## Usage Patterns

### Basic Calculator

```cpp
calculator::ExpressionParser<double> calc;

// Simple arithmetic
double result1 = calc.eval("2 + 3 * 4");        // 14.0
double result2 = calc.eval("(10 + 5) / 3");     // 5.0
double result3 = calc.eval("2 ** 3");           // 8.0
```

### Scientific Calculator

```cpp
calculator::ExpressionParser<double> sci_calc;

// Set mathematical constants
sci_calc.set("pi", 3.141592653589793);
sci_calc.set("e", 2.718281828459045);

// Set mathematical functions
sci_calc.set("sin", [](double x) { return std::sin(x); });
sci_calc.set("cos", [](double x) { return std::cos(x); });
sci_calc.set("log", [](double x) { return std::log(x); });
sci_calc.set("exp", [](double x) { return std::exp(x); });

// Complex calculations
double result1 = sci_calc.eval("sin(pi/2)");     // 1.0
double result2 = sci_calc.eval("log(e)");        // 1.0
double result3 = sci_calc.eval("exp(log(5))");   // 5.0
```

### Configuration System

```cpp
calculator::ExpressionParser<double> config;

// Load configuration values
config.set("base_rate", 0.05);
config.set("multiplier", 1.2);
config.set("bonus", 100.0);

// Define calculation formulas
std::string formula1 = "base_rate * multiplier + bonus";
std::string formula2 = "(base_rate + 0.01) * multiplier";

double result1 = config.eval(formula1);  // 106.0
double result2 = config.eval(formula2);  // 0.072
```

## Type-Specific Behavior

### Integer Type (`int`)

```cpp
calculator::ExpressionParser<int> int_calc;

// Supports all operations including bitwise
int result1 = int_calc.eval("5 + 3");      // 8
int result2 = int_calc.eval("10 / 3");     // 3 (integer division)
int result3 = int_calc.eval("7 % 3");      // 1
int result4 = int_calc.eval("5 & 3");      // 1 (bitwise AND)
int result5 = int_calc.eval("5 | 3");      // 7 (bitwise OR)
int result6 = int_calc.eval("5 ^ 3");      // 6 (bitwise XOR)
int result7 = int_calc.eval("8 >> 1");     // 4 (right shift)
int result8 = int_calc.eval("4 << 1");     // 8 (left shift)
int result9 = int_calc.eval("~5");         // -6 (bitwise NOT)
```

### Floating-Point Types (`float`, `double`)

```cpp
calculator::ExpressionParser<double> float_calc;

// Supports arithmetic and power operations (no bitwise)
double result1 = float_calc.eval("5.5 + 3.2");    // 8.7
double result2 = float_calc.eval("10.0 / 3.0");   // 3.333...
double result3 = float_calc.eval("2.5 ** 2");     // 6.25
double result4 = float_calc.eval("1.5e2");        // 150.0

// Bitwise operations not supported for floating-point
// float_calc.eval("5.5 & 3.2");  // Would throw error
```

## Error Conditions

The parser throws `calculator::error` exceptions in the following cases:

### Syntax Errors

```cpp
calculator::ExpressionParser<double> parser;

try {
    parser.eval("2 + + 3");        // Invalid operator sequence
    parser.eval("(2 + 3");         // Unmatched parenthesis
    parser.eval("2 3");            // Missing operator
    parser.eval("");              // Empty expression
} catch (const calculator::error& e) {
    std::cerr << "Syntax error: " << e.what() << std::endl;
}
```

### Runtime Errors

```cpp
calculator::ExpressionParser<double> parser;

try {
    parser.eval("5 / 0");          // Division by zero
    parser.eval("unknown_var");    // Undefined variable
    parser.eval("unknown_func(5)"); // Undefined function
} catch (const calculator::error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
}
```

### Type Errors

```cpp
calculator::ExpressionParser<int> int_parser;

try {
    int_parser.eval("3.14");       // Float literal in int parser
    int_parser.eval("1.5 + 2.5");  // Float arithmetic in int parser
} catch (const calculator::error& e) {
    std::cerr << "Type error: " << e.what() << std::endl;
}
```

## Performance Considerations

### Optimization Tips

1. **Reuse parser instances** for multiple evaluations:

   ```cpp
   calculator::ExpressionParser<double> parser;
   parser.set("x", 1.0);
   
   // Good: Reuse parser
   for (int i = 0; i < 1000; ++i) {
       parser.set("x", i);
       double result = parser.eval("x * x + 2 * x + 1");
   }
   ```

2. **Use appropriate types** for your use case:

   ```cpp
   // Use int for integer calculations
   calculator::ExpressionParser<int> int_calc;
   
   // Use double for precision
   calculator::ExpressionParser<double> precise_calc;
   ```

3. **Cache complex expressions**:

   ```cpp
   std::string complex_expr = "sin(x) * cos(y) + tan(z)";
   // Evaluate once, reuse result if inputs don't change
   ```

### Memory Usage

- **Base overhead**: ~1KB per parser instance
- **Variables**: ~32 bytes per variable
- **Functions**: ~64 bytes per function
- **Expression parsing**: Stack usage proportional to expression complexity

## Thread Safety

!!! warning "Not Thread-Safe"
    `ExpressionParser` instances are **not thread-safe**. Use separate instances per thread or provide external synchronization.

```cpp
// Good: Thread-local instances
thread_local calculator::ExpressionParser<double> parser;

// Good: Separate instances per thread
void worker_thread() {
    calculator::ExpressionParser<double> local_parser;
    // Use local_parser safely
}
```

## See Also

- [Free Functions](functions.md) - Global evaluation functions
- [Exception Handling](exceptions.md) - Error handling details
- [Examples](../examples/advanced-examples.md) - Advanced usage patterns
