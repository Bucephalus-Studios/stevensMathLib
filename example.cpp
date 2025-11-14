/**
 * example.cpp
 *
 * Example usage of the stevensMathLib library
 *
 * Compile with:
 *   g++ -std=c++17 example.cpp -o example
 *   ./example
 */

#include "stevensMathLib.h"
#include <iostream>
#include <iomanip>

using namespace stevensMathLib;

void demonstrateRounding()
{
    std::cout << "=== Rounding Functions ===\n";

    float pi = 3.14159f;
    std::cout << "Original: " << pi << "\n";
    std::cout << "roundToNearest10th: " << roundToNearest10th(pi) << "\n";
    std::cout << "round(pi, 2): " << round(pi, 2) << "\n";
    std::cout << "round(pi, 4): " << round(pi, 4) << "\n";

    float wholeNum = 10.0f;
    std::cout << "\nWhole number " << wholeNum << ":\n";
    std::cout << "isWholeNumber: " << (isWholeNumber(wholeNum) ? "true" : "false") << "\n";
    std::cout << "roundToNearest10th: " << roundToNearest10th(wholeNum) << "\n\n";
}

void demonstrateRandomNumbers()
{
    std::cout << "=== Random Number Generation ===\n";

    std::cout << "5 random integers [0, 100):\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  " << randomInt(0, 100) << "\n";
    }

    std::cout << "\n5 random floats [0.0, 1.0]:\n";
    std::cout << std::fixed << std::setprecision(6);
    for (int i = 0; i < 5; ++i) {
        std::cout << "  " << randomFloat(0.0f, 1.0f) << "\n";
    }

    std::cout << "\n5 random integers [10, 20) excluding {12, 15, 18}:\n";
    std::vector<int> blacklist = {12, 15, 18};
    for (int i = 0; i < 5; ++i) {
        std::cout << "  " << randomIntNotInBlacklist(blacklist, 10, 20) << "\n";
    }
    std::cout << std::defaultfloat << "\n";
}

void demonstrateTypeConversion()
{
    std::cout << "=== Type Conversion ===\n";

    float f1 = 3.7f;
    float f2 = -5.2f;
    double d1 = 42.9;

    std::cout << "floatToInt(" << f1 << ") = " << floatToInt(f1) << "\n";
    std::cout << "floatToInt(" << f2 << ") = " << floatToInt(f2) << "\n";
    std::cout << "floatToInt(" << d1 << ") = " << floatToInt(d1) << "\n\n";
}

void demonstrateRangeChecking()
{
    std::cout << "=== Range Checking ===\n";

    int value = 5;
    int lower = 0;
    int upper = 10;

    std::cout << "Value: " << value << ", Range: [" << lower << ", " << upper << "]\n";
    std::cout << "in_range (Inclusive): "
              << (in_range(value, lower, upper, BoundType::Inclusive) ? "true" : "false") << "\n";
    std::cout << "in_range (Exclusive): "
              << (in_range(value, lower, upper, BoundType::Exclusive) ? "true" : "false") << "\n";

    int boundary = 10;
    std::cout << "\nBoundary value: " << boundary << ", Range: [" << lower << ", " << upper << "]\n";
    std::cout << "in_range (Inclusive): "
              << (in_range(boundary, lower, upper, BoundType::Inclusive) ? "true" : "false") << "\n";
    std::cout << "in_range (Exclusive): "
              << (in_range(boundary, lower, upper, BoundType::Exclusive) ? "true" : "false") << "\n";

    float floatVal = 3.14f;
    float floatLower = 0.0f;
    float floatUpper = 5.0f;
    std::cout << "\nFloat value: " << floatVal << ", Range: ["
              << floatLower << ", " << floatUpper << "]\n";
    std::cout << "in_range (Inclusive): "
              << (in_range(floatVal, floatLower, floatUpper, BoundType::Inclusive) ? "true" : "false")
              << "\n\n";
}

void demonstrateModernRandom()
{
    std::cout << "=== Modern Random Engine ===\n";

    auto& engine = getRandomEngine();
    std::cout << "Raw engine outputs:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  " << engine() << "\n";
    }
    std::cout << "\n";
}

int main()
{
    std::cout << "stevensMathLib - Example Usage\n";
    std::cout << "===============================\n\n";

    demonstrateRounding();
    demonstrateRandomNumbers();
    demonstrateTypeConversion();
    demonstrateRangeChecking();
    demonstrateModernRandom();

    std::cout << "Example completed successfully!\n";
    return 0;
}
