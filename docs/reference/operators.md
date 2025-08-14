# Operators Reference

This page provides a comprehensive reference for all operators supported by the Modern C++ Calculator library, including precedence rules, associativity, and type-specific behaviors.

## Operator Categories

### Arithmetic Operators

Basic mathematical operations supported by all numeric types.

| Operator | Name | Example | Result | Types |
|----------|------|---------|--------|-------|
| `+` | Addition | `5 + 3` | `8` | All |
| `-` | Subtraction | `5 - 3` | `2` | All |
| `*` | Multiplication | `5 * 3` | `15` | All |
| `/` | Division | `6 / 3` | `2` | All |
| `%` | Modulo | `7 % 3` | `1` | `int` only |

#### Division Behavior

```cpp
// Integer division (truncates towards zero)
int result1 = calculator::eval<int>("7 / 3");     // 2
int result2 = calculator::eval<int>("-7 / 3");    // -2

// Floating-point division
double result3 = calculator::eval<double>("7.0 / 3.0");   // 2.33333...
float result4 = calculator::eval<float>("7.0 / 3.0");     // 2.33333f
```

#### Modulo Behavior

```cpp
// Modulo is only supported for integer types
int result1 = calculator::eval<int>("7 % 3");     // 1
int result2 = calculator::eval<int>("-7 % 3");    // -1
int result3 = calculator::eval<int>("7 % -3");    // 1

// Error: Modulo not supported for floating-point
// calculator::eval<double>("7.5 % 3.2");  // Throws error
```

### Exponentiation Operators

Power operations and scientific notation.

| Operator | Name | Example | Result | Types |
|----------|------|---------|--------|-------|
| `**` | Power | `2 ** 3` | `8` | All |
| `e`, `E` | Scientific notation | `1.5e2` | `150` | All |

#### Power Operator Examples

```cpp
// Basic exponentiation
double result1 = calculator::eval("2 ** 3");        // 8.0
double result2 = calculator::eval("5 ** 2");        // 25.0
double result3 = calculator::eval("2 ** -1");       // 0.5

// Fractional exponents
double result4 = calculator::eval("8 ** (1.0/3.0)"); // 2.0 (cube root)
double result5 = calculator::eval("16 ** 0.5");      // 4.0 (square root)

// Right associativity
double result6 = calculator::eval("2 ** 3 ** 2");    // 512 (2 ** (3 ** 2))
double result7 = calculator::eval("(2 ** 3) ** 2");  // 64
```

#### Scientific Notation Examples

```cpp
// Scientific notation
double result1 = calculator::eval("1.5e2");     // 150.0
double result2 = calculator::eval("2.5E-3");    // 0.0025
double result3 = calculator::eval("6.02e23");   // 6.02e+23
double result4 = calculator::eval("1e-6");      // 0.000001

// Negative exponents
double result5 = calculator::eval("1.5e-2");    // 0.015
double result6 = calculator::eval("3E-4");      // 0.0003
```

### Bitwise Operators

Bitwise operations are only supported for integer types.

| Operator | Name | Example | Result | Types |
|----------|------|---------|--------|-------|
| `\|` | Bitwise OR | `5 \| 3` | `7` | `int` only |
| `^` | Bitwise XOR | `5 ^ 3` | `6` | `int` only |
| `&` | Bitwise AND | `5 & 3` | `1` | `int` only |
| `<<` | Left shift | `4 << 1` | `8` | `int` only |
| `>>` | Right shift | `8 >> 1` | `4` | `int` only |

#### Bitwise Operation Examples

```cpp
// Bitwise OR
int result1 = calculator::eval<int>("5 | 3");    // 7 (101 | 011 = 111)
int result2 = calculator::eval<int>("8 | 4");    // 12 (1000 | 0100 = 1100)

// Bitwise XOR
int result3 = calculator::eval<int>("5 ^ 3");    // 6 (101 ^ 011 = 110)
int result4 = calculator::eval<int>("15 ^ 15");  // 0 (same values = 0)

// Bitwise AND
int result5 = calculator::eval<int>("5 & 3");    // 1 (101 & 011 = 001)
int result6 = calculator::eval<int>("12 & 8");   // 8 (1100 & 1000 = 1000)

// Left shift (multiply by 2^n)
int result7 = calculator::eval<int>("1 << 3");   // 8 (1 * 2^3)
int result8 = calculator::eval<int>("5 << 2");   // 20 (5 * 2^2)

// Right shift (divide by 2^n)
int result9 = calculator::eval<int>("16 >> 2");  // 4 (16 / 2^2)
int result10 = calculator::eval<int>("7 >> 1");  // 3 (7 / 2, truncated)
```

#### Bitwise Error Cases

```cpp
// Error: Bitwise operations not supported for floating-point types
try {
    calculator::eval<double>("5.5 & 3.2");  // Throws calculator::error
} catch (const calculator::error& e) {
    std::cout << "Error: " << e.what() << std::endl;
}

try {
    calculator::eval<float>("7.1 | 2.3");   // Throws calculator::error
} catch (const calculator::error& e) {
    std::cout << "Error: " << e.what() << std::endl;
}
```

### Unary Operators

Operators that work on a single operand.

| Operator | Name | Example | Result | Types |
|----------|------|---------|--------|-------|
| `+` | Unary plus | `+5` | `5` | All |
| `-` | Unary minus | `-5` | `-5` | All |
| `~` | Bitwise NOT | `~5` | `-6` | `int` only |

#### Unary Operator Examples

```cpp
// Unary plus (identity operation)
double result1 = calculator::eval("+5");        // 5.0
double result2 = calculator::eval("+(3 + 4)");  // 7.0

// Unary minus (negation)
double result3 = calculator::eval("-5");        // -5.0
double result4 = calculator::eval("-(3 + 4)");  // -7.0
double result5 = calculator::eval("--5");       // 5.0 (double negation)

// Bitwise NOT (integer only)
int result6 = calculator::eval<int>("~5");      // -6 (two's complement)
int result7 = calculator::eval<int>("~0");      // -1
int result8 = calculator::eval<int>("~(-1)");   // 0
```

### Grouping Operators

Parentheses for controlling order of operations.

| Operator | Name | Example | Result |
|----------|------|---------|--------|
| `(` `)` | Parentheses | `(2 + 3) * 4` | `20` |

#### Grouping Examples

```cpp
// Without parentheses (follows precedence)
double result1 = calculator::eval("2 + 3 * 4");      // 14 (3*4 first)
double result2 = calculator::eval("10 - 6 / 2");     // 7 (6/2 first)

// With parentheses (overrides precedence)
double result3 = calculator::eval("(2 + 3) * 4");    // 20
double result4 = calculator::eval("(10 - 6) / 2");   // 2

// Nested parentheses
double result5 = calculator::eval("((2 + 3) * 4) / 5");     // 4
double result6 = calculator::eval("2 * (3 + (4 * 5))");    // 46

// Complex grouping
double result7 = calculator::eval("(2 + 3) * (4 - 1)");    // 15
double result8 = calculator::eval("((1 + 2) * 3) ** 2");   // 81
```

## Operator Precedence

The calculator follows standard mathematical operator precedence rules:

| Precedence | Operators | Associativity | Description |
|------------|-----------|---------------|-------------|
| 1 (Highest) | `~`, `+` (unary), `-` (unary) | Right | Unary operators |
| 2 | `**` | Right | Exponentiation |
| 3 | `*`, `/`, `%` | Left | Multiplicative |
| 4 | `+`, `-` | Left | Additive |
| 5 | `<<`, `>>` | Left | Bitwise shift |
| 6 | `&` | Left | Bitwise AND |
| 7 | `^` | Left | Bitwise XOR |
| 8 (Lowest) | `\|` | Left | Bitwise OR |

### Precedence Examples

```cpp
// Precedence demonstration
std::cout << "2 + 3 * 4 = " << calculator::eval("2 + 3 * 4") << std::endl;           // 14
std::cout << "2 ** 3 ** 2 = " << calculator::eval("2 ** 3 ** 2") << std::endl;       // 512
std::cout << "10 - 5 + 2 = " << calculator::eval("10 - 5 + 2") << std::endl;         // 7
std::cout << "8 / 2 * 3 = " << calculator::eval("8 / 2 * 3") << std::endl;           // 12

// Bitwise precedence (integer only)
std::cout << "5 | 3 & 1 = " << calculator::eval<int>("5 | 3 & 1") << std::endl;      // 5
std::cout << "8 >> 1 + 1 = " << calculator::eval<int>("8 >> 1 + 1") << std::endl;    // 4
std::cout << "4 << 1 | 2 = " << calculator::eval<int>("4 << 1 | 2") << std::endl;    // 10
```

### Associativity Rules

#### Left Associativity

Most operators are left-associative (evaluated left to right):

```cpp
// Left associative operators
std::cout << "10 - 5 - 2 = " << calculator::eval("10 - 5 - 2") << std::endl;         // 3 ((10-5)-2)
std::cout << "12 / 3 / 2 = " << calculator::eval("12 / 3 / 2") << std::endl;         // 2 ((12/3)/2)
std::cout << "5 & 3 & 1 = " << calculator::eval<int>("5 & 3 & 1") << std::endl;      // 1 ((5&3)&1)
```

#### Right Associativity

Exponentiation and unary operators are right-associative:

```cpp
// Right associative operators
std::cout << "2 ** 3 ** 2 = " << calculator::eval("2 ** 3 ** 2") << std::endl;       // 512 (2**(3**2))
std::cout << "--5 = " << calculator::eval("--5") << std::endl;                       // 5 (-(-5))
std::cout << "~~5 = " << calculator::eval<int>("~~5") << std::endl;                  // 5 (~(~5))
```

## Type-Specific Behaviors

### Integer Type (`int`)

```cpp
// All operations supported
int result1 = calculator::eval<int>("10 + 5");      // 15
int result2 = calculator::eval<int>("10 / 3");      // 3 (truncated)
int result3 = calculator::eval<int>("10 % 3");      // 1
int result4 = calculator::eval<int>("5 & 3");       // 1
int result5 = calculator::eval<int>("5 | 3");       // 7
int result6 = calculator::eval<int>("5 ^ 3");       // 6
int result7 = calculator::eval<int>("8 >> 1");      // 4
int result8 = calculator::eval<int>("4 << 1");      // 8
int result9 = calculator::eval<int>("~5");          // -6
```

### Floating-Point Types (`float`, `double`)

```cpp
// Arithmetic and power operations only
double result1 = calculator::eval<double>("10.5 + 5.3");    // 15.8
double result2 = calculator::eval<double>("10.0 / 3.0");    // 3.33333...
double result3 = calculator::eval<double>("2.5 ** 2");      // 6.25
double result4 = calculator::eval<double>("1.5e2");         // 150.0

// Bitwise operations not supported
// calculator::eval<double>("5.5 & 3.2");  // Error!
// calculator::eval<double>("7.1 % 2.3");  // Error!
```

## Common Pitfalls

### 1. Integer Division

```cpp
// Unexpected truncation
int result1 = calculator::eval<int>("1 / 2");       // 0 (not 0.5)
int result2 = calculator::eval<int>("5 / 2");       // 2 (not 2.5)

// Solution: Use floating-point
double result3 = calculator::eval<double>("1.0 / 2.0");  // 0.5
double result4 = calculator::eval<double>("5.0 / 2.0");  // 2.5
```

### 2. Operator Precedence

```cpp
// Unexpected precedence
double result1 = calculator::eval("2 + 3 * 4");     // 14 (not 20)
double result2 = calculator::eval("2 ** 3 ** 2");   // 512 (not 64)

// Solution: Use parentheses
double result3 = calculator::eval("(2 + 3) * 4");   // 20
double result4 = calculator::eval("(2 ** 3) ** 2"); // 64
```

### 3. Type Mismatches

```cpp
// Bitwise operations on floating-point
try {
    calculator::eval<double>("5.5 & 3.2");  // Error!
} catch (const calculator::error& e) {
    // Handle error
}

// Solution: Use appropriate types
int result = calculator::eval<int>("5 & 3");  // OK
```

### 4. Modulo with Floating-Point

```cpp
// Modulo not supported for floating-point
try {
    calculator::eval<double>("7.5 % 2.3");  // Error!
} catch (const calculator::error& e) {
    // Handle error
}

// Solution: Use integer types or implement custom function
int result = calculator::eval<int>("7 % 2");  // OK
```

## Performance Considerations

### Operator Complexity

| Operator Category | Complexity | Notes |
|------------------|------------|-------|
| Arithmetic | O(1) | Fast, hardware-supported |
| Bitwise | O(1) | Very fast, single CPU instructions |
| Exponentiation | O(log n) | More expensive, especially for large exponents |
| Scientific notation | O(1) | Parsed at compile time |

### Optimization Tips

1. **Use appropriate types**: Integer operations are generally faster than floating-point
2. **Avoid unnecessary parentheses**: They don't affect performance but can make expressions harder to read
3. **Consider operator precedence**: Understanding precedence can help write more efficient expressions
4. **Use bitwise operations**: For integer calculations, bitwise operations are very fast

## See Also

- [Numeric Types](numeric-types.md) - Detailed type behaviors and conversions
- [Limitations](limitations.md) - Current constraints and workarounds
- [Basic Usage](../user-guide/basic-usage.md) - Practical operator usage examples
- [API Reference](../api-reference/) - Complete technical documentation
