/**
 * test_range.cpp
 *
 * Unit tests for range checking functions in stevensMathLib
 */

#include "../stevensMathLib.h"
#include <gtest/gtest.h>

using namespace stevensMathLib;

// Test fixture for range checking
class RangeTest : public ::testing::Test {};

// Tests for in_range with BoundType::Inclusive
TEST_F(RangeTest, InRange_Inclusive_WithinRange)
{
    EXPECT_TRUE(in_range(5, 0, 10, BoundType::Inclusive));
    EXPECT_TRUE(in_range(0, 0, 10, BoundType::Inclusive));
    EXPECT_TRUE(in_range(10, 0, 10, BoundType::Inclusive));
}

TEST_F(RangeTest, InRange_Inclusive_OutsideRange)
{
    EXPECT_FALSE(in_range(-1, 0, 10, BoundType::Inclusive));
    EXPECT_FALSE(in_range(11, 0, 10, BoundType::Inclusive));
    EXPECT_FALSE(in_range(100, 0, 10, BoundType::Inclusive));
}

TEST_F(RangeTest, InRange_Inclusive_BoundaryValues)
{
    EXPECT_TRUE(in_range(0, 0, 10, BoundType::Inclusive));
    EXPECT_TRUE(in_range(10, 0, 10, BoundType::Inclusive));
}

TEST_F(RangeTest, InRange_Inclusive_NegativeRange)
{
    EXPECT_TRUE(in_range(-5, -10, 0, BoundType::Inclusive));
    EXPECT_TRUE(in_range(-10, -10, 0, BoundType::Inclusive));
    EXPECT_TRUE(in_range(0, -10, 0, BoundType::Inclusive));
    EXPECT_FALSE(in_range(-11, -10, 0, BoundType::Inclusive));
}

TEST_F(RangeTest, InRange_Inclusive_FloatValues)
{
    EXPECT_TRUE(in_range(5.5f, 0.0f, 10.0f, BoundType::Inclusive));
    EXPECT_TRUE(in_range(0.0f, 0.0f, 10.0f, BoundType::Inclusive));
    EXPECT_TRUE(in_range(10.0f, 0.0f, 10.0f, BoundType::Inclusive));
    EXPECT_FALSE(in_range(10.1f, 0.0f, 10.0f, BoundType::Inclusive));
}

TEST_F(RangeTest, InRange_Inclusive_DefaultParameter)
{
    // Test that Inclusive is the default
    EXPECT_TRUE(in_range(0, 0, 10));
    EXPECT_TRUE(in_range(10, 0, 10));
}

// Tests for in_range with BoundType::Exclusive
TEST_F(RangeTest, InRange_Exclusive_WithinRange)
{
    EXPECT_TRUE(in_range(5, 0, 10, BoundType::Exclusive));
    EXPECT_TRUE(in_range(1, 0, 10, BoundType::Exclusive));
    EXPECT_TRUE(in_range(9, 0, 10, BoundType::Exclusive));
}

TEST_F(RangeTest, InRange_Exclusive_OutsideRange)
{
    EXPECT_FALSE(in_range(0, 0, 10, BoundType::Exclusive));
    EXPECT_FALSE(in_range(10, 0, 10, BoundType::Exclusive));
    EXPECT_FALSE(in_range(-1, 0, 10, BoundType::Exclusive));
    EXPECT_FALSE(in_range(11, 0, 10, BoundType::Exclusive));
}

TEST_F(RangeTest, InRange_Exclusive_BoundaryValues)
{
    EXPECT_FALSE(in_range(0, 0, 10, BoundType::Exclusive));
    EXPECT_FALSE(in_range(10, 0, 10, BoundType::Exclusive));
}

TEST_F(RangeTest, InRange_Exclusive_NegativeRange)
{
    EXPECT_TRUE(in_range(-5, -10, 0, BoundType::Exclusive));
    EXPECT_FALSE(in_range(-10, -10, 0, BoundType::Exclusive));
    EXPECT_FALSE(in_range(0, -10, 0, BoundType::Exclusive));
}

TEST_F(RangeTest, InRange_Exclusive_FloatValues)
{
    EXPECT_TRUE(in_range(5.5f, 0.0f, 10.0f, BoundType::Exclusive));
    EXPECT_FALSE(in_range(0.0f, 0.0f, 10.0f, BoundType::Exclusive));
    EXPECT_FALSE(in_range(10.0f, 0.0f, 10.0f, BoundType::Exclusive));
    EXPECT_TRUE(in_range(9.99f, 0.0f, 10.0f, BoundType::Exclusive));
}

// Tests with different numeric types
TEST_F(RangeTest, InRange_DoubleType)
{
    EXPECT_TRUE(in_range(5.5, 0.0, 10.0, BoundType::Inclusive));
    EXPECT_TRUE(in_range(0.0, 0.0, 10.0, BoundType::Inclusive));
    EXPECT_FALSE(in_range(0.0, 0.0, 10.0, BoundType::Exclusive));
}

TEST_F(RangeTest, InRange_LongType)
{
    long value = 5000L;
    long lower = 0L;
    long upper = 10000L;

    EXPECT_TRUE(in_range(value, lower, upper, BoundType::Inclusive));
    EXPECT_TRUE(in_range(lower, lower, upper, BoundType::Inclusive));
    EXPECT_FALSE(in_range(lower, lower, upper, BoundType::Exclusive));
}

TEST_F(RangeTest, InRange_CharType)
{
    EXPECT_TRUE(in_range('m', 'a', 'z', BoundType::Inclusive));
    EXPECT_TRUE(in_range('a', 'a', 'z', BoundType::Inclusive));
    EXPECT_FALSE(in_range('a', 'a', 'z', BoundType::Exclusive));
}

// Edge cases
TEST_F(RangeTest, InRange_SinglePointRange_Inclusive)
{
    EXPECT_TRUE(in_range(5, 5, 5, BoundType::Inclusive));
}

TEST_F(RangeTest, InRange_SinglePointRange_Exclusive)
{
    EXPECT_FALSE(in_range(5, 5, 5, BoundType::Exclusive));
}

TEST_F(RangeTest, InRange_VeryLargeNumbers)
{
    long long huge = 1000000000000LL;
    EXPECT_TRUE(in_range(huge, 0LL, 2000000000000LL, BoundType::Inclusive));
}

TEST_F(RangeTest, InRange_VerySmallFloats)
{
    float tiny = 0.000001f;
    EXPECT_TRUE(in_range(tiny, 0.0f, 0.00001f, BoundType::Inclusive));
    EXPECT_TRUE(in_range(tiny, 0.0f, 0.00001f, BoundType::Exclusive));
}
