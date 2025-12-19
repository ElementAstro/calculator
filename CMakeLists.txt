cmake_minimum_required(VERSION 3.20)
project(calculator VERSION 2.0.0 LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_REPL "Build REPL executable" ON)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_library(calculator INTERFACE)
target_include_directories(calculator INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

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
    # Main example
    add_executable(example example/main.cpp)
    target_link_libraries(example PRIVATE calculator)

    # Functions example
    add_executable(example_functions example/functions.cpp)
    target_link_libraries(example_functions PRIVATE calculator)

    # Error handling example
    add_executable(example_errors example/errors.cpp)
    target_link_libraries(example_errors PRIVATE calculator)

    # Numeric types example
    add_executable(example_types example/types.cpp)
    target_link_libraries(example_types PRIVATE calculator)

    # Ensure math constants like M_PI and M_E are available on MSVC
    if (MSVC)
        target_compile_definitions(example PRIVATE _USE_MATH_DEFINES)
        target_compile_definitions(example_functions PRIVATE _USE_MATH_DEFINES)
        target_compile_definitions(example_errors PRIVATE _USE_MATH_DEFINES)
        target_compile_definitions(example_types PRIVATE _USE_MATH_DEFINES)
    endif()

    # Set output directory for examples
    set_target_properties(example example_functions example_errors example_types
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/example
    )
endif()

if (BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    add_executable(calc_test test/main.cpp test/test_parser.cpp)
    target_link_libraries(calc_test PRIVATE calculator GTest::gtest GTest::gtest_main)
    if (MSVC)
        target_compile_definitions(calc_test PRIVATE _USE_MATH_DEFINES)
    endif()
    add_test(NAME CalcTest COMMAND calc_test)
endif()

if (BUILD_BENCHMARKS)
    find_package(benchmark REQUIRED)
    add_executable(calc_benchmark benchmark/main.cpp)
    target_link_libraries(calc_benchmark PRIVATE calculator benchmark::benchmark)
endif()

if (BUILD_REPL)
    add_executable(calc_repl example/repl_main.cpp)
    target_link_libraries(calc_repl PRIVATE calculator)
    if (MSVC)
        target_compile_definitions(calc_repl PRIVATE _USE_MATH_DEFINES)
    endif()
    set_target_properties(calc_repl PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()
