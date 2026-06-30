add_executable(tests
    test_main.cpp
    test_rounding.cpp
    test_random.cpp
    test_conversion.cpp
    test_range.cpp
)

target_link_libraries(tests
    PRIVATE
        stevensMathLib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
