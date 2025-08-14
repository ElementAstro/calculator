# Reference

This section provides comprehensive reference documentation for the Modern C++ Calculator library, including detailed operator specifications, type behaviors, and current limitations.

## Quick Reference

### Supported Operations

| Category | Operators | Types | Example |
|----------|-----------|-------|---------|
| **Arithmetic** | `+`, `-`, `*`, `/`, `%` | All | `3 + 4 * 2` |
| **Bitwise** | `\|`, `^`, `&`, `<<`, `>>`, `~` | `int` only | `5 & 3` |
| **Exponentiation** | `**`, `e`/`E` | All | `2 ** 3`, `1.5e2` |
| **Unary** | `+`, `-`, `~` | All/`int` | `+5`, `-3`, `~7` |
| **Grouping** | `(`, `)` | All | `(2 + 3) * 4` |

### Numeric Types

| Type | Size | Range | Precision | Bitwise | Notes |
|------|------|-------|-----------|---------|-------|
| `int` | 32-bit | ±2.1B | Exact | ✅ | All operations supported |
| `float` | 32-bit | ±3.4E±38 | ~7 digits | ❌ | Single precision |
| `double` | 64-bit | ±1.7E±308 | ~15 digits | ❌ | Default type |

### Operator Precedence

| Level | Operators | Associativity | Description |
|-------|-----------|---------------|-------------|
| 1 | `~`, `+` (unary), `-` (unary) | Right | Unary operators |
| 2 | `**` | Right | Exponentiation |
| 3 | `*`, `/`, `%` | Left | Multiplicative |
| 4 | `+`, `-` | Left | Additive |
| 5 | `<<`, `>>` | Left | Bitwise shift |
| 6 | `&` | Left | Bitwise AND |
| 7 | `^` | Left | Bitwise XOR |
| 8 | `\|` | Left | Bitwise OR |

## Documentation Sections

### 📋 Core Reference

- **[Operators](operators.md)** - Complete operator reference with precedence and examples
- **[Numeric Types](numeric-types.md)** - Detailed type behaviors and conversions
- **[Limitations](limitations.md)** - Current limitations and workarounds

### 🎯 Quick Lookups

#### Expression Syntax

```cpp
// Basic syntax
expression := term (('+' | '-') term)*
term := factor (('*' | '/' | '%') factor)*
factor := base ('**' base)*
base := number | variable | function_call | '(' expression ')' | unary_op base

// Examples
"2 + 3 * 4"           // Valid: 14
"(2 + 3) * 4"         // Valid: 20
"2 ** 3 ** 2"         // Valid: 512 (right associative)
"sin(pi/2)"           // Valid: requires function definition
```

#### Variable Names

```cpp
// Valid variable names
"x", "y", "z"         // Single letters
"var1", "var2"        // Alphanumeric
"pi", "e"             // Mathematical constants
"base_value"          // Underscores allowed
"camelCase"           // Camel case allowed

// Invalid variable names
"2var"                // Cannot start with digit
"var-name"            // Hyphens not allowed
"var name"            // Spaces not allowed
```

#### Function Names

```cpp
// Valid function names (same rules as variables)
"sin", "cos", "tan"   // Trigonometric
"sqrt", "log", "exp"  // Mathematical
"abs", "floor", "ceil" // Utility
"custom_func"         // User-defined

// Function call syntax
"function_name(argument)"
"sin(pi/2)"           // Single argument
"max(a)"              // Variable argument
"square(5 + 3)"       // Expression argument
```

## Error Reference

### Common Error Types

| Error Type | Cause | Example | Solution |
|------------|-------|---------|----------|
| **Syntax Error** | Invalid expression syntax | `"2 + + 3"` | Fix operator sequence |
| **Division by Zero** | Division or modulo by zero | `"5 / 0"` | Check denominator |
| **Undefined Variable** | Variable not set | `"x + 5"` | Define variable first |
| **Undefined Function** | Function not set | `"sqrt(16)"` | Define function first |
| **Type Error** | Type mismatch | `eval<int>("3.14")` | Use correct type |
| **Bitwise Error** | Bitwise op on float | `eval<double>("5.5 & 3")` | Use integer type |

### Error Message Format

```
Error: [description] at position [pos] in expression '[expr]'
```

**Examples:**

```
Error: Unexpected token '+' at position 3 in expression '2 + + 3'
Error: Division by zero at position 4 in expression '5 / 0'
Error: Undefined variable 'x' at position 0 in expression 'x + 5'
```

## Performance Reference

### Complexity

| Operation | Time Complexity | Space Complexity | Notes |
|-----------|----------------|------------------|-------|
| Simple expression | O(n) | O(d) | n = length, d = depth |
| Variable lookup | O(1) | O(1) | Hash table based |
| Function call | O(1) + f(x) | O(1) | Plus function cost |
| Parser creation | O(1) | O(1) | Lightweight |

### Memory Usage

| Component | Memory Usage | Notes |
|-----------|--------------|-------|
| Parser instance | ~1KB | Base overhead |
| Variable | ~32 bytes | Name + value |
| Function | ~64 bytes | Name + function object |
| Expression parsing | O(depth) | Stack usage |

### Optimization Tips

1. **Reuse parser instances** for multiple evaluations
2. **Use appropriate types** (`int` vs `double`)
3. **Cache complex expressions** when possible
4. **Minimize variable/function redefinition**
5. **Use const references** for string parameters

## Compatibility Reference

### Compiler Support

| Compiler | Minimum | Recommended | Notes |
|----------|---------|-------------|-------|
| **GCC** | 8.0 | 11.0+ | Full C++17 support |
| **Clang** | 8.0 | 13.0+ | Full C++17 support |
| **MSVC** | 19.0 (VS 2015) | 19.29+ (VS 2019) | C++17 mode required |

### Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| **Linux** | ✅ Full | All distributions |
| **Windows** | ✅ Full | MinGW, MSVC |
| **macOS** | ✅ Full | Xcode, Homebrew |
| **FreeBSD** | ✅ Full | Ports collection |

### Standard Library

- **Minimum**: C++17 standard library
- **Dependencies**: None (header-only)
- **Features used**: `<string_view>`, `<functional>`, `<stack>`, `<unordered_map>`

## API Stability

### Version Policy

The library follows [Semantic Versioning](https://semver.org/):

- **MAJOR**: Breaking API changes
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes, no API changes

### Current Guarantees

| Component | Stability | Notes |
|-----------|-----------|-------|
| **Core API** | Stable | `ExpressionParser`, `eval` functions |
| **Exception types** | Stable | `calculator::error` |
| **Operator precedence** | Stable | Mathematical standard |
| **Type behavior** | Stable | Consistent across versions |

### Deprecation Policy

- **Deprecation notice**: 1 minor version
- **Removal**: Next major version
- **Migration guide**: Provided for breaking changes

## Standards Compliance

### Mathematical Standards

- **Operator precedence**: Follows standard mathematical conventions
- **Associativity**: Left-to-right except for exponentiation (right-to-left)
- **Type promotion**: Follows C++ arithmetic conversion rules
- **IEEE 754**: Floating-point arithmetic compliance

### C++ Standards

- **C++17**: Minimum required standard
- **C++20**: Recommended for optimal performance
- **Concepts**: Not used (for C++17 compatibility)
- **Modules**: Not used (header-only design)

## Migration Guides

### From Version 0.x to 1.x

No breaking changes - fully backward compatible.

### Best Practices Evolution

| Version | Recommendation | Reason |
|---------|----------------|--------|
| 0.x | Use `eval()` for simple cases | Limited functionality |
| 1.x | Use `ExpressionParser` for complex cases | Variables and functions |
| Future | Consider compile-time evaluation | Performance optimization |

## See Also

- **[Operators](operators.md)** - Detailed operator documentation
- **[Numeric Types](numeric-types.md)** - Type-specific behaviors
- **[Limitations](limitations.md)** - Current constraints and workarounds
- **[API Reference](../api-reference/)** - Complete API documentation
- **[Examples](../examples/)** - Practical usage examples
