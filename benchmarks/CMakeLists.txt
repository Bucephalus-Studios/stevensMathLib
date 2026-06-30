add_executable(benchmarks
    benchmark_main.cpp
    benchmark_rounding.cpp
    benchmark_random.cpp
)

target_link_libraries(benchmarks
    PRIVATE
        stevensMathLib
        benchmark::benchmark
)