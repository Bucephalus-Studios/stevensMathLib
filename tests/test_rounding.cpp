/**
 * test_rounding.cpp
 *
 * Unit tests for rounding functions in stevensMathLib
 */

#include "../stevensMathLib.h"
#include <gtest/gtest.h>
#include <cmath>

using namespace stevensMathLib;

// Test fixture for rounding tests
class RoundingTest : public ::testing::Test
{
protected:
    const float epsilon = 0.0001f;

    bool floatsEqual(float a, float b)
    {
        return std::abs(a - b) < epsilon;
    }
};

// Tests for isWholeNumber
TEST_F(RoundingTest, IsWholeNumber_WithInteger)
{
    EXPECT_TRUE(isWholeNumber(5.0f));
    EXPECT_TRUE(isWholeNumber(0.0f));
    EXPECT_TRUE(isWholeNumber(-10.0f));
}

TEST_F(RoundingTest, IsWholeNumber_WithDecimal)
{
    EXPECT_FALSE(isWholeNumber(5.5f));
    EXPECT_FALSE(isWholeNumber(0.1f));
    EXPECT_FALSE(isWholeNumber(-10.3f));
}

// Tests for roundToNearest10th
TEST_F(RoundingTest, RoundToNearest10th_WholeNumber)
{
    EXPECT_FLOAT_EQ(roundToNearest10th(5.0f), 5.0f);
    EXPECT_FLOAT_EQ(roundToNearest10th(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(roundToNearest10th(-10.0f), -10.0f);
}

TEST_F(RoundingTest, RoundToNearest10th_RoundsToWhole)
{
    EXPECT_FLOAT_EQ(roundToNearest10th(4.96f), 5.0f);
    EXPECT_FLOAT_EQ(roundToNearest10th(10.04f), 10.0f);
}

TEST_F(RoundingTest, RoundToNearest10th_RoundsToTenth)
{
    EXPECT_TRUE(floatsEqual(roundToNearest10th(3.14f), 3.1f));
    EXPECT_TRUE(floatsEqual(roundToNearest10th(3.16f), 3.2f));
    EXPECT_TRUE(floatsEqual(roundToNearest10th(-2.47f), -2.5f));
}

TEST_F(RoundingTest, RoundToNearest10th_AlreadyRounded)
{
    EXPECT_TRUE(floatsEqual(roundToNearest10th(3.1f), 3.1f));
    EXPECT_TRUE(floatsEqual(roundToNearest10th(7.5f), 7.5f));
}

// Tests for round function with precision
TEST_F(RoundingTest, Round_ZeroDecimalPlaces)
{
    EXPECT_FLOAT_EQ(round(3.14159f, 0), 3.0f);
    EXPECT_FLOAT_EQ(round(3.7f, 0), 4.0f);
    EXPECT_FLOAT_EQ(round(-2.3f, 0), -2.0f);
}

TEST_F(RoundingTest, Round_OneDecimalPlace)
{
    EXPECT_TRUE(floatsEqual(round(3.14159f, 1), 3.1f));
    EXPECT_TRUE(floatsEqual(round(3.16f, 1), 3.2f));
}

TEST_F(RoundingTest, Round_TwoDecimalPlaces)
{
    EXPECT_TRUE(floatsEqual(round(3.14159f, 2), 3.14f));
    EXPECT_TRUE(floatsEqual(round(2.718f, 2), 2.72f));
}

TEST_F(RoundingTest, Round_ThreeDecimalPlaces)
{
    EXPECT_TRUE(floatsEqual(round(3.14159f, 3), 3.142f));
    EXPECT_TRUE(floatsEqual(round(1.99999f, 3), 2.0f));
}

TEST_F(RoundingTest, Round_NegativeNumbers)
{
    EXPECT_TRUE(floatsEqual(round(-3.14159f, 2), -3.14f));
    EXPECT_TRUE(floatsEqual(round(-2.718f, 1), -2.7f));
}

TEST_F(RoundingTest, Round_NegativePrecision)
{
    // Should handle gracefully by using absolute value
    EXPECT_TRUE(floatsEqual(round(3.14159f, -2), 3.14f));
}

TEST_F(RoundingTest, Round_LargePrecision)
{
    EXPECT_TRUE(floatsEqual(round(3.14159f, 5), 3.14159f));
}

// Tests for roundToInt
TEST_F(RoundingTest, RoundToInt_NearestByDefault)
{
    EXPECT_EQ(roundToInt(3.4f), 3);
    EXPECT_EQ(roundToInt(3.6f), 4);
    EXPECT_EQ(roundToInt(-3.4f), -3);
    EXPECT_EQ(roundToInt(-3.6f), -4);
    EXPECT_EQ(roundToInt(7.0f), 7);
}

TEST_F(RoundingTest, RoundToInt_NearestHalfwayAwayFromZero)
{
    EXPECT_EQ(roundToInt(2.5f), 3);
    EXPECT_EQ(roundToInt(-2.5f), -3);
    EXPECT_EQ(roundToInt(0.5f), 1);
}

TEST_F(RoundingTest, RoundToInt_NearestRecoversWholeNumbersStoredJustBelow)
{
    // Dividing first stores 53/100 as 0.52999997f, so the percentage lands just below 53 -- a cast gives 52
    const float percent = (static_cast<float>(53) / 100) * 100.0f;
    EXPECT_LT(percent, 53.0f);
    EXPECT_EQ(static_cast<int>(percent), 52);
    EXPECT_EQ(roundToInt(percent), 53);

    // Every hit count out of 100 comes back as itself
    for (int hits = 0; hits <= 100; ++hits)
    {
        EXPECT_EQ(roundToInt((static_cast<float>(hits) / 100) * 100.0f), hits) << "hits = " << hits;
    }
}

TEST_F(RoundingTest, RoundToInt_Up)
{
    EXPECT_EQ(roundToInt(2.1f, RoundingMode::Up), 3);
    EXPECT_EQ(roundToInt(2.0f, RoundingMode::Up), 2);
    EXPECT_EQ(roundToInt(-2.9f, RoundingMode::Up), -2);
}

TEST_F(RoundingTest, RoundToInt_Down)
{
    EXPECT_EQ(roundToInt(2.9f, RoundingMode::Down), 2);
    EXPECT_EQ(roundToInt(2.0f, RoundingMode::Down), 2);
    EXPECT_EQ(roundToInt(-2.1f, RoundingMode::Down), -3);
}
