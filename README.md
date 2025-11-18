# stevensMathLib

A modern, header-only C++17 library providing utility functions for numerical operations, random number generation, and range checking.

## Features

- **Rounding Functions**: Round numbers to specific decimal places or nearest tenth
- **Random Number Generation**: Modern C++ random number generation using Mersenne Twister
- **Type Conversion**: Safe float-to-integer conversion with bounds checking
- **Range Checking**: Flexible range validation with inclusive/exclusive bounds
- **Header-Only**: Simply include the header file in your project
- **Well-Tested**: Comprehensive unit tests using Google Test
- **Benchmarked**: Performance benchmarks using Google Benchmark

## Refactoring Improvements

This library has been thoroughly refactored with focus on:

✅ **Simplicity**: Clear, straightforward implementations
✅ **Readability**: Comprehensive documentation and meaningful names
✅ **Functional Abstraction**: Well-defined, single-purpose functions
✅ **Encapsulation**: Clean namespace organization
✅ **DRY Code**: No code duplication, extracted helpers
✅ **Efficiency**: Optimized algorithms (e.g., rejection sampling)
✅ **Speed**: Modern C++ random with thread-local engine
✅ **Good Variable Names**: Descriptive, self-documenting identifiers
✅ **Maximum 2 Nesting Levels**: All functions use early returns for clarity

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14 or higher (for building tests and benchmarks)

## Usage

### As a Header-Only Library

Simply include the header in your C++ project:

```cpp
#include "stevensMathLib.h"

using namespace stevensMathLib;

int main() {
    // Rounding
    float pi = 3.14159f;
    float rounded = round(pi, 2);  // 3.14

    // Random numbers
    int randomValue = randomInt(0, 100);  // [0, 100)
    float randomFloat = randomFloat(0.0f, 1.0f);  // [0.0, 1.0]

    // Range checking
    bool inRange = in_range(5, 0, 10, BoundType::Inclusive);  // true

    // Type conversion
    int converted = floatToInt(3.7f);  // 3

    return 0;
}
```

Compile with:
```bash
g++ -std=c++17 your_program.cpp -o your_program
```

### Example Programs

Here are some example usages of the library:

**Rounding Functions:**
```cpp
float pi = 3.14159f;
std::cout << roundToNearest10th(pi) << "\n";  // 3.1
std::cout << round(pi, 2) << "\n";  // 3.14
std::cout << round(pi, 4) << "\n";  // 3.1416
std::cout << isWholeNumber(10.0f) << "\n";  // true
```

**Random Number Generation:**
```cpp
// Random integers in range [0, 100)
int randInt = randomInt(0, 100);

// Random floats in range [0.0, 1.0]
float randFloat = randomFloat(0.0f, 1.0f);

// Random integers excluding a blacklist
std::vector<int> blacklist = {12, 15, 18};
int randExcluded = randomIntNotInBlacklist(blacklist, 10, 20);

// Access the random engine directly
auto& engine = getRandomEngine();
```

**Type Conversion:**
```cpp
float f1 = 3.7f;
float f2 = -5.2f;
double d1 = 42.9;

int i1 = floatToInt(f1);  // 3
int i2 = floatToInt(f2);  // -5
int i3 = floatToInt(d1);  // 42
```

**Range Checking:**
```cpp
int value = 5;

// Inclusive bounds: [0, 10]
bool inRangeInc = in_range(value, 0, 10, BoundType::Inclusive);  // true

// Exclusive bounds: (0, 10)
bool inRangeExc = in_range(value, 0, 10, BoundType::Exclusive);  // true

// Boundary checks
bool boundaryInc = in_range(10, 0, 10, BoundType::Inclusive);  // true
bool boundaryExc = in_range(10, 0, 10, BoundType::Exclusive);  // false

// Works with floats too
bool floatInRange = in_range(3.14f, 0.0f, 5.0f, BoundType::Inclusive);  // true
```

## Building with CMake

### Clone the Repository

```bash
git clone https://github.com/Bucephalus-Studios/stevensMathLib.git
cd stevensMathLib
```

### Building Tests

```bash
cd tests
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

```bash
# From tests/build directory
./stevensMathLib_tests

# Or using CTest
ctest --output-on-failure
```

### Building Benchmarks

```bash
cd benchmarks
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Benchmarks

```bash
# From benchmarks/build directory
./stevensMathLib_benchmarks
```

## API Reference

### Enumerations

#### `BoundType`
```cpp
enum class BoundType {
    Inclusive,  // Uses >= and <= comparisons
    Exclusive   // Uses > and < comparisons
};
```

### Functions

#### Rounding Functions

**`isWholeNumber(float value) -> bool`**
Checks if a value has no fractional part.

**`roundToNearest10th(float numberToRound) -> float`**
Rounds a number to the nearest tenth, returning as integer if whole.

**`round(float value, int decimalPlaces) -> float`**
Rounds a number to a specified number of decimal places.

#### Random Number Generation

**`seedRNG() -> void`** *(deprecated)*
Seeds the legacy C-style rand() function. Use `getRandomEngine()` instead.

**`getRandomEngine() -> std::mt19937&`**
Returns a thread-local Mersenne Twister random engine.

**`randomInt(int lowerBound = 0, int upperBound = 100) -> int`**
Generates a random integer in the range [lowerBound, upperBound).

**`randomFloat(float lowerBound = 0.0f, float upperBound = 1.0f) -> float`**
Generates a random float in the range [lowerBound, upperBound].

**`randomIntNotInBlacklist(const std::vector<int>& blacklist, int lowerBound = 0, int upperBound = 100) -> int`**
Generates a random integer not in the blacklist. Uses rejection sampling for efficiency.

#### Type Conversion

**`floatToInt<FloatType>(const FloatType& value) -> int`**
Safely converts a floating point number to an integer with bounds checking.

#### Range Checking

**`in_range<T>(const T& value, const T& lowerBound, const T& upperBound, BoundType boundType = BoundType::Inclusive) -> bool`**
Checks if a value falls within a specified range.

## Code Quality Metrics

### Nesting Levels
All functions maintain a maximum of **2 nesting levels** through:
- Early returns
- Guard clauses
- Helper function extraction

### Modern C++ Features
- `enum class` for type-safe enumerations
- Template functions for generic programming
- `inline` functions for header-only library
- Modern `<random>` library instead of `rand()`
- `thread_local` for thread-safe random generation
- Const-correctness throughout

## Testing

The library includes comprehensive unit tests covering:
- Edge cases (boundary values, empty inputs, etc.)
- Normal operation
- Error conditions
- Type variations (int, float, double, long, char)
- Random distribution quality

**Test Coverage:**
- Rounding functions: 21 test cases
- Random generation: 25 test cases
- Type conversion: 11 test cases
- Range checking: 24 test cases

## Performance

Benchmarks are provided to measure:
- Rounding operations at different precisions
- Random number generation speed
- Blacklist rejection sampling efficiency
- Range checking performance
- Float-to-int conversion speed

Run benchmarks to see performance on your system:
```bash
./build/benchmarks/stevensMathLib_benchmarks
```

## License

MIT License - Copyright (c) 2025 Bucephalus-Studios

See [LICENSE](LICENSE) for details.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Support

If you find this library useful, consider supporting the development:

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/bucephalusstudios)
