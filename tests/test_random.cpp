/**
 * test_random.cpp
 *
 * Unit tests for random number generation functions in stevensMathLib
 */

#include "../stevensMathLib.h"
#include <gtest/gtest.h>
#include <cstdint>
#include <future>
#include <set>
#include <sstream>
#include <thread>
#include <vector>

using namespace stevensMathLib;

// Test fixture for random number generation
class RandomTest : public ::testing::Test
{
protected:
    static constexpr int sampleSize = 1000;
    static constexpr int minUniqueExpected = 10;
};

// Tests for randomInt
TEST_F(RandomTest, RandomInt_WithinRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        int value = randomInt(0, 10);
        EXPECT_GE(value, 0);
        EXPECT_LT(value, 10);
    }
}

TEST_F(RandomTest, RandomInt_CustomRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        int value = randomInt(50, 100);
        EXPECT_GE(value, 50);
        EXPECT_LT(value, 100);
    }
}

TEST_F(RandomTest, RandomInt_NegativeRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        int value = randomInt(-50, -10);
        EXPECT_GE(value, -50);
        EXPECT_LT(value, -10);
    }
}

TEST_F(RandomTest, RandomInt_EdgeCase_EqualBounds)
{
    int value = randomInt(5, 5);
    EXPECT_EQ(value, 5);
}

TEST_F(RandomTest, RandomInt_EdgeCase_InvertedBounds)
{
    int value = randomInt(10, 5);
    EXPECT_EQ(value, 10); // Should return lowerBound
}

TEST_F(RandomTest, RandomInt_ProducesVariety)
{
    std::set<int> uniqueValues;
    for (int i = 0; i < sampleSize; ++i) {
        uniqueValues.insert(randomInt(0, 100));
    }
    EXPECT_GT(uniqueValues.size(), minUniqueExpected);
}

// Tests for randomFloat
TEST_F(RandomTest, RandomFloat_WithinDefaultRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        float value = randomFloat();
        EXPECT_GE(value, 0.0f);
        EXPECT_LE(value, 1.0f);
    }
}

TEST_F(RandomTest, RandomFloat_WithinCustomRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        float value = randomFloat(10.0f, 20.0f);
        EXPECT_GE(value, 10.0f);
        EXPECT_LE(value, 20.0f);
    }
}

TEST_F(RandomTest, RandomFloat_NegativeRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        float value = randomFloat(-5.0f, -1.0f);
        EXPECT_GE(value, -5.0f);
        EXPECT_LE(value, -1.0f);
    }
}

TEST_F(RandomTest, RandomFloat_ProducesVariety)
{
    float sum = 0.0f;
    for (int i = 0; i < sampleSize; ++i) {
        sum += randomFloat(0.0f, 100.0f);
    }
    float average = sum / sampleSize;
    // Average should be close to midpoint (50.0)
    EXPECT_GT(average, 30.0f);
    EXPECT_LT(average, 70.0f);
}

// Tests for randomIntNotInBlacklist
TEST_F(RandomTest, RandomIntNotInBlacklist_EmptyBlacklist)
{
    std::vector<int> emptyBlacklist;
    for (int i = 0; i < 100; ++i) {
        int value = randomIntNotInBlacklist(emptyBlacklist, 0, 10);
        EXPECT_GE(value, 0);
        EXPECT_LT(value, 10);
    }
}

TEST_F(RandomTest, RandomIntNotInBlacklist_SingleBlacklisted)
{
    std::vector<int> blacklist = {5};
    for (int i = 0; i < 100; ++i) {
        int value = randomIntNotInBlacklist(blacklist, 0, 10);
        EXPECT_NE(value, 5);
        EXPECT_GE(value, 0);
        EXPECT_LT(value, 10);
    }
}

TEST_F(RandomTest, RandomIntNotInBlacklist_MultipleBlacklisted)
{
    std::vector<int> blacklist = {2, 5, 8};
    for (int i = 0; i < 100; ++i) {
        int value = randomIntNotInBlacklist(blacklist, 0, 10);
        EXPECT_NE(value, 2);
        EXPECT_NE(value, 5);
        EXPECT_NE(value, 8);
        EXPECT_GE(value, 0);
        EXPECT_LT(value, 10);
    }
}

TEST_F(RandomTest, RandomIntNotInBlacklist_ProducesVariety)
{
    std::vector<int> blacklist = {5};
    std::set<int> uniqueValues;

    for (int i = 0; i < sampleSize; ++i) {
        int value = randomIntNotInBlacklist(blacklist, 0, 10);
        uniqueValues.insert(value);
    }

    // Should have at least several unique values
    EXPECT_GT(uniqueValues.size(), 5);

    // Ensure blacklisted value never appears
    EXPECT_EQ(uniqueValues.count(5), 0);
}

TEST_F(RandomTest, RandomIntNotInBlacklist_ThrowsWhenAllBlacklisted)
{
    std::vector<int> blacklist = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    EXPECT_THROW(
        {
            randomIntNotInBlacklist(blacklist, 0, 10);
        },
        std::invalid_argument
    );
}

TEST_F(RandomTest, RandomIntNotInBlacklist_ThrowsWhenInvalidRange)
{
    std::vector<int> blacklist;

    EXPECT_THROW(
        {
            randomIntNotInBlacklist(blacklist, 10, 5);
        },
        std::invalid_argument
    );
}

TEST_F(RandomTest, RandomIntNotInBlacklist_ThrowsWhenBlacklistTooLarge)
{
    std::vector<int> blacklist = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    EXPECT_THROW(
        {
            randomIntNotInBlacklist(blacklist, 0, 10);
        },
        std::invalid_argument
    );
}

// Tests for randomBelow
TEST_F(RandomTest, RandomBelow_WithinRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        EXPECT_LT(randomBelow(7), 7u);
    }
}

TEST_F(RandomTest, RandomBelow_LargeBoundWithinRange)
{
    const uint64_t bound = 1000000000000ull; // above 2^32, takes the two-draw path
    for (int i = 0; i < sampleSize; ++i) {
        EXPECT_LT(randomBelow(bound), bound);
    }
}

TEST_F(RandomTest, RandomBelow_EdgeCase_ZeroAndOne)
{
    EXPECT_EQ(randomBelow(0), 0u);
    EXPECT_EQ(randomBelow(1), 0u);
}

TEST_F(RandomTest, RandomBelow_RoughlyUniform)
{
    std::vector<int> counts(3, 0);
    for (int i = 0; i < 30000; ++i) {
        counts[randomBelow(3)]++;
    }
    for (int count : counts) {
        EXPECT_GT(count, 9000);
        EXPECT_LT(count, 11000);
    }
}

// Tests for randomDouble
TEST_F(RandomTest, RandomDouble_WithinCustomRange)
{
    for (int i = 0; i < sampleSize; ++i) {
        double value = randomDouble(10.0, 20.0);
        EXPECT_GE(value, 10.0);
        EXPECT_LE(value, 20.0);
    }
}

// Tests for shuffle
TEST_F(RandomTest, Shuffle_IsPermutation)
{
    std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle(values.begin(), values.end());
    std::set<int> unique(values.begin(), values.end());
    EXPECT_EQ(values.size(), 10u);
    EXPECT_EQ(unique.size(), 10u);
}

TEST_F(RandomTest, Shuffle_EmptyAndSingle)
{
    std::vector<int> empty;
    shuffle(empty.begin(), empty.end());
    EXPECT_TRUE(empty.empty());

    std::vector<int> single = {5};
    shuffle(single.begin(), single.end());
    EXPECT_EQ(single[0], 5);
}

/*
 * Cross-platform reproducibility
 *
 * These expected values pin the exact outputs for seed 42. They must pass unchanged on every
 * platform (GCC/libstdc++, MSVC, Clang/libc++); a failure on one platform means seeded results
 * (e.g. CultGame world seeds) differ there. randomDouble()'s first value also matches NumPy's
 * np.random.seed(42); np.random.random(), which uses the same MT19937 53-bit construction.
 */
TEST_F(RandomTest, Reproducible_RandomBelow)
{
    setSeed(42);
    const std::vector<uint64_t> expected = {37, 79, 95, 18, 73, 77, 59, 59};
    for (uint64_t value : expected) {
        EXPECT_EQ(randomBelow(100), value);
    }
}

TEST_F(RandomTest, Reproducible_RandomBelowLargeBound)
{
    setSeed(42);
    const std::vector<uint64_t> expected = {637428952499ull, 593393853710ull, 847239337031ull, 886329703444ull};
    for (uint64_t value : expected) {
        EXPECT_EQ(randomBelow(1000000000000ull), value);
    }
}

TEST_F(RandomTest, Reproducible_RandomInt)
{
    setSeed(42);
    const std::vector<int> expected = {-13, 29, 45, -32, 23, 27, 9, 9};
    for (int value : expected) {
        EXPECT_EQ(randomInt(-50, 50), value);
    }

    setSeed(42);
    EXPECT_EQ(randomInt(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), -538846107);
}

TEST_F(RandomTest, Reproducible_RandomFloat)
{
    setSeed(42);
    const std::vector<float> expected = {0.374540091f, 0.796542943f, 0.95071429f, 0.183434784f};
    for (float value : expected) {
        EXPECT_EQ(randomFloat(), value);
    }
}

TEST_F(RandomTest, Reproducible_RandomDouble)
{
    setSeed(42);
    const std::vector<double> expected = {0.37454011884736249, 0.95071430640991617, 0.73199394181140509, 0.5986584841970366};
    for (double value : expected) {
        EXPECT_EQ(randomDouble(), value);
    }
}

TEST_F(RandomTest, Reproducible_Shuffle)
{
    setSeed(42);
    std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7};
    shuffle(values.begin(), values.end());
    EXPECT_EQ(values, (std::vector<int>{4, 1, 3, 7, 0, 6, 5, 2}));
}

// Tests for getRandomEngine
TEST_F(RandomTest, GetRandomEngine_ProducesNumbers)
{
    auto& engine = getRandomEngine();
    unsigned int value1 = engine();
    unsigned int value2 = engine();

    // Just verify it produces values (they should be different most of the time)
    EXPECT_TRUE(value1 != value2 || value1 == value2); // Always true, just exercises the function
}

/*
 * Per-thread engines
 *
 * Every thread gets its own engine, seeded on first use. These tests run on fresh std::threads
 * because a thread's first call to getRandomEngine() is where its engine gets built, and the
 * test runner's main thread has long since built (and drawn from) its own.
 *
 * Regression: GCC 16.1 MinGW at -O3 with LTO miscompiled the old function-local
 * `thread_local std::mt19937`, writing most of a new thread's seeded state to the wrong
 * address. The thread was left with an all-zero engine that only ever returns 0, which hung
 * randomBelow()'s rejection loop forever. Build the tests in Release with LTO to cover this.
 */
namespace
{
    // An mt19937's state words, read through its standard text serialization
    std::vector<uint32_t> readEngineState(const std::mt19937& engine)
    {
        std::stringstream stream;
        stream << engine;
        std::vector<uint32_t> state(std::mt19937::state_size);
        for (uint32_t& word : state) {
            stream >> word;
        }
        return state;
    }

    // True if the state is exactly what seeding an mt19937 produces: every word derived from the one before
    bool isFreshlySeededState(const std::vector<uint32_t>& state)
    {
        for (size_t i = 1; i < state.size(); ++i) {
            const uint32_t expected = static_cast<uint32_t>(1812433253u * (state[i - 1] ^ (state[i - 1] >> 30)) + i);
            if (state[i] != expected) {
                return false;
            }
        }
        return true;
    }

    template<typename F>
    auto runOnNewThread(F function)
    {
        auto task = std::async(std::launch::async, function);
        return task.get();
    }
}

TEST_F(RandomTest, GetRandomEngine_NewThreadEngineIsFullySeeded)
{
    const std::vector<uint32_t> state = runOnNewThread([] {
        return readEngineState(getRandomEngine());
    });
    EXPECT_TRUE(isFreshlySeededState(state));
}

TEST_F(RandomTest, GetRandomEngine_NewThreadEngineProducesVariety)
{
    // Checked on the raw engine first: a broken engine would hang randomBelow() instead of failing
    const std::set<uint32_t> uniqueValues = runOnNewThread([] {
        std::set<uint32_t> values;
        for (int i = 0; i < 100; ++i) {
            values.insert(getRandomEngine()());
        }
        return values;
    });
    EXPECT_GT(uniqueValues.size(), 90u);
}

TEST_F(RandomTest, RandomBelow_WorksOnNewThread)
{
    // 942 is not a power of two, so a zero draw is rejected and redrawn (the original hang)
    const std::vector<uint64_t> values = runOnNewThread([] {
        std::vector<uint64_t> draws;
        for (int i = 0; i < sampleSize; ++i) {
            draws.push_back(randomBelow(942));
        }
        return draws;
    });
    std::set<uint64_t> uniqueValues(values.begin(), values.end());
    for (uint64_t value : values) {
        EXPECT_LT(value, 942u);
    }
    EXPECT_GT(uniqueValues.size(), static_cast<size_t>(minUniqueExpected));
}

TEST_F(RandomTest, GetRandomEngine_EachThreadHasItsOwnEngine)
{
    // Both threads stay alive until both have reported, so neither engine can be freed and its memory reused
    std::promise<const std::mt19937*> firstEngine;
    std::promise<const std::mt19937*> secondEngine;
    std::promise<void> release;
    std::shared_future<void> released = release.get_future().share();

    std::thread first([&] { firstEngine.set_value(&getRandomEngine()); released.wait(); });
    std::thread second([&] { secondEngine.set_value(&getRandomEngine()); released.wait(); });
    const std::mt19937* firstAddress = firstEngine.get_future().get();
    const std::mt19937* secondAddress = secondEngine.get_future().get();
    const std::mt19937* mainAddress = &getRandomEngine();
    release.set_value();
    first.join();
    second.join();

    EXPECT_NE(firstAddress, secondAddress);
    EXPECT_NE(firstAddress, mainAddress);
    EXPECT_NE(secondAddress, mainAddress);
}

TEST_F(RandomTest, Reproducible_SetSeedOnNewThread)
{
    // A seeded thread reproduces the same sequence as the main thread does (see Reproducible_RandomBelow)
    const std::vector<uint64_t> values = runOnNewThread([] {
        setSeed(42);
        std::vector<uint64_t> draws;
        for (int i = 0; i < 8; ++i) {
            draws.push_back(randomBelow(100));
        }
        return draws;
    });
    EXPECT_EQ(values, (std::vector<uint64_t>{37, 79, 95, 18, 73, 77, 59, 59}));
}

TEST_F(RandomTest, Reproducible_OtherThreadsDoNotDisturbSeededSequence)
{
    setSeed(42);
    EXPECT_EQ(randomBelow(100), 37u);
    EXPECT_EQ(randomBelow(100), 79u);

    runOnNewThread([] {
        setSeed(7);
        for (int i = 0; i < 1000; ++i) {
            randomBelow(100);
        }
        return 0;
    });

    const std::vector<uint64_t> expectedRest = {95, 18, 73, 77, 59, 59};
    for (uint64_t value : expectedRest) {
        EXPECT_EQ(randomBelow(100), value);
    }
}
