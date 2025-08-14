# Building from Source

This guide provides detailed instructions for building the Modern C++ Calculator library from source code, including all dependencies, build options, and platform-specific considerations.

## Prerequisites

### System Requirements

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| **C++ Compiler** | C++17 support | C++20 support |
| **CMake** | 3.20+ | 3.25+ |
| **Memory** | 2GB RAM | 4GB+ RAM |
| **Disk Space** | 100MB | 500MB |

### Supported Compilers

| Compiler | Minimum Version | Recommended | Notes |
|----------|----------------|-------------|-------|
| **GCC** | 8.0 | 11.0+ | Full C++17/20 support |
| **Clang** | 8.0 | 13.0+ | Full C++17/20 support |
| **MSVC** | 19.0 (VS 2015) | 19.29+ (VS 2019) | C++17 mode required |
| **Apple Clang** | 10.0 | 12.0+ | Xcode 10+ |

## Quick Start

### Basic Build

```bash
# Clone the repository
git clone https://github.com/ElementAstro/calculator.git
cd calculator

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run tests (optional)
./calc_test  # Linux/macOS
.\calc_test.exe  # Windows
```

### Development Build

```bash
# Configure with all options enabled
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTS=ON \
  -DBUILD_EXAMPLES=ON \
  -DBUILD_BENCHMARKS=ON \
  -DENABLE_COVERAGE=ON

# Build everything
cmake --build . --parallel

# Run comprehensive tests
ctest --verbose
```

## Build Options

### CMake Configuration Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | `OFF` | Build unit tests |
| `BUILD_EXAMPLES` | `OFF` | Build example programs |
| `BUILD_BENCHMARKS` | `OFF` | Build performance benchmarks |
| `ENABLE_COVERAGE` | `OFF` | Enable code coverage |
| `ENABLE_SANITIZERS` | `OFF` | Enable AddressSanitizer/UBSan |
| `ENABLE_LTO` | `OFF` | Enable Link Time Optimization |
| `BUILD_DOCS` | `OFF` | Build documentation |

### Build Types

```bash
# Debug build (default)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build (optimized)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Release with debug info
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Minimum size release
cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel
```

## Platform-Specific Instructions

### Linux

#### Ubuntu/Debian

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake git

# For testing (optional)
sudo apt install libgtest-dev

# For benchmarking (optional)
sudo apt install libbenchmark-dev

# Clone and build
git clone https://github.com/ElementAstro/calculator.git
cd calculator
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
make -j$(nproc)
```

#### CentOS/RHEL/Fedora

```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install cmake git

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake git

# Build process same as Ubuntu
```

#### Arch Linux

```bash
# Install dependencies
sudo pacman -S base-devel cmake git

# Optional dependencies
sudo pacman -S gtest benchmark

# Build process same as Ubuntu
```

### Windows

#### Visual Studio

```cmd
REM Install Visual Studio 2019/2022 with C++ workload
REM Install Git for Windows
REM Install CMake

REM Clone repository
git clone https://github.com/ElementAstro/calculator.git
cd calculator

REM Create build directory
mkdir build
cd build

REM Configure for Visual Studio
cmake .. -G "Visual Studio 16 2019" -A x64
REM Or for VS 2022: cmake .. -G "Visual Studio 17 2022" -A x64

REM Build
cmake --build . --config Release
```

#### MinGW-w64

```bash
# Install MSYS2 and MinGW-w64
# In MSYS2 terminal:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make git

# Clone and build
git clone https://github.com/ElementAstro/calculator.git
cd calculator
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DBUILD_TESTS=ON
mingw32-make -j$(nproc)
```

#### Clang on Windows

```cmd
REM Install LLVM/Clang for Windows
REM Set environment variables

cmake .. -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
ninja
```

### macOS

#### Using Xcode

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (via Homebrew)
brew install cmake

# Clone and build
git clone https://github.com/ElementAstro/calculator.git
cd calculator
mkdir build && cd build
cmake .. -G Xcode -DBUILD_TESTS=ON
xcodebuild -configuration Release
```

#### Using Homebrew

```bash
# Install dependencies
brew install cmake git

# Optional dependencies
brew install googletest google-benchmark

# Clone and build
git clone https://github.com/ElementAstro/calculator.git
cd calculator
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_BENCHMARKS=ON
make -j$(sysctl -n hw.ncpu)
```

## Advanced Build Configuration

### Cross-Compilation

#### ARM64 on x86_64

```bash
# Install cross-compilation toolchain
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# Configure for ARM64
cmake .. \
  -DCMAKE_SYSTEM_NAME=Linux \
  -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
  -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
  -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++

make -j$(nproc)
```

#### Windows on Linux (MinGW)

```bash
# Install MinGW cross-compiler
sudo apt install mingw-w64

# Configure for Windows
cmake .. \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++

make -j$(nproc)
```

### Static Analysis

#### Clang Static Analyzer

```bash
# Configure with Clang
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

# Run static analyzer
scan-build make
```

#### Clang-Tidy

```bash
# Configure with compile commands
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Run clang-tidy
clang-tidy src/*.cpp -p build/
```

### Sanitizers

#### AddressSanitizer

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"

make -j$(nproc)
./calc_test
```

#### UndefinedBehaviorSanitizer

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=undefined -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=undefined"

make -j$(nproc)
./calc_test
```

#### ThreadSanitizer

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=thread -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread"

make -j$(nproc)
./calc_test
```

## Testing

### Running Tests

```bash
# Build with tests enabled
cmake .. -DBUILD_TESTS=ON
make -j$(nproc)

# Run all tests
./calc_test

# Run specific test suite
./calc_test --gtest_filter="CalculatorTest.*"

# Run with verbose output
./calc_test --gtest_verbose

# Using CTest
ctest --verbose
ctest --parallel $(nproc)
```

### Code Coverage

```bash
# Configure with coverage
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_COVERAGE=ON \
  -DCMAKE_CXX_FLAGS="--coverage" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"

# Build and run tests
make -j$(nproc)
./calc_test

# Generate coverage report
gcov *.gcno
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

## Benchmarking

### Building Benchmarks

```bash
# Install Google Benchmark
sudo apt install libbenchmark-dev  # Ubuntu
brew install google-benchmark      # macOS

# Configure with benchmarks
cmake .. -DBUILD_BENCHMARKS=ON
make -j$(nproc)

# Run benchmarks
./calc_benchmark

# Run specific benchmark
./calc_benchmark --benchmark_filter="BM_SimpleExpression"

# Output to JSON
./calc_benchmark --benchmark_format=json --benchmark_out=results.json
```

## Documentation

### Building Documentation

```bash
# Install dependencies
pip install mkdocs mkdocs-material

# Build documentation
mkdocs build

# Serve locally
mkdocs serve
```

## Troubleshooting

### Common Issues

#### CMake Version Too Old

```bash
# Error: CMake 3.20 or higher is required
# Solution: Install newer CMake
wget https://github.com/Kitware/CMake/releases/download/v3.25.0/cmake-3.25.0-linux-x86_64.sh
chmod +x cmake-3.25.0-linux-x86_64.sh
sudo ./cmake-3.25.0-linux-x86_64.sh --prefix=/usr/local --skip-license
```

#### Compiler Not Found

```bash
# Error: Could not find compiler
# Solution: Install development tools
sudo apt install build-essential  # Ubuntu
sudo yum groupinstall "Development Tools"  # CentOS
```

#### Missing Dependencies

```bash
# Error: Could not find GTest
# Solution: Install or build from source
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

### Debug Build Issues

```bash
# Clean build directory
rm -rf build/*

# Reconfigure with verbose output
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON

# Build with verbose output
make VERBOSE=1
```

### Performance Issues

```bash
# Use parallel builds
make -j$(nproc)  # Linux
make -j$(sysctl -n hw.ncpu)  # macOS

# Enable Link Time Optimization
cmake .. -DENABLE_LTO=ON -DCMAKE_BUILD_TYPE=Release
```

## Integration

### Using as Submodule

```bash
# Add as Git submodule
git submodule add https://github.com/ElementAstro/calculator.git third_party/calculator

# In your CMakeLists.txt
add_subdirectory(third_party/calculator)
target_link_libraries(your_target calculator)
```

### Using with vcpkg

```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# Install calculator (when available)
./vcpkg install calculator
```

### Using with Conan

```python
# conanfile.txt
[requires]
calculator/1.0.0

[generators]
cmake
```

## Next Steps

After successfully building the library:

1. **[Run the tests](testing.md)** to verify everything works
2. **[Try the examples](../examples/)** to see the library in action
3. **[Read the API documentation](../api-reference/)** for detailed usage
4. **[Contribute improvements](contributing.md)** to help the project grow

Happy building! 🔨
