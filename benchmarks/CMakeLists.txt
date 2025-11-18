cmake_minimum_required(VERSION 3.14)
project(stevensMathLib_benchmarks VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Create interface library for header-only library
add_library(stevensMathLib INTERFACE)
target_include_directories(stevensMathLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Fetch Google Benchmark
include(FetchContent)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googlebenchmark)

# Add benchmark executable
add_executable(stevensMathLib_benchmarks
    benchmark_main.cpp
    benchmark_rounding.cpp
    benchmark_random.cpp
)

target_link_libraries(stevensMathLib_benchmarks
    PRIVATE
        stevensMathLib
        benchmark::benchmark
        benchmark::benchmark_main
)
