/**
 * test_conversion.cpp
 *
 * Unit tests for type conversion functions in stevensMathLib
 */

#include "../stevensMathLib.h"
#include <gtest/gtest.h>
#include <limits>

using namespace stevensMathLib;

// Test fixture for conversion tests
class ConversionTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Redirect cerr to suppress warning messages during tests
        originalCerr = std::cerr.rdbuf();
        std::cerr.rdbuf(nullptr);
    }

    void TearDown() override
    {
        // Restore cerr
        std::cerr.rdbuf(originalCerr);
    }

private:
    std::streambuf* originalCerr;
};

// Tests for floatToInt
TEST_F(ConversionTest, FloatToInt_PositiveFloat)
{
    EXPECT_EQ(floatToInt(3.14f), 3);
    EXPECT_EQ(floatToInt(5.9f), 5);
    EXPECT_EQ(floatToInt(0.5f), 0);
}

TEST_F(ConversionTest, FloatToInt_NegativeFloat)
{
    EXPECT_EQ(floatToInt(-3.14f), -3);
    EXPECT_EQ(floatToInt(-5.9f), -5);
    EXPECT_EQ(floatToInt(-0.5f), 0);
}

TEST_F(ConversionTest, FloatToInt_Zero)
{
    EXPECT_EQ(floatToInt(0.0f), 0);
}

TEST_F(ConversionTest, FloatToInt_WholeNumber)
{
    EXPECT_EQ(floatToInt(10.0f), 10);
    EXPECT_EQ(floatToInt(-25.0f), -25);
}

TEST_F(ConversionTest, FloatToInt_Double)
{
    EXPECT_EQ(floatToInt(3.14159), 3);
    EXPECT_EQ(floatToInt(100.999), 100);
}

TEST_F(ConversionTest, FloatToInt_LongDouble)
{
    EXPECT_EQ(floatToInt(3.14159L), 3);
    EXPECT_EQ(floatToInt(-7.5L), -7);
}

TEST_F(ConversionTest, FloatToInt_LargeValidValue)
{
    float largeValue = 1000000.0f;
    EXPECT_EQ(floatToInt(largeValue), 1000000);
}

TEST_F(ConversionTest, FloatToInt_OverflowReturnsMax)
{
    // Test value larger than int max
    double hugeValue = static_cast<double>(std::numeric_limits<int>::max()) * 2.0;
    EXPECT_EQ(floatToInt(hugeValue), std::numeric_limits<int>::max());
}

TEST_F(ConversionTest, FloatToInt_UnderflowReturnsMax)
{
    // Test value smaller than int min
    double tinyValue = static_cast<double>(std::numeric_limits<int>::min()) * 2.0;
    EXPECT_EQ(floatToInt(tinyValue), std::numeric_limits<int>::max());
}

TEST_F(ConversionTest, FloatToInt_NearIntMax)
{
    int maxInt = std::numeric_limits<int>::max();
    float nearMax = static_cast<float>(maxInt) * 0.9f;
    int result = floatToInt(nearMax);
    EXPECT_LT(result, maxInt);
    EXPECT_GT(result, 0);
}

TEST_F(ConversionTest, FloatToInt_NearIntMin)
{
    int minInt = std::numeric_limits<int>::min();
    float nearMin = static_cast<float>(minInt) * 0.9f;
    int result = floatToInt(nearMin);
    EXPECT_GT(result, minInt);
    EXPECT_LT(result, 0);
}
