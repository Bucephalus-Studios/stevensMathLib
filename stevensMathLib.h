/**
 * stevensMathLib.h
 *
 * A modern C++ library containing useful functions for numerical operations,
 * random number generation, and range checking.
 *
 * Copyright (c) 2025 Bucephalus-Studios
 * Licensed under the MIT License
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

namespace stevensMathLib
{
    /**
     * @brief Specifies whether range bounds are inclusive or exclusive
     */
    enum class BoundType
    {
        Inclusive,  // Uses >= and <= comparisons
        Exclusive   // Uses > and < comparisons
    };

    /**
     * @brief Seeds the legacy random number generator
     *
     * @deprecated Use getRandomEngine() for modern C++ random number generation
     *
     * This function seeds the C-style rand() function with the current time.
     * For new code, prefer using the modern C++ <random> library.
     */
    inline void seedRNG()
    {
        srand(static_cast<unsigned>(time(nullptr)));
    }

    /**
     * @brief Returns a thread-local random number engine
     *
     * @return A reference to a mt19937 random engine, seeded with high-resolution clock
     *
     * This provides a modern, high-quality random number generator using
     * the Mersenne Twister algorithm. The engine is thread-local to avoid
     * race conditions in multithreaded code.
     *
     * Uses high-resolution clock for seeding to ensure each std::thread gets a unique
     * seed while avoiding potential entropy exhaustion from repeated random_device usage.
     */
    inline std::mt19937& getRandomEngine()
    {
        thread_local std::mt19937 engine(
            static_cast<unsigned int>(
                std::chrono::high_resolution_clock::now().time_since_epoch().count()
            )
        );
        return engine;
    }

    /**
     * @brief Re-seeds the random engine for reproducible generation (e.g. world seeds)
     */
    inline void setSeed(unsigned int seed)
    {
        getRandomEngine().seed(seed);
    }

    /**
     * @brief Checks if a value has no fractional part
     *
     * @param value The value to check
     * @return true if the value equals its floor (no fractional part)
     */
    inline bool isWholeNumber(float value)
    {
        return std::floor(value) == value;
    }

    /**
     * @brief Rounds a number to the nearest tenth
     *
     * @param numberToRound The number to round
     * @return The number rounded to one decimal place, or as an integer if whole
     *
     * If the number has no decimal part (or rounds to a whole number),
     * returns it as an integer. Otherwise, rounds to the nearest tenth.
     */
    inline float roundToNearest10th(float numberToRound)
    {
        if (isWholeNumber(numberToRound)) {
            return static_cast<int>(numberToRound);
        }

        const float roundedValue = std::round(numberToRound * 10.0f) / 10.0f;

        if (isWholeNumber(roundedValue)) {
            return static_cast<int>(roundedValue);
        }

        return roundedValue;
    }

    /**
     * @brief Rounds a number to a specified precision
     *
     * @param value The value to round
     * @param decimalPlaces The number of decimal places to round to
     * @return The value rounded to the specified decimal places
     *
     * @note Precision must be non-negative. For precision 0, rounds to integer.
     *
     * Example: round(3.14159, 2) returns 3.14
     */
    inline float round(float value, int decimalPlaces)
    {
        if (decimalPlaces < 0) {
            std::cerr << "Warning: negative precision (" << decimalPlaces
                      << ") provided to round(). Using absolute value.\n";
            decimalPlaces = std::abs(decimalPlaces);
        }

        const float scaleFactor = std::pow(10.0f, decimalPlaces);
        return std::round(value * scaleFactor) / scaleFactor;
    }

    /*
     * Portable random ranges
     *
     * std::mt19937 is fully specified by the C++ standard, so a given seed yields the same raw
     * 32-bit outputs everywhere. std::uniform_int_distribution, std::uniform_real_distribution
     * and std::shuffle are NOT: libstdc++ (GCC/Linux), the MSVC STL (Windows) and libc++
     * (Clang/macOS) each map raw outputs to a range differently, and may consume a different
     * number of them per call. So the same seed produced different results per platform.
     *
     * The functions below build ranges directly from the engine's raw output with fully
     * specified algorithms, so a seed reproduces identically on every platform. Use them
     * instead of the std:: distributions whenever results must be reproducible from a seed.
     */

    /**
     * @brief Draws one raw 32-bit value from the engine
     *
     * mt19937's result_type is uint_fast32_t, which is 64 bits wide on some platforms, but its
     * values always fit in 32 bits.
     */
    inline uint32_t randomBits32()
    {
        return static_cast<uint32_t>(getRandomEngine()());
    }

    /**
     * @brief Generates a uniform random integer in [0, bound), identically on every platform
     *
     * @param bound Exclusive upper bound
     * @return A random integer in [0, bound), or 0 if bound is 0 or 1
     *
     * Bounds that fit in 32 bits use Lemire's multiply-and-reject method (unbiased, usually
     * one draw). Larger bounds combine two draws into 64 bits and use modulo rejection.
     */
    inline uint64_t randomBelow(uint64_t bound)
    {
        if (bound <= 1) {
            return 0;
        }

        if (bound <= std::numeric_limits<uint32_t>::max()) {
            const uint32_t bound32 = static_cast<uint32_t>(bound);
            uint64_t product = static_cast<uint64_t>(randomBits32()) * bound32;
            uint32_t low = static_cast<uint32_t>(product);
            if (low < bound32) {
                // Values whose low half falls below (2^32 mod bound) would bias the result; redraw them
                const uint32_t threshold = static_cast<uint32_t>(0u - bound32) % bound32;
                while (low < threshold) {
                    product = static_cast<uint64_t>(randomBits32()) * bound32;
                    low = static_cast<uint32_t>(product);
                }
            }
            return product >> 32;
        }

        // Accept only draws at or above (2^64 mod bound), so every result is equally likely
        const uint64_t threshold = (0ull - bound) % bound;
        while (true) {
            // Two separate statements: the evaluation order of two calls in one expression is
            // unspecified, so combining them inline could differ between compilers
            const uint64_t high = randomBits32();
            const uint64_t low = randomBits32();
            const uint64_t value = (high << 32) | low;
            if (value >= threshold) {
                return value % bound;
            }
        }
    }

    /**
     * @brief Generates a random floating point number in a range, identically on every platform
     *
     * @param lowerBound Inclusive lower bound (minimum value)
     * @param upperBound Upper bound (maximum value); exclusive except where rounding lands on it
     * @return A random float between lowerBound and upperBound
     *
     * Takes the top 24 bits of one draw as a fraction in [0, 1), then scales it with std::fma.
     * fma rounds once by definition, so the result can't vary with whether the compiler
     * fuses the multiply and add.
     */
    inline float randomFloat(float lowerBound = 0.0f, float upperBound = 1.0f)
    {
        const float fraction = static_cast<float>(randomBits32() >> 8) * (1.0f / 16777216.0f);
        return std::fma(fraction, upperBound - lowerBound, lowerBound);
    }

    /**
     * @brief Generates a random double in a range, identically on every platform
     *
     * @param lowerBound Inclusive lower bound (minimum value)
     * @param upperBound Upper bound (maximum value); exclusive except where rounding lands on it
     * @return A random double between lowerBound and upperBound
     *
     * Combines two draws into a 53-bit fraction in [0, 1), then scales it with std::fma
     * (see randomFloat()).
     */
    inline double randomDouble(double lowerBound = 0.0, double upperBound = 1.0)
    {
        const uint64_t high = randomBits32() >> 5; // 27 bits
        const uint64_t low = randomBits32() >> 6;  // 26 bits
        const double fraction = static_cast<double>((high << 26) | low) * (1.0 / 9007199254740992.0);
        return std::fma(fraction, upperBound - lowerBound, lowerBound);
    }

    /**
     * @brief Generates a random integer in a range, identically on every platform
     *
     * @param lowerBound Inclusive lower bound (minimum value)
     * @param upperBound Exclusive upper bound (one past maximum value)
     * @return A random integer in [lowerBound, upperBound)
     *
     * If upperBound <= lowerBound, returns lowerBound.
     */
    inline int randomInt(int lowerBound = 0, int upperBound = 100)
    {
        if (upperBound <= lowerBound) {
            return lowerBound;
        }

        const uint64_t range = static_cast<uint64_t>(static_cast<int64_t>(upperBound) - lowerBound);
        return static_cast<int>(static_cast<int64_t>(lowerBound) + static_cast<int64_t>(randomBelow(range)));
    }

    /**
     * @brief Shuffles a range into a random order, identically on every platform
     *
     * @param first Iterator to the first element
     * @param last Iterator one past the last element
     *
     * Fisher-Yates shuffle drawing from randomBelow(). Use instead of std::shuffle, whose
     * algorithm is implementation-defined.
     */
    template<typename RandomIt>
    void shuffle(RandomIt first, RandomIt last)
    {
        const auto count = std::distance(first, last);
        for (auto i = count - 1; i > 0; --i) {
            const auto j = static_cast<decltype(i)>(randomBelow(static_cast<uint64_t>(i) + 1));
            using std::swap;
            swap(first[i], first[j]);
        }
    }

    /**
     * @brief Generates a random integer not in a blacklist
     *
     * @param blacklist Vector of integers to exclude
     * @param lowerBound Inclusive lower bound
     * @param upperBound Exclusive upper bound
     * @return A random integer in range that's not in the blacklist
     *
     * @throws std::invalid_argument if all integers in range are blacklisted
     *
     * This function uses a rejection sampling approach: it generates random
     * integers until finding one not in the blacklist. This is efficient
     * when the blacklist is small relative to the range.
     */
    inline int randomIntNotInBlacklist(const std::vector<int>& blacklist,
                                       int lowerBound = 0,
                                       int upperBound = 100)
    {
        const int rangeSize = upperBound - lowerBound;

        if (rangeSize <= 0) {
            throw std::invalid_argument(
                "stevensMathLib::randomIntNotInBlacklist - upperBound must be greater than lowerBound");
        }

        if (static_cast<int>(blacklist.size()) >= rangeSize) {
            throw std::invalid_argument(
                "stevensMathLib::randomIntNotInBlacklist - blacklist size >= available range");
        }

        // Rejection sampling: keep generating until we find a non-blacklisted value
        const int maxAttempts = rangeSize * 10;
        for (int attempt = 0; attempt < maxAttempts; ++attempt) {
            const int candidate = randomInt(lowerBound, upperBound);

            const bool isBlacklisted = std::find(blacklist.begin(), blacklist.end(),
                                                 candidate) != blacklist.end();
            if (!isBlacklisted) {
                return candidate;
            }
        }

        // Fallback: build valid set if rejection sampling fails
        std::vector<int> validNumbers;
        validNumbers.reserve(rangeSize - blacklist.size());

        for (int i = lowerBound; i < upperBound; ++i) {
            const bool isBlacklisted = std::find(blacklist.begin(), blacklist.end(),
                                                 i) != blacklist.end();
            if (!isBlacklisted) {
                validNumbers.push_back(i);
            }
        }

        if (validNumbers.empty()) {
            throw std::invalid_argument(
                "stevensMathLib::randomIntNotInBlacklist - no valid integers remain after blacklist");
        }

        const int randomIndex = randomInt(0, static_cast<int>(validNumbers.size()));
        return validNumbers[randomIndex];
    }

    /**
     * @brief Safely converts a floating point number to an integer
     *
     * @tparam FloatType The floating point type (float, double, long double)
     * @param value The floating point value to convert
     * @return The value converted to int, or int::max() if out of range
     *
     * This function checks if the float value fits within the int range
     * before conversion. If out of range, it prints a warning and returns
     * the maximum int value.
     */
    template <typename FloatType>
    int floatToInt(const FloatType& value)
    {
        const bool floatHasMorePrecision =
            std::numeric_limits<int>::digits < std::numeric_limits<FloatType>::digits;

        if (!floatHasMorePrecision) {
            return static_cast<int>(value);
        }

        const FloatType maxIntAsFloat = static_cast<FloatType>(std::numeric_limits<int>::max());
        const FloatType minIntAsFloat = static_cast<FloatType>(std::numeric_limits<int>::min());

        const bool fitsInInt = (value < maxIntAsFloat) && (value > minIntAsFloat);

        if (fitsInInt) {
            return static_cast<int>(value);
        }

        std::cerr << "Warning: unsafe conversion of value " << value
                  << " to int. Returning int::max().\n";
        return std::numeric_limits<int>::max();
    }

    /**
     * @brief Checks if a value falls within a specified range
     *
     * @tparam T The numeric type (must support comparison operators)
     * @param value The value to check
     * @param lowerBound The lower bound of the range
     * @param upperBound The upper bound of the range
     * @param boundType Whether bounds are inclusive or exclusive
     * @return true if value is within the range, false otherwise
     *
     * Example:
     *   in_range(5, 0, 10, BoundType::Inclusive)  -> true
     *   in_range(10, 0, 10, BoundType::Exclusive) -> false
     */
    template <typename T>
    bool in_range(const T& value,
                  const T& lowerBound,
                  const T& upperBound,
                  BoundType boundType = BoundType::Inclusive)
    {
        if (boundType == BoundType::Inclusive) {
            return (value >= lowerBound) && (value <= upperBound);
        }

        return (value > lowerBound) && (value < upperBound);
    }

} // namespace stevensMathLib

