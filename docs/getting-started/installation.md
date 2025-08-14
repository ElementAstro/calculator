# Installation

The Modern C++ Calculator is a header-only library, making installation simple and straightforward. This guide covers various installation methods to suit different project setups.

## Requirements

### Compiler Support

| Compiler | Minimum Version | Recommended |
|----------|----------------|-------------|
| GCC      | 8.0            | 11.0+       |
| Clang    | 8.0            | 13.0+       |
| MSVC     | 19.0 (VS 2015) | 19.29+ (VS 2019) |

### C++ Standard

- **Minimum**: C++17
- **Recommended**: C++20 (for optimal performance and features)

## Installation Methods

### Method 1: Direct Download (Recommended)

The simplest way to get started:

1. **Download the header file**:

   ```bash
   curl -O https://raw.githubusercontent.com/ElementAstro/calculator/main/calculator.hpp
   ```

2. **Place in your project**:

   ```
   your-project/
   ├── include/
   │   └── calculator.hpp
   └── src/
       └── main.cpp
   ```

3. **Include in your code**:

   ```cpp
   #include "calculator.hpp"
   ```

### Method 2: Git Submodule

For projects using Git:

1. **Add as submodule**:

   ```bash
   git submodule add https://github.com/ElementAstro/calculator.git third-party/calculator
   ```

2. **Update your CMakeLists.txt**:

   ```cmake
   add_subdirectory(third-party/calculator)
   target_link_libraries(your-target PRIVATE calculator)
   ```

3. **Include in your code**:

   ```cpp
   #include "calculator.hpp"
   ```

### Method 3: CMake FetchContent

For modern CMake projects (3.14+):

```cmake
include(FetchContent)

FetchContent_Declare(
    calculator
    GIT_REPOSITORY https://github.com/ElementAstro/calculator.git
    GIT_TAG        main  # or specific version tag
)

FetchContent_MakeAvailable(calculator)

target_link_libraries(your-target PRIVATE calculator)
```

### Method 4: Manual Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/ElementAstro/calculator.git
   cd calculator
   ```

2. **Install system-wide** (optional):

   ```bash
   mkdir build && cd build
   cmake ..
   cmake --install . --prefix /usr/local
   ```

3. **Use in your project**:

   ```cpp
   #include <calculator/calculator.hpp>
   ```

## Verification

Create a simple test file to verify installation:

```cpp title="test_installation.cpp"
#include "calculator.hpp"
#include <iostream>

int main() {
    try {
        double result = calculator::eval("2 + 3 * 4");
        std::cout << "Calculator works! Result: " << result << std::endl;
        return 0;
    } catch (const calculator::error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```

**Compile and run**:

```bash
g++ -std=c++17 test_installation.cpp -o test_installation
./test_installation
```

**Expected output**:

```
Calculator works! Result: 14
```

## CMake Integration

### Basic CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.17)
project(my_calculator_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If using local copy
add_executable(my_app main.cpp)
target_include_directories(my_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# If using installed version
# find_package(calculator REQUIRED)
# target_link_libraries(my_app PRIVATE calculator::calculator)
```

### Advanced CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)
project(advanced_calculator_project VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(my_app PRIVATE -Wall -Wextra -O3)
elseif(MSVC)
    target_compile_options(my_app PRIVATE /W4 /O2)
endif()

add_executable(my_app main.cpp)
target_include_directories(my_app PRIVATE include)
```

## Package Managers

### vcpkg

```bash
vcpkg install calculator
```

### Conan

```python title="conanfile.txt"
[requires]
calculator/1.0.0

[generators]
cmake
```

!!! note "Package Manager Support"
    Package manager support is planned for future releases. Currently, direct download or Git integration is recommended.

## Troubleshooting

### Common Issues

**Compiler errors about C++17 features**:

```bash
# Ensure C++17 is enabled
g++ -std=c++17 your_file.cpp
```

**Header not found**:

```cpp
// Check include path
#include "calculator.hpp"  // Relative path
// or
#include <calculator.hpp>  // System path
```

**Linker errors**:
The library is header-only, so no linking is required. If you see linker errors, ensure you're not trying to link against a non-existent library file.

### Getting Help

If you encounter issues:

1. Check the [Examples](../examples/) for working code
2. Review the [API Reference](../api-reference/) for correct usage
3. Open an issue on [GitHub](https://github.com/ElementAstro/calculator/issues)

## Next Steps

- Continue with the [Quick Start Guide](quick-start.md)
- Learn about [Building from Source](building.md)
- Explore [Basic Usage](../user-guide/basic-usage.md)
