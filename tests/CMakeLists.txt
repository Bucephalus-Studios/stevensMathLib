cmake_minimum_required(VERSION 3.14)
project(stevensMathLib_tests VERSION 1.0.0 LANGUAGES CXX)

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

# Fetch Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# Add test executable
add_executable(stevensMathLib_tests
    test_main.cpp
    test_rounding.cpp
    test_random.cpp
    test_conversion.cpp
    test_range.cpp
)

target_link_libraries(stevensMathLib_tests
    PRIVATE
        stevensMathLib
        GTest::gtest
        GTest::gtest_main
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(stevensMathLib_tests)
