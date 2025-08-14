# User Guide

Welcome to the comprehensive user guide for the Modern C++ Calculator library. This section provides detailed instructions and examples for using all features of the calculator, from basic arithmetic to advanced mathematical expressions.

## What You'll Learn

This user guide covers everything you need to know to effectively use the calculator library:

### 📚 Core Topics

- **[Basic Usage](basic-usage.md)**: Fundamental operations and expression evaluation
- **[Advanced Features](advanced-features.md)**: Complex expressions, operator precedence, and optimization
- **[Variables & Functions](variables-functions.md)**: Working with variables and custom functions
- **[Error Handling](error-handling.md)**: Robust error management and debugging

### 🎯 Learning Path

We recommend following this learning path:

1. **Start with [Basic Usage](basic-usage.md)** - Learn the fundamentals
2. **Explore [Variables & Functions](variables-functions.md)** - Add dynamic capabilities
3. **Master [Advanced Features](advanced-features.md)** - Handle complex scenarios
4. **Implement [Error Handling](error-handling.md)** - Build robust applications

## Quick Reference

### Essential Classes

| Class | Purpose | Usage |
|-------|---------|-------|
| `ExpressionParser<T>` | Main calculator engine | Template-based expression evaluation |
| `calculator::error` | Exception handling | Thrown on syntax or evaluation errors |

### Key Functions

| Function | Return Type | Description |
|----------|-------------|-------------|
| `eval(expression)` | `double` | Evaluate expression as double |
| `eval<T>(expression)` | `T` | Evaluate expression as type T |
| `parser.set(name, value)` | `void` | Set variable value |
| `parser.set(name, function)` | `void` | Set custom function |

### Supported Operations

| Category | Operators | Example |
|----------|-----------|---------|
| Arithmetic | `+`, `-`, `*`, `/`, `%` | `3 + 4 * 2` |
| Bitwise | `\|`, `^`, `&`, `<<`, `>>` | `5 & 3` |
| Exponentiation | `**`, `e`/`E` | `2 ** 3`, `1.5e2` |
| Unary | `+`, `-`, `~` | `-5`, `~7` |

## Common Use Cases

### Scientific Computing

```cpp
calculator::ExpressionParser<double> parser;
parser.set("pi", 3.14159265359);
parser.set("sin", [](double x) { return std::sin(x); });
double result = parser.eval("sin(pi/2)");  // 1.0
```

### Configuration Systems

```cpp
// Parse user-defined formulas
std::string user_formula = "base_value * multiplier + offset";
parser.set("base_value", 100.0);
parser.set("multiplier", 1.5);
parser.set("offset", 25.0);
double config_result = parser.eval(user_formula);  // 175.0
```

### Game Development

```cpp
// Damage calculation system
parser.set("base_damage", 50.0);
parser.set("strength", 15.0);
parser.set("weapon_modifier", 1.2);
double damage = parser.eval("(base_damage + strength * 2) * weapon_modifier");
```

## Best Practices

### Performance Tips

1. **Reuse parser instances** for multiple evaluations with the same variables/functions
2. **Use appropriate numeric types** (`int` for integers, `double` for precision)
3. **Cache complex expressions** when evaluating repeatedly
4. **Prefer template functions** for type safety

### Error Prevention

1. **Always wrap evaluations in try-catch blocks**
2. **Validate user input** before evaluation
3. **Use meaningful variable names** to avoid confusion
4. **Test expressions** with known values first

### Code Organization

1. **Separate expression logic** from business logic
2. **Create helper functions** for common mathematical operations
3. **Document complex expressions** with comments
4. **Use constants** for mathematical values like π, e

## Examples by Domain

### Mathematics & Science

- Trigonometric calculations
- Statistical formulas
- Physics equations
- Engineering calculations

### Business & Finance

- Financial formulas
- Tax calculations
- Interest computations
- Profit/loss analysis

### Gaming & Graphics

- Damage calculations
- Physics simulations
- Animation curves
- Procedural generation

### Data Processing

- Custom metrics
- Aggregation formulas
- Transformation functions
- Validation rules

## Getting Help

If you need assistance:

1. **Check the [Examples](../examples/)** for practical use cases
2. **Review the [API Reference](../api-reference/)** for detailed documentation
3. **Browse the [Reference](../reference/)** for operator precedence and limitations
4. **Visit our [GitHub Issues](https://github.com/ElementAstro/calculator/issues)** for community support

## What's Next?

Ready to dive deeper? Choose your path:

- **New to the library?** Start with [Basic Usage](basic-usage.md)
- **Need dynamic expressions?** Jump to [Variables & Functions](variables-functions.md)
- **Building production apps?** Focus on [Error Handling](error-handling.md)
- **Want to optimize?** Explore [Advanced Features](advanced-features.md)
