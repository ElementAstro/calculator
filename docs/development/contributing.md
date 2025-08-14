# Contributing Guide

Thank you for your interest in contributing to the Modern C++ Calculator library! This guide will help you get started with contributing code, documentation, or other improvements to the project.

## Ways to Contribute

### 1. Code Contributions

- **Bug fixes**: Fix issues reported in GitHub Issues
- **New features**: Implement new functionality
- **Performance improvements**: Optimize existing code
- **Code quality**: Refactoring and cleanup

### 2. Documentation

- **API documentation**: Improve code comments and documentation
- **User guides**: Write tutorials and examples
- **README improvements**: Enhance project description and usage
- **Wiki contributions**: Add community knowledge

### 3. Testing

- **Unit tests**: Add test coverage for existing code
- **Integration tests**: Test component interactions
- **Performance tests**: Benchmark critical paths
- **Edge case testing**: Test boundary conditions

### 4. Community Support

- **Issue triage**: Help categorize and prioritize issues
- **Code reviews**: Review pull requests from other contributors
- **User support**: Answer questions in discussions
- **Bug reports**: Report issues with detailed reproduction steps

## Getting Started

### 1. Set Up Development Environment

```bash
# Fork and clone the repository
git clone https://github.com/YOUR_USERNAME/calculator.git
cd calculator

# Set up upstream remote
git remote add upstream https://github.com/ElementAstro/calculator.git

# Create development build
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DBUILD_BENCHMARKS=ON
cmake --build .

# Verify setup by running tests
./calc_test
```

### 2. Find Something to Work On

- **Good first issues**: Look for issues labeled `good first issue`
- **Help wanted**: Issues labeled `help wanted` need contributors
- **Documentation**: Issues labeled `documentation` are great for new contributors
- **Bug reports**: Issues labeled `bug` need investigation and fixes

### 3. Create a Branch

```bash
# Update your fork
git fetch upstream
git checkout main
git merge upstream/main

# Create feature branch
git checkout -b feature/your-feature-name

# Or create bugfix branch
git checkout -b bugfix/issue-123-description
```

## Development Workflow

### 1. Code Standards

#### C++ Style Guide

Follow these conventions:

```cpp
// Class names: PascalCase
class ExpressionParser {
private:
    // Private members: trailing underscore
    std::unordered_map<std::string, double> variables_;
    std::unordered_map<std::string, std::function<double(double)>> functions_;
    
public:
    // Method names: snake_case
    double eval(std::string_view expression);
    void set_variable(const std::string& name, double value);
    
    // Use const correctness
    bool has_variable(const std::string& name) const;
    
    // Use explicit constructors
    explicit ExpressionParser() = default;
    
    // Use move semantics appropriately
    void set_function(std::string name, std::function<double(double)> func) {
        functions_[std::move(name)] = std::move(func);
    }
};

// Constants: UPPER_SNAKE_CASE
constexpr int MAX_RECURSION_DEPTH = 100;

// Free functions: snake_case
template<typename T>
T eval(std::string_view expression);
```

#### Documentation Standards

All public APIs must be documented:

```cpp
/**
 * @brief Evaluates a mathematical expression
 * 
 * This function parses and evaluates mathematical expressions containing
 * arithmetic operations, functions, and variables.
 * 
 * @param expression The mathematical expression to evaluate
 * @return The result of the evaluation
 * @throws calculator::error If the expression contains syntax errors
 * 
 * @example
 * ```cpp
 * auto parser = calculator::ExpressionParser<double>();
 * parser.set("x", 5.0);
 * double result = parser.eval("x * 2 + 1");  // Returns 11.0
 * ```
 * 
 * @see set_variable, set_function
 * @since 1.0.0
 */
double eval(std::string_view expression);
```

### 2. Testing Requirements

#### Write Tests First (TDD)

```cpp
#include <gtest/gtest.h>
#include "calculator.hpp"

// Test class setup
class CalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser_.set("pi", M_PI);
        parser_.set("sin", [](double x) { return std::sin(x); });
    }
    
    calculator::ExpressionParser<double> parser_;
};

// Test basic functionality
TEST_F(CalculatorTest, BasicArithmetic) {
    EXPECT_DOUBLE_EQ(parser_.eval("2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(parser_.eval("10 - 4"), 6.0);
    EXPECT_DOUBLE_EQ(parser_.eval("3 * 4"), 12.0);
    EXPECT_DOUBLE_EQ(parser_.eval("15 / 3"), 5.0);
}

// Test error conditions
TEST_F(CalculatorTest, ErrorHandling) {
    EXPECT_THROW(parser_.eval("2 + + 3"), calculator::error);
    EXPECT_THROW(parser_.eval("5 / 0"), calculator::error);
    EXPECT_THROW(parser_.eval("(2 + 3"), calculator::error);
}

// Test edge cases
TEST_F(CalculatorTest, EdgeCases) {
    EXPECT_DOUBLE_EQ(parser_.eval("0"), 0.0);
    EXPECT_DOUBLE_EQ(parser_.eval("-0"), 0.0);
    EXPECT_DOUBLE_EQ(parser_.eval("1e-10"), 1e-10);
}
```

#### Test Coverage Requirements

- **New features**: Must have 100% line coverage
- **Bug fixes**: Must include regression tests
- **Edge cases**: Test boundary conditions
- **Error paths**: Test all error conditions

### 3. Performance Considerations

#### Benchmarking New Features

```cpp
#include <benchmark/benchmark.h>
#include "calculator.hpp"

static void BM_NewFeature(benchmark::State& state) {
    calculator::ExpressionParser<double> parser;
    
    for (auto _ : state) {
        // Benchmark your new feature
        double result = parser.eval("your_new_expression");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_NewFeature);

// Compare with baseline
static void BM_Baseline(benchmark::State& state) {
    calculator::ExpressionParser<double> parser;
    
    for (auto _ : state) {
        double result = parser.eval("baseline_expression");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Baseline);
```

#### Performance Guidelines

1. **Profile before optimizing**: Use profiling tools to identify bottlenecks
2. **Measure impact**: Benchmark before and after changes
3. **Avoid premature optimization**: Focus on correctness first
4. **Consider memory usage**: Minimize allocations in hot paths

## Pull Request Process

### 1. Before Submitting

- [ ] All tests pass locally
- [ ] Code follows style guidelines
- [ ] Documentation is updated
- [ ] Benchmarks show acceptable performance
- [ ] Commit messages are clear and descriptive

### 2. Pull Request Template

```markdown
## Description
Brief description of changes made.

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update

## Testing
- [ ] Unit tests added/updated
- [ ] All tests pass
- [ ] Manual testing performed

## Performance Impact
- [ ] No performance impact
- [ ] Performance improved
- [ ] Performance regression (justified)

## Related Issues
Fixes #123
Related to #456

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] Tests added for new functionality
```

### 3. Review Process

1. **Automated checks**: CI/CD pipeline runs automatically
2. **Code review**: Maintainers review code quality and design
3. **Testing**: Reviewers verify tests are comprehensive
4. **Documentation**: Ensure documentation is clear and complete
5. **Approval**: At least one maintainer approval required

### 4. After Approval

- **Squash commits**: Clean up commit history if needed
- **Merge**: Maintainer will merge the pull request
- **Cleanup**: Delete feature branch after merge

## Code Review Guidelines

### As a Reviewer

#### What to Look For

1. **Correctness**: Does the code do what it's supposed to do?
2. **Style**: Does it follow project conventions?
3. **Performance**: Are there obvious performance issues?
4. **Security**: Are there potential security vulnerabilities?
5. **Maintainability**: Is the code easy to understand and modify?

#### Review Checklist

- [ ] Code is well-structured and readable
- [ ] Error handling is appropriate
- [ ] Tests cover the new functionality
- [ ] Documentation is clear and complete
- [ ] No obvious performance regressions
- [ ] Follows project coding standards

#### Providing Feedback

```markdown
# Good feedback examples

## Constructive suggestions
Consider using `std::string_view` instead of `const std::string&` for better performance.

## Questions for clarification
Why did you choose this approach over using the existing `parse_number` function?

## Positive reinforcement
Great job on the comprehensive test coverage!

## Specific improvements
```cpp
// Instead of this:
if (result == nullptr) {
    return false;
}

// Consider this:
return result != nullptr;
```

```

### As a Contributor

#### Responding to Reviews

- **Be receptive**: Reviews help improve code quality
- **Ask questions**: If feedback is unclear, ask for clarification
- **Explain decisions**: Provide context for your implementation choices
- **Make changes promptly**: Address feedback in a timely manner

#### Handling Disagreements

1. **Discuss openly**: Use PR comments for technical discussions
2. **Provide evidence**: Back up your position with data or examples
3. **Seek consensus**: Work together to find the best solution
4. **Escalate if needed**: Involve maintainers for final decisions

## Community Guidelines

### Code of Conduct

We are committed to providing a welcoming and inclusive environment:

1. **Be respectful**: Treat all contributors with respect
2. **Be inclusive**: Welcome people of all backgrounds and experience levels
3. **Be constructive**: Provide helpful feedback and suggestions
4. **Be patient**: Remember that everyone is learning

### Communication

#### GitHub Issues

- **Use clear titles**: Describe the issue concisely
- **Provide context**: Include relevant information and examples
- **Follow templates**: Use issue templates when available
- **Search first**: Check if the issue already exists

#### Pull Request Discussions

- **Stay on topic**: Keep discussions focused on the code changes
- **Be specific**: Reference line numbers and provide concrete suggestions
- **Be timely**: Respond to feedback promptly

### Recognition

Contributors are recognized in several ways:

- **Contributors list**: Added to README.md
- **Release notes**: Mentioned in changelog
- **GitHub insights**: Contribution graphs and statistics
- **Community recognition**: Highlighted in discussions

## Getting Help

### Resources

- **[Development Guide](index.md)**: Overview of development process
- **[Building from Source](building-from-source.md)**: Detailed build instructions
- **[Testing Guide](testing.md)**: Testing best practices
- **[Benchmarking Guide](benchmarking.md)**: Performance testing

### Support Channels

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For questions and general discussion
- **Code Reviews**: For technical feedback on contributions
- **Email**: For security issues or private matters

### Mentorship

New contributors can get help from:

- **Good first issues**: Beginner-friendly tasks
- **Mentorship program**: Experienced contributors willing to help
- **Documentation**: Comprehensive guides and examples
- **Community support**: Active community in discussions

## Advanced Contributing

### Becoming a Maintainer

Regular contributors may be invited to become maintainers:

1. **Consistent contributions**: Regular, high-quality contributions
2. **Community involvement**: Active participation in discussions and reviews
3. **Technical expertise**: Deep understanding of the codebase
4. **Leadership**: Helping other contributors and guiding project direction

### Maintainer Responsibilities

- **Code review**: Review and approve pull requests
- **Issue triage**: Categorize and prioritize issues
- **Release management**: Coordinate releases and versioning
- **Community management**: Foster a welcoming environment

## Thank You!

Your contributions make this project better for everyone. Whether you're fixing a typo, adding a feature, or helping other users, every contribution is valuable and appreciated.

Happy coding! 🚀
