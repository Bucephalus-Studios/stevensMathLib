/**
 * benchmark_rounding.cpp
 *
 * Performance benchmarks for rounding functions
 */

#include "../stevensMathLib.h"
#include <benchmark/benchmark.h>
#include <vector>
#include <random>

using namespace stevensMathLib;

// Benchmark data generation
static std::vector<float> generateTestData(size_t size)
{
    std::vector<float> data;
    data.reserve(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);

    for (size_t i = 0; i < size; ++i) {
        data.push_back(dis(gen));
    }

    return data;
}

// Benchmark for isWholeNumber
static void BM_IsWholeNumber(benchmark::State& state)
{
    auto testData = generateTestData(1000);

    for (auto _ : state) {
        for (const auto& value : testData) {
            benchmark::DoNotOptimize(isWholeNumber(value));
        }
    }

    state.SetItemsProcessed(state.iterations() * testData.size());
}
BENCHMARK(BM_IsWholeNumber);

// Benchmark for roundToNearest10th
static void BM_RoundToNearest10th(benchmark::State& state)
{
    auto testData = generateTestData(1000);

    for (auto _ : state) {
        for (const auto& value : testData) {
            benchmark::DoNotOptimize(roundToNearest10th(value));
        }
    }

    state.SetItemsProcessed(state.iterations() * testData.size());
}
BENCHMARK(BM_RoundToNearest10th);

// Benchmark for round with different precisions
static void BM_Round_Precision1(benchmark::State& state)
{
    auto testData = generateTestData(1000);

    for (auto _ : state) {
        for (const auto& value : testData) {
            benchmark::DoNotOptimize(round(value, 1));
        }
    }

    state.SetItemsProcessed(state.iterations() * testData.size());
}
BENCHMARK(BM_Round_Precision1);

static void BM_Round_Precision2(benchmark::State& state)
{
    auto testData = generateTestData(1000);

    for (auto _ : state) {
        for (const auto& value : testData) {
            benchmark::DoNotOptimize(round(value, 2));
        }
    }

    state.SetItemsProcessed(state.iterations() * testData.size());
}
BENCHMARK(BM_Round_Precision2);

static void BM_Round_Precision5(benchmark::State& state)
{
    auto testData = generateTestData(1000);

    for (auto _ : state) {
        for (const auto& value : testData) {
            benchmark::DoNotOptimize(round(value, 5));
        }
    }

    state.SetItemsProcessed(state.iterations() * testData.size());
}
BENCHMARK(BM_Round_Precision5);

// Benchmark for floatToInt
static void BM_FloatToInt_Float(benchmark::State& state)
{
    auto testData = generateTestData(1000);

    for (auto _ : state) {
        for (const auto& value : testData) {
            benchmark::DoNotOptimize(floatToInt(value));
        }
    }

    state.SetItemsProcessed(state.iterations() * testData.size());
}
BENCHMARK(BM_FloatToInt_Float);

static void BM_FloatToInt_Double(benchmark::State& state)
{
    auto testData = generateTestData(1000);
    std::vector<double> doubleData(testData.begin(), testData.end());

    for (auto _ : state) {
        for (const auto& value : doubleData) {
            benchmark::DoNotOptimize(floatToInt(value));
        }
    }

    state.SetItemsProcessed(state.iterations() * doubleData.size());
}
BENCHMARK(BM_FloatToInt_Double);
