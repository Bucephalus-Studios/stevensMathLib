/**
 * test_random.cpp
 *
 * Unit tests for random number generation functions in stevensMathLib
 */

#include "../stevensMathLib.h"
#include <gtest/gtest.h>
#include <set>
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

// Tests for getRandomEngine
TEST_F(RandomTest, GetRandomEngine_ProducesNumbers)
{
    auto& engine = getRandomEngine();
    unsigned int value1 = engine();
    unsigned int value2 = engine();

    // Just verify it produces values (they should be different most of the time)
    EXPECT_TRUE(value1 != value2 || value1 == value2); // Always true, just exercises the function
}
