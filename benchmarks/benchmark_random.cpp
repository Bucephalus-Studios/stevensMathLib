/**
 * benchmark_random.cpp
 *
 * Performance benchmarks for random number generation functions
 */

#include "../stevensMathLib.h"
#include <benchmark/benchmark.h>
#include <vector>

using namespace stevensMathLib;

// Benchmark for randomInt
static void BM_RandomInt_SmallRange(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(randomInt(0, 10));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomInt_SmallRange);

static void BM_RandomInt_MediumRange(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(randomInt(0, 1000));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomInt_MediumRange);

static void BM_RandomInt_LargeRange(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(randomInt(0, 1000000));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomInt_LargeRange);

// Benchmark for randomFloat
static void BM_RandomFloat_DefaultRange(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(randomFloat());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomFloat_DefaultRange);

static void BM_RandomFloat_CustomRange(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(randomFloat(-100.0f, 100.0f));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomFloat_CustomRange);

// Benchmark for randomIntNotInBlacklist with different scenarios
static void BM_RandomIntNotInBlacklist_EmptyBlacklist(benchmark::State& state)
{
    std::vector<int> emptyBlacklist;

    for (auto _ : state) {
        benchmark::DoNotOptimize(randomIntNotInBlacklist(emptyBlacklist, 0, 100));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomIntNotInBlacklist_EmptyBlacklist);

static void BM_RandomIntNotInBlacklist_SmallBlacklist(benchmark::State& state)
{
    std::vector<int> blacklist = {5, 15, 25, 35, 45};

    for (auto _ : state) {
        benchmark::DoNotOptimize(randomIntNotInBlacklist(blacklist, 0, 100));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomIntNotInBlacklist_SmallBlacklist);

static void BM_RandomIntNotInBlacklist_MediumBlacklist(benchmark::State& state)
{
    std::vector<int> blacklist;
    for (int i = 0; i < 50; i += 5) {
        blacklist.push_back(i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(randomIntNotInBlacklist(blacklist, 0, 100));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RandomIntNotInBlacklist_MediumBlacklist);

// Benchmark for in_range
static void BM_InRange_Inclusive_Int(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(in_range(50, 0, 100, BoundType::Inclusive));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_InRange_Inclusive_Int);

static void BM_InRange_Exclusive_Int(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(in_range(50, 0, 100, BoundType::Exclusive));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_InRange_Exclusive_Int);

static void BM_InRange_Inclusive_Float(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(in_range(50.5f, 0.0f, 100.0f, BoundType::Inclusive));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_InRange_Inclusive_Float);

static void BM_InRange_Exclusive_Float(benchmark::State& state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(in_range(50.5f, 0.0f, 100.0f, BoundType::Exclusive));
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_InRange_Exclusive_Float);

// Benchmark comparing getRandomEngine vs seedRNG
static void BM_GetRandomEngine(benchmark::State& state)
{
    for (auto _ : state) {
        auto& engine = getRandomEngine();
        benchmark::DoNotOptimize(engine());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_GetRandomEngine);
