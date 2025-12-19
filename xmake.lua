-- xmake.lua
-- Build configuration for Modern C++ Calculator v2.0

set_project("calculator")
set_version("2.1.0")
set_xmakever("2.5.0")

-- Set C++ standard
set_languages("c++17")

-- Build options
option("tests")
    set_default(true)
    set_showmenu(true)
    set_description("Build tests")
option_end()

option("examples")
    set_default(true)
    set_showmenu(true)
    set_description("Build examples")
option_end()

option("benchmarks")
    set_default(false)
    set_showmenu(true)
    set_description("Build benchmarks")
option_end()

option("repl")
    set_default(true)
    set_showmenu(true)
    set_description("Build REPL executable")
option_end()

-- Header-only library
target("calculator")
    set_kind("headeronly")
    add_headerfiles("calculator.hpp")
    add_headerfiles("include/calculator/*.hpp")
    add_includedirs(".", "include", {public = true})
target_end()

-- Examples
if has_config("examples") then
    target("example")
        set_kind("binary")
        add_files("example/main.cpp")
        add_deps("calculator")
        set_targetdir("$(builddir)/example")
        if is_plat("windows") then
            add_defines("_USE_MATH_DEFINES")
        end
    target_end()

    target("example_functions")
        set_kind("binary")
        add_files("example/functions.cpp")
        add_deps("calculator")
        set_targetdir("$(builddir)/example")
        if is_plat("windows") then
            add_defines("_USE_MATH_DEFINES")
        end
    target_end()

    target("example_errors")
        set_kind("binary")
        add_files("example/errors.cpp")
        add_deps("calculator")
        set_targetdir("$(builddir)/example")
        if is_plat("windows") then
            add_defines("_USE_MATH_DEFINES")
        end
    target_end()

    target("example_types")
        set_kind("binary")
        add_files("example/types.cpp")
        add_deps("calculator")
        set_targetdir("$(builddir)/example")
        if is_plat("windows") then
            add_defines("_USE_MATH_DEFINES")
        end
    target_end()
end

-- REPL
if has_config("repl") then
    target("calc_repl")
        set_kind("binary")
        add_files("example/repl_main.cpp")
        add_deps("calculator")
        set_targetdir("$(builddir)/bin")
        if is_plat("windows") then
            add_defines("_USE_MATH_DEFINES")
        end
    target_end()
end

-- Tests
if has_config("tests") then
    add_requires("gtest")
    
    target("calc_test")
        set_kind("binary")
        add_files("test/main.cpp", "test/test_parser.cpp")
        add_deps("calculator")
        add_packages("gtest")
        set_targetdir("$(builddir)/test")
        if is_plat("windows") then
            add_defines("_USE_MATH_DEFINES")
        end
        
        -- Add test
        on_run(function (target)
            os.execv(target:targetfile())
        end)
    target_end()
end

-- Benchmarks
if has_config("benchmarks") then
    add_requires("benchmark")
    
    target("calc_benchmark")
        set_kind("binary")
        add_files("benchmark/main.cpp")
        add_deps("calculator")
        add_packages("benchmark")
        set_targetdir("$(builddir)/benchmark")
        if is_plat("windows") then
            add_defines("_USE_MATH_DEFINES")
        end
    target_end()
end

-- Install rules
target("install")
    set_kind("phony")
    on_install(function (target)
        os.cp("calculator.hpp", "$(installdir)/include/")
        os.cp("include/calculator", "$(installdir)/include/")
    end)
target_end()
