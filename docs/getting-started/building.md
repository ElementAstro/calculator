# Building from Source

This guide covers how to build the calculator library from source, including examples, tests, and benchmarks. While the library itself is header-only, building from source allows you to run tests, benchmarks, and explore the provided examples.

## Prerequisites

### Required Tools

- **CMake**: 3.20 or later
- **C++ Compiler**: GCC 8.0+, Clang 8.0+, or MSVC 19.0+
- **Git**: For cloning the repository

### Optional Dependencies

- **Google Test**: For running unit tests (auto-downloaded if not found)
- **Google Benchmark**: For performance benchmarks (optional)

## Quick Build

The fastest way to build everything:

```bash
git clone https://github.com/ElementAstro/calculator.git
cd calculator
mkdir build && cd build
cmake ..
cmake --build .
```

## Detailed Build Process

### 1. Clone the Repository

```bash
git clone https://github.com/ElementAstro/calculator.git
cd calculator
```

### 2. Configure Build Options

The project supports several build options:

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | `ON` | Build unit tests |
| `BUILD_EXAMPLES` | `ON` | Build example programs |
| `BUILD_BENCHMARKS` | `OFF` | Build performance benchmarks |

### 3. Configure with CMake

```bash
mkdir build && cd build

# Basic configuration
cmake ..

# Custom configuration
cmake .. -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DBUILD_BENCHMARKS=ON

# Release build with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release

# Debug build with debug symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

### 4. Build the Project

```bash
# Build everything
cmake --build .

# Build specific targets
cmake --build . --target example
cmake --build . --target calc_test
cmake --build . --target benchmark
```

## Platform-Specific Instructions

### Linux/macOS

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential cmake libgtest-dev

# Install dependencies (macOS with Homebrew)
brew install cmake googletest

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Windows (Visual Studio)

```bash
# Using Visual Studio Developer Command Prompt
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

### Windows (MinGW)

```bash
# Using MSYS2/MinGW
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-gtest

mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## Build Targets

### Examples

The project includes several example programs:

```bash
# Build and run the main example
cmake --build . --target example
./example  # Linux/macOS
.\example.exe  # Windows
```

**Example output**:

```
Result of expression 1: 3.5
Result of expression 2: 59
Result of expression 3: 10
```

### Tests

Run the comprehensive test suite:

```bash
# Build tests
cmake --build . --target calc_test

# Run tests
./calc_test  # Linux/macOS
.\calc_test.exe  # Windows

# Run tests with CTest
ctest --verbose
```

**Expected test output**:

```
[==========] Running 8 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 8 tests from CalculatorTest
[ RUN      ] CalculatorTest.BasicArithmetic
[       OK ] CalculatorTest.BasicArithmetic (0 ms)
...
[----------] 8 tests from CalculatorTest (X ms total)
[==========] 8 tests from 1 test suite ran. (X ms total)
[  PASSED  ] 8 tests.
```

### Benchmarks

Build and run performance benchmarks:

```bash
# Enable benchmarks during configuration
cmake .. -DBUILD_BENCHMARKS=ON

# Build benchmarks
cmake --build . --target benchmark

# Run benchmarks
./benchmark  # Linux/macOS
.\benchmark.exe  # Windows
```

## CMake Options Reference

### Compiler Selection

```bash
# Use specific compiler
cmake .. -DCMAKE_CXX_COMPILER=g++
cmake .. -DCMAKE_CXX_COMPILER=clang++

# Use specific C++ standard
cmake .. -DCMAKE_CXX_STANDARD=17
cmake .. -DCMAKE_CXX_STANDARD=20
```

### Build Types

```bash
# Debug build (default)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build (optimized)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Release with debug info
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Minimal size release
cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel
```

### Installation

```bash
# Configure installation prefix
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# Install the library
cmake --install .

# Install to custom location
cmake --install . --prefix /custom/path
```

## Troubleshooting

### Common Build Issues

**CMake version too old**:

```
CMake Error: CMake 3.20 or higher is required
```

Solution: Update CMake to version 3.20 or later.

**Compiler not found**:

```
CMake Error: CMAKE_CXX_COMPILER not set
```

Solution: Install a compatible C++ compiler or specify the path:

```bash
cmake .. -DCMAKE_CXX_COMPILER=/path/to/compiler
```

**Google Test not found**:

```
CMake Error: Could not find GTest
```

Solution: Install Google Test or let CMake download it automatically:

```bash
cmake .. -DBUILD_TESTS=OFF  # Disable tests
# or install GTest system-wide
```

### Performance Issues

**Slow compilation**:

- Use parallel builds: `cmake --build . --parallel`
- Reduce optimization level for debug builds
- Use precompiled headers if available

**Large binary size**:

- Use `CMAKE_BUILD_TYPE=MinSizeRel`
- Strip debug symbols: `strip ./example`

## Integration with IDEs

### Visual Studio Code

Create `.vscode/settings.json`:

```json
{
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "cmake.configureArgs": [
        "-DBUILD_TESTS=ON",
        "-DBUILD_EXAMPLES=ON"
    ]
}
```

### CLion

CLion automatically detects CMake projects. Configure build profiles in Settings → Build, Execution, Deployment → CMake.

### Visual Studio

Open the folder in Visual Studio 2019+ for automatic CMake integration.

## Continuous Integration

The project includes CI configurations for:

- **GitHub Actions**: Windows (MinGW), Linux, and macOS builds
- **CodeQL**: Static analysis
- **SonarCloud**: Code quality analysis

View the `.github/workflows/` directory for CI configuration details.

## Next Steps

- Explore the [Examples](../examples/) to see the calculator in action
- Read the [User Guide](../user-guide/) for comprehensive usage instructions
- Check out the [API Reference](../api-reference/) for detailed documentation
- Learn about [Contributing](../development/contributing.md) to the project
