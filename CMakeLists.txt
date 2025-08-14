cmake_minimum_required(VERSION 3.20)
project(calculator VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_library(calculator INTERFACE)
target_include_directories(calculator INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.0.0")
        message(FATAL_ERROR "GCC/Clang version must be at least 8.0.0")
    endif()
elseif(MSVC)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.0.0")
        message(FATAL_ERROR "MSVC version must be at least 19.0.0")
    endif()
endif()

install(FILES calculator.hpp DESTINATION calculator)

if (BUILD_EXAMPLES)
    # Main comprehensive example
    add_executable(example example/main.cpp)
    target_link_libraries(example PRIVATE calculator)

    # Advanced usage patterns
    add_executable(advanced_usage example/advanced_usage.cpp)
    target_link_libraries(advanced_usage PRIVATE calculator)

    # Error handling demonstration
    add_executable(error_handling_demo example/error_handling_demo.cpp)
    target_link_libraries(error_handling_demo PRIVATE calculator)

    # Numeric types demonstration
    add_executable(numeric_types_demo example/numeric_types_demo.cpp)
    target_link_libraries(numeric_types_demo PRIVATE calculator)

    # Set output directory for examples
    set_target_properties(example advanced_usage error_handling_demo numeric_types_demo
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/example
    )
endif()

if (BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    add_executable(calc_test test/main.cpp)
    target_link_libraries(calc_test PRIVATE calculator GTest::gtest)
    add_test(NAME CalcTest COMMAND calc_test)
endif()

if (BUILD_BENCHMARKS)
    find_package(benchmark REQUIRED)
    add_executable(benchmark benchmark/main.cpp)
    target_link_libraries(benchmark PRIVATE calculator benchmark::benchmark)
endif()
