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
