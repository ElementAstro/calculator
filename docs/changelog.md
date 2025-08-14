# Changelog

All notable changes to the Modern C++ Calculator library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Comprehensive MkDocs documentation
- Complete API reference documentation
- User guide with examples
- Getting started tutorials
- Reference documentation for operators and types

### Changed

- Improved README with links to full documentation
- Enhanced code examples and usage patterns

### Documentation

- Added complete MkDocs-based documentation site
- Created comprehensive examples for all features
- Added API reference for all classes and functions
- Included troubleshooting and best practices guides

## [1.0.0] - 2024-09-24

### Added

- Initial release of the Modern C++ Calculator library
- Template-based `ExpressionParser<T>` class
- Support for multiple numeric types (`int`, `float`, `double`)
- Comprehensive arithmetic operations (`+`, `-`, `*`, `/`, `%`, `**`)
- Bitwise operations for integer types (`|`, `^`, `&`, `<<`, `>>`, `~`)
- Unary operators (`+`, `-`, `~`)
- Scientific notation support (`e`, `E`)
- Variable support with `set(name, value)`
- Custom function support with `set(name, function)`
- Robust error handling with `calculator::error` exception
- Operator precedence following mathematical standards
- Parentheses support for expression grouping
- Free functions for simple evaluation (`eval<T>()`, `eval()`)

### Features

- **Header-only design** - Single file inclusion
- **Type safety** - Template-based implementation
- **High performance** - Optimized expression parsing
- **Extensible** - Custom variables and functions
- **Standards compliant** - Follows C++17/20 standards
- **Cross-platform** - Works on Linux, Windows, macOS

### Supported Operations

- **Arithmetic**: `+`, `-`, `*`, `/`, `%`, `**`
- **Bitwise** (int only): `|`, `^`, `&`, `<<`, `>>`, `~`
- **Unary**: `+`, `-`, `~`
- **Scientific notation**: `1.5e2`, `2.5E-3`
- **Parentheses**: `(`, `)`

### Supported Types

- `int` - 32-bit integers with full bitwise support
- `float` - Single-precision floating-point
- `double` - Double-precision floating-point (default)

### Build System

- CMake 3.20+ support
- Optional components: examples, tests, benchmarks
- Compiler support: GCC 8+, Clang 8+, MSVC 19+
- C++17 minimum, C++20 recommended

### Testing

- Comprehensive unit tests with Google Test
- Coverage for all operations and error conditions
- Continuous integration with GitHub Actions
- Cross-platform testing (Linux, Windows, macOS)

### Examples

- Basic arithmetic examples
- Variable and function usage
- Error handling patterns
- Real-world applications (scientific, financial, gaming)

### Documentation

- Comprehensive README with usage examples
- Inline code documentation
- Build instructions
- API reference in header comments

## Development History

### Pre-1.0.0 Development

#### [0.9.0] - Development Phase

- Core expression parsing engine
- Basic arithmetic operations
- Initial template design
- Error handling framework

#### [0.8.0] - Development Phase  

- Bitwise operations implementation
- Type-specific behavior design
- Operator precedence system
- Variable support prototype

#### [0.7.0] - Development Phase

- Function support implementation
- Scientific notation parsing
- Unary operator support
- Expression validation

#### [0.6.0] - Development Phase

- Template architecture design
- Multi-type support
- Performance optimizations
- Memory management improvements

#### [0.5.0] - Development Phase

- Initial parser implementation
- Basic calculator functionality
- Proof of concept
- Core algorithms

## Migration Guide

### Upgrading to 1.0.0

This is the initial stable release. No migration is required.

### Future Compatibility

The library follows semantic versioning:

- **Patch releases** (1.0.x): Bug fixes, no API changes
- **Minor releases** (1.x.0): New features, backward compatible
- **Major releases** (x.0.0): Breaking changes, migration guide provided

## Planned Features

### Version 1.1.0 (Planned)

- Additional mathematical functions (trigonometric, logarithmic)
- Multi-argument function support
- Expression compilation and caching
- Performance improvements

### Version 1.2.0 (Planned)

- Complex number support
- Matrix operations
- Vector calculations
- Advanced mathematical constants

### Version 2.0.0 (Future)

- Multi-threaded evaluation
- JIT compilation support
- Plugin architecture
- Advanced optimization features

## Contributing

We welcome contributions! Please see our [Contributing Guide](development/contributing.md) for details.

### How to Contribute

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Update documentation
6. Submit a pull request

### Reporting Issues

- Use GitHub Issues for bug reports
- Provide minimal reproduction cases
- Include system information and compiler details
- Check existing issues before creating new ones

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/ElementAstro/calculator/blob/main/LICENSE) file for details.

## Acknowledgments

### Contributors

- Max Qian - Original author and maintainer
- Community contributors - Bug reports and feature suggestions

### Inspiration

- Mathematical expression parsing literature
- C++ template metaprogramming techniques
- Modern C++ best practices
- Open source calculator implementations

### Tools and Libraries

- **CMake** - Build system
- **Google Test** - Unit testing framework
- **Google Benchmark** - Performance testing
- **GitHub Actions** - Continuous integration
- **SonarCloud** - Code quality analysis
- **MkDocs** - Documentation generation

## Support

### Getting Help

- 📖 [Documentation](https://elementastro.github.io/calculator/)
- 💬 [GitHub Discussions](https://github.com/ElementAstro/calculator/discussions)
- 🐛 [Issue Tracker](https://github.com/ElementAstro/calculator/issues)
- 📧 [Email Support](mailto:lightapt.com)

### Community

- Follow development on [GitHub](https://github.com/ElementAstro/calculator)
- Star the repository if you find it useful
- Share your use cases and feedback

---

**Note**: This changelog is automatically updated with each release. For the most current information, please check the [GitHub repository](https://github.com/ElementAstro/calculator).
