# Benchmarks CMakeLists.txt

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
