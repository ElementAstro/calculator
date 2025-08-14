# Numeric Types

This page provides comprehensive documentation for the numeric types supported by the Modern C++ Calculator library, including their behaviors, limitations, and best practices for usage.

## Supported Types

The calculator library supports three fundamental numeric types, each with distinct characteristics and use cases.

### Overview Table

| Type | Size | Range | Precision | Bitwise Ops | Modulo | Scientific Notation |
|------|------|-------|-----------|-------------|--------|-------------------|
| `int` | 32-bit | ±2.1B | Exact | ✅ | ✅ | ✅ |
| `float` | 32-bit | ±3.4E±38 | ~7 digits | ❌ | ❌ | ✅ |
| `double` | 64-bit | ±1.7E±308 | ~15 digits | ❌ | ❌ | ✅ |

## Integer Type (`int`)

### Characteristics

- **Size**: 32-bit signed integer
- **Range**: -2,147,483,648 to 2,147,483,647
- **Precision**: Exact (no rounding errors)
- **Operations**: All operations supported

### Usage Examples

```cpp
#include "calculator.hpp"
#include <iostream>

void demonstrate_int_type() {
    // Basic arithmetic
    int result1 = calculator::eval<int>("100 + 50");        // 150
    int result2 = calculator::eval<int>("100 - 50");        // 50
    int result3 = calculator::eval<int>("100 * 50");        // 5000
    int result4 = calculator::eval<int>("100 / 50");        // 2
    
    // Integer division (truncation)
    int result5 = calculator::eval<int>("7 / 3");           // 2 (not 2.33...)
    int result6 = calculator::eval<int>("-7 / 3");          // -2
    int result7 = calculator::eval<int>("7 / -3");          // -2
    
    // Modulo operation
    int result8 = calculator::eval<int>("7 % 3");           // 1
    int result9 = calculator::eval<int>("-7 % 3");          // -1
    int result10 = calculator::eval<int>("7 % -3");         // 1
    
    // Bitwise operations
    int result11 = calculator::eval<int>("5 | 3");          // 7 (101 | 011 = 111)
    int result12 = calculator::eval<int>("5 & 3");          // 1 (101 & 011 = 001)
    int result13 = calculator::eval<int>("5 ^ 3");          // 6 (101 ^ 011 = 110)
    int result14 = calculator::eval<int>("8 >> 1");         // 4 (right shift)
    int result15 = calculator::eval<int>("4 << 1");         // 8 (left shift)
    int result16 = calculator::eval<int>("~5");             // -6 (bitwise NOT)
    
    // Exponentiation
    int result17 = calculator::eval<int>("2 ** 3");         // 8
    int result18 = calculator::eval<int>("5 ** 2");         // 25
    
    // Scientific notation
    int result19 = calculator::eval<int>("1e2");            // 100
    int result20 = calculator::eval<int>("2E1");            // 20
    
    std::cout << "Integer operations completed successfully" << std::endl;
}
```

### Integer Division Behavior

Integer division in the calculator follows C++ truncation rules:

```cpp
void demonstrate_integer_division() {
    // Positive numbers
    std::cout << "7 / 3 = " << calculator::eval<int>("7 / 3") << std::endl;     // 2
    std::cout << "8 / 3 = " << calculator::eval<int>("8 / 3") << std::endl;     // 2
    std::cout << "9 / 3 = " << calculator::eval<int>("9 / 3") << std::endl;     // 3
    
    // Negative numbers (truncation towards zero)
    std::cout << "-7 / 3 = " << calculator::eval<int>("-7 / 3") << std::endl;   // -2
    std::cout << "7 / -3 = " << calculator::eval<int>("7 / -3") << std::endl;   // -2
    std::cout << "-7 / -3 = " << calculator::eval<int>("-7 / -3") << std::endl; // 2
    
    // Edge cases
    std::cout << "0 / 5 = " << calculator::eval<int>("0 / 5") << std::endl;     // 0
    std::cout << "5 / 1 = " << calculator::eval<int>("5 / 1") << std::endl;     // 5
}
```

### Bitwise Operations Details

```cpp
void demonstrate_bitwise_operations() {
    // Bitwise OR (|)
    std::cout << "5 | 3 = " << calculator::eval<int>("5 | 3") << std::endl;     // 7
    // Binary: 101 | 011 = 111 (5 | 3 = 7)
    
    // Bitwise AND (&)
    std::cout << "5 & 3 = " << calculator::eval<int>("5 & 3") << std::endl;     // 1
    // Binary: 101 & 011 = 001 (5 & 3 = 1)
    
    // Bitwise XOR (^)
    std::cout << "5 ^ 3 = " << calculator::eval<int>("5 ^ 3") << std::endl;     // 6
    // Binary: 101 ^ 011 = 110 (5 ^ 3 = 6)
    
    // Left shift (<<) - multiply by 2^n
    std::cout << "1 << 3 = " << calculator::eval<int>("1 << 3") << std::endl;   // 8
    std::cout << "5 << 2 = " << calculator::eval<int>("5 << 2") << std::endl;   // 20
    
    // Right shift (>>) - divide by 2^n (truncated)
    std::cout << "16 >> 2 = " << calculator::eval<int>("16 >> 2") << std::endl; // 4
    std::cout << "7 >> 1 = " << calculator::eval<int>("7 >> 1") << std::endl;   // 3
    
    // Bitwise NOT (~) - two's complement
    std::cout << "~5 = " << calculator::eval<int>("~5") << std::endl;           // -6
    std::cout << "~0 = " << calculator::eval<int>("~0") << std::endl;           // -1
    std::cout << "~(-1) = " << calculator::eval<int>("~(-1)") << std::endl;     // 0
}
```

### Integer Overflow

```cpp
void demonstrate_integer_overflow() {
    // Values within range
    int result1 = calculator::eval<int>("2000000000");      // OK
    int result2 = calculator::eval<int>("-2000000000");     // OK
    
    // Values near limits
    try {
        // This may cause overflow depending on the implementation
        int result3 = calculator::eval<int>("2147483647 + 1");
        std::cout << "Result: " << result3 << std::endl;
    } catch (const calculator::error& e) {
        std::cout << "Overflow error: " << e.what() << std::endl;
    }
}
```

## Float Type (`float`)

### Characteristics

- **Size**: 32-bit IEEE 754 single-precision
- **Range**: ±3.4E±38 (approximately)
- **Precision**: ~7 decimal digits
- **Operations**: Arithmetic and exponentiation only

### Usage Examples

```cpp
void demonstrate_float_type() {
    // Basic arithmetic
    float result1 = calculator::eval<float>("10.5 + 5.3");     // 15.8
    float result2 = calculator::eval<float>("10.7 - 5.2");     // 5.5
    float result3 = calculator::eval<float>("3.14 * 2.0");     // 6.28
    float result4 = calculator::eval<float>("10.0 / 3.0");     // 3.33333...
    
    // Exponentiation
    float result5 = calculator::eval<float>("2.5 ** 2");       // 6.25
    float result6 = calculator::eval<float>("8.0 ** (1.0/3.0)"); // 2.0 (cube root)
    
    // Scientific notation
    float result7 = calculator::eval<float>("1.5e2");          // 150.0f
    float result8 = calculator::eval<float>("2.5E-3");         // 0.0025f
    
    // Precision demonstration
    float result9 = calculator::eval<float>("1.0 / 3.0");      // 0.333333f (limited precision)
    
    std::cout << "Float operations completed successfully" << std::endl;
}
```

### Float Precision Limitations

```cpp
void demonstrate_float_precision() {
    // Precision comparison
    float float_result = calculator::eval<float>("1.0 / 3.0");
    double double_result = calculator::eval<double>("1.0 / 3.0");
    
    std::cout << std::setprecision(10);
    std::cout << "Float precision:  " << float_result << std::endl;   // ~0.3333333433
    std::cout << "Double precision: " << double_result << std::endl;  // ~0.3333333333
    
    // Large number precision loss
    float large1 = calculator::eval<float>("1234567.0 + 1.0");
    float large2 = calculator::eval<float>("1234567.0");
    
    if (large1 == large2) {
        std::cout << "Precision lost: 1234567.0 + 1.0 == 1234567.0" << std::endl;
    }
}
```

### Float Limitations

```cpp
void demonstrate_float_limitations() {
    // Bitwise operations not supported
    try {
        calculator::eval<float>("5.5 & 3.2");  // Error!
    } catch (const calculator::error& e) {
        std::cout << "Bitwise error: " << e.what() << std::endl;
    }
    
    // Modulo not supported
    try {
        calculator::eval<float>("7.5 % 2.3");  // Error!
    } catch (const calculator::error& e) {
        std::cout << "Modulo error: " << e.what() << std::endl;
    }
}
```

## Double Type (`double`)

### Characteristics

- **Size**: 64-bit IEEE 754 double-precision
- **Range**: ±1.7E±308 (approximately)
- **Precision**: ~15 decimal digits
- **Operations**: Arithmetic and exponentiation only
- **Default**: Used by `calculator::eval()` without template parameter

### Usage Examples

```cpp
void demonstrate_double_type() {
    // Basic arithmetic (default type)
    double result1 = calculator::eval("10.5 + 5.3");           // 15.8
    double result2 = calculator::eval("10.7 - 5.2");           // 5.5
    double result3 = calculator::eval("3.141592653589793 * 2"); // 6.283185307179586
    double result4 = calculator::eval("10.0 / 3.0");           // 3.3333333333333335
    
    // Explicit double type
    double result5 = calculator::eval<double>("2.5 ** 2");     // 6.25
    double result6 = calculator::eval<double>("1.0 / 3.0");    // High precision
    
    // Scientific notation
    double result7 = calculator::eval("1.5e2");                // 150.0
    double result8 = calculator::eval("6.02214076e23");        // Avogadro's number
    double result9 = calculator::eval("1.380649e-23");         // Boltzmann constant
    
    // Very large and small numbers
    double result10 = calculator::eval("1.7e308");             // Near maximum
    double result11 = calculator::eval("2.2e-308");            // Near minimum
    
    std::cout << "Double operations completed successfully" << std::endl;
}
```

### Double Precision Advantages

```cpp
void demonstrate_double_precision() {
    // High precision calculations
    double pi_approx = calculator::eval("22.0 / 7.0");
    double pi_better = calculator::eval("3.141592653589793");
    
    std::cout << std::setprecision(15);
    std::cout << "22/7 approximation: " << pi_approx << std::endl;
    std::cout << "Better π value:     " << pi_better << std::endl;
    
    // Financial calculations requiring precision
    double compound_interest = calculator::eval("1000.0 * (1.05 ** 30)");
    std::cout << "Compound interest:  " << compound_interest << std::endl;
    
    // Scientific calculations
    double planck_energy = calculator::eval("6.62607015e-34 * 3e8 / 500e-9");
    std::cout << "Photon energy:      " << planck_energy << " J" << std::endl;
}
```

## Type Conversion and Compatibility

### Implicit Conversions

The calculator doesn't perform implicit type conversions. You must specify the desired type explicitly:

```cpp
void demonstrate_type_conversion() {
    // Different types for same expression
    int int_result = calculator::eval<int>("10 / 3");           // 3
    float float_result = calculator::eval<float>("10.0 / 3.0"); // 3.33333f
    double double_result = calculator::eval("10.0 / 3.0");      // 3.33333... (default)
    
    std::cout << "Int result:    " << int_result << std::endl;
    std::cout << "Float result:  " << float_result << std::endl;
    std::cout << "Double result: " << double_result << std::endl;
}
```

### Type Validation

```cpp
void demonstrate_type_validation() {
    // Valid: Integer literal in integer context
    int result1 = calculator::eval<int>("42");                  // OK
    
    // Valid: Float literal in float context
    float result2 = calculator::eval<float>("3.14");           // OK
    
    // Error: Float literal in integer context
    try {
        int result3 = calculator::eval<int>("3.14");           // Error!
    } catch (const calculator::error& e) {
        std::cout << "Type error: " << e.what() << std::endl;
    }
    
    // Error: Decimal in integer expression
    try {
        int result4 = calculator::eval<int>("10.5 + 5");       // Error!
    } catch (const calculator::error& e) {
        std::cout << "Type error: " << e.what() << std::endl;
    }
}
```

## Performance Characteristics

### Operation Speed Comparison

| Type | Arithmetic | Bitwise | Exponentiation | Memory Usage |
|------|------------|---------|----------------|--------------|
| `int` | Fastest | Fastest | Fast | 4 bytes |
| `float` | Fast | N/A | Moderate | 4 bytes |
| `double` | Moderate | N/A | Slower | 8 bytes |

### Benchmark Example

```cpp
#include <chrono>

void benchmark_types() {
    const int iterations = 1000000;
    
    // Integer benchmark
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        calculator::eval<int>("123 + 456");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto int_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Double benchmark
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        calculator::eval<double>("123.0 + 456.0");
    }
    end = std::chrono::high_resolution_clock::now();
    auto double_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Integer operations: " << int_time.count() << " μs" << std::endl;
    std::cout << "Double operations:  " << double_time.count() << " μs" << std::endl;
    std::cout << "Ratio: " << (double)double_time.count() / int_time.count() << std::endl;
}
```

## Best Practices

### 1. Choose Appropriate Types

```cpp
// Good: Use int for counting, indexing
int count = calculator::eval<int>("items + new_items");
int index = calculator::eval<int>("start_pos + offset");

// Good: Use double for scientific calculations
double energy = calculator::eval("mass * c ** 2");
double probability = calculator::eval("successes / total_trials");

// Good: Use float when memory is constrained and precision is sufficient
float graphics_coord = calculator::eval<float>("screen_width * 0.5");
```

### 2. Handle Precision Requirements

```cpp
// Financial calculations: use double for precision
double money = calculator::eval("principal * (1 + rate) ** years");

// Graphics/games: float often sufficient
float position = calculator::eval<float>("velocity * time");

// Counters/flags: use int
int flags = calculator::eval<int>("flag1 | flag2 | flag3");
```

### 3. Avoid Type Mismatches

```cpp
// Bad: Mixing types inappropriately
// calculator::eval<int>("3.14159");  // Error!

// Good: Use consistent types
double pi = calculator::eval<double>("3.14159");
int whole_pi = calculator::eval<int>("3");
```

### 4. Consider Range Limitations

```cpp
// Check for potential overflow
if (value1 > 1000000 && value2 > 1000000) {
    // Use double to avoid int overflow
    double result = calculator::eval<double>(expression);
} else {
    int result = calculator::eval<int>(expression);
}
```

## Common Pitfalls

### 1. Integer Division Truncation

```cpp
// Problem: Unexpected truncation
int result = calculator::eval<int>("1 / 2");  // 0, not 0.5

// Solution: Use floating-point
double result = calculator::eval<double>("1.0 / 2.0");  // 0.5
```

### 2. Floating-Point Precision

```cpp
// Problem: Precision loss
float result = calculator::eval<float>("0.1 + 0.2");  // Not exactly 0.3

// Solution: Use double or handle precision explicitly
double result = calculator::eval<double>("0.1 + 0.2");
```

### 3. Type-Specific Operations

```cpp
// Problem: Using bitwise ops on floating-point
// calculator::eval<double>("5.5 & 3.2");  // Error!

// Solution: Use appropriate type
int result = calculator::eval<int>("5 & 3");  // OK
```

## See Also

- [Operators Reference](operators.md) - Complete operator documentation
- [Limitations](limitations.md) - Current constraints and workarounds
- [Basic Usage](../user-guide/basic-usage.md) - Practical type usage examples
- [API Reference](../api-reference/) - Complete technical documentation
