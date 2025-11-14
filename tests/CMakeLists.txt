# Tests CMakeLists.txt

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
