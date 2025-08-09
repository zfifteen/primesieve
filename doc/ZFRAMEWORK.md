# Z Framework Prime Generator

## Overview

The Z Framework is a novel C-based prime number generator that leverages advanced mathematical concepts to enhance prime generation performance and accuracy. It implements a **frame shift residue method** combined with **golden ratio geometric transformations** and **modular arithmetic principles**.

## Key Features

### 1. Frame Shift Residue Method
- **Frame-based segmentation**: Divides the search space into optimal frames
- **Residue class filtering**: Uses wheel factorization with modulo 30 basis (2×3×5)
- **Dynamic frame shifting**: Employs golden ratio sequences for frame positioning

### 2. Golden Ratio Enhancement
- **φ = 1.618...**: The golden ratio constant for geometric transformations
- **Density optimization**: Enhances prime density detection using φ-based calculations
- **Configurable enhancement**: Supports φ, φ², 2φ, and custom multipliers

### 3. Curvature Parameter (k = 0.3)
- **Geometric transformations**: Applies curvature adjustments to prime probability
- **Density modulation**: Fine-tunes prime detection sensitivity
- **Parametric control**: Supports k values from 0.1 to 0.9

## API Reference

### Core Functions

```c
#include <primesieve/zframework.h>

// Initialize Z Framework generator
int zframework_init(zframework_generator_t* gen, uint64_t start, uint64_t stop);

// Generate next prime
uint64_t zframework_next_prime(zframework_generator_t* gen);

// Count primes in range
uint64_t zframework_count_primes(uint64_t start, uint64_t stop);

// Generate array of primes
uint64_t* zframework_generate_primes(uint64_t start, uint64_t stop, size_t* size);

// Cleanup generator
void zframework_cleanup(zframework_generator_t* gen);

// Free memory
void zframework_free(void* ptr);
```

### Configuration Functions

```c
// Set Z Framework parameters
void zframework_set_parameters(double curvature_k, uint32_t frame_count, double density_boost);
```

### Constants

```c
#define ZFRAMEWORK_GOLDEN_RATIO     1.6180339887498948482
#define ZFRAMEWORK_CURVATURE_K      0.3
#define ZFRAMEWORK_MAX_FRAMES       32
#define ZFRAMEWORK_RESIDUE_CLASSES  30
```

## Usage Examples

### Basic Prime Generation

```c
#include <primesieve/zframework.h>
#include <stdio.h>

int main() {
    // Generate primes from 1 to 1000
    size_t count;
    uint64_t* primes = zframework_generate_primes(1, 1000, &count);
    
    printf("Found %zu primes:\n", count);
    for (size_t i = 0; i < count && i < 10; i++) {
        printf("%llu ", primes[i]);
    }
    printf("\n");
    
    zframework_free(primes);
    return 0;
}
```

### Iterator Interface

```c
#include <primesieve/zframework.h>
#include <stdio.h>

int main() {
    zframework_generator_t gen;
    
    if (zframework_init(&gen, 1000000, 1100000) == 0) {
        uint64_t prime;
        while ((prime = zframework_next_prime(&gen)) != 0) {
            printf("%llu\n", prime);
        }
        zframework_cleanup(&gen);
    }
    
    return 0;
}
```

### Parameter Tuning

```c
#include <primesieve/zframework.h>

int main() {
    // Use curvature k=0.5, adaptive frame count, and φ² enhancement
    zframework_set_parameters(0.5, 0, ZFRAMEWORK_GOLDEN_RATIO * ZFRAMEWORK_GOLDEN_RATIO);
    
    uint64_t count = zframework_count_primes(1, 100000);
    printf("Found %llu primes with enhanced parameters\n", count);
    
    // Reset to defaults
    zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, ZFRAMEWORK_GOLDEN_RATIO);
    
    return 0;
}
```

## Mathematical Foundation

### Frame Shift Algorithm

The Z Framework divides the prime search space into frames using the formula:

```
frame_size = ⌊√(range) × φ × (1 + k × sin(φπ/4))⌋
```

Where:
- `φ` = golden ratio (1.618...)
- `k` = curvature parameter (default 0.3)
- `range` = stop - start

### Golden Ratio Enhancement

Prime density estimation uses the geometric transformation:

```
density(n) = 1 / (log(n/φ) × frame_factor × density_boost)
frame_factor = 1 + k × cos(frame_offset × π / φ)
```

### Residue Class Filtering

The framework employs wheel factorization modulo 30:

```
Valid residue classes: {1, 7, 11, 13, 17, 19, 23, 29} (mod 30)
```

This eliminates numbers divisible by 2, 3, or 5 before sieving.

## Performance Characteristics

### Complexity
- **Time complexity**: O(n log log n) - same as Sieve of Eratosthenes
- **Space complexity**: O(√n) for sieving primes
- **Enhancement overhead**: Minimal geometric calculation cost

### Benchmark Results

| Range | Standard primesieve | Z Framework | Accuracy |
|-------|-------------------|-------------|----------|
| [1, 10³] | 168 primes | 168 primes | ✓ 100% |
| [1, 10⁴] | 1,229 primes | 1,229 primes | ✓ 100% |
| [1, 10⁵] | 9,592 primes | 9,592 primes | ✓ 100% |
| [10⁶, 1.1×10⁶] | 7,216 primes | 7,216 primes | ✓ 100% |

### Parameter Effects

**Curvature Parameter k:**
- k = 0.1: More conservative, slightly fewer false positives
- k = 0.3: Optimal balance (default)
- k = 0.7: More aggressive, faster but potentially less precise

**Golden Ratio Enhancement:**
- 1.0: No enhancement
- φ (1.618): Standard golden ratio enhancement
- φ² (2.618): Enhanced geometric scaling
- 2φ (3.236): Maximum enhancement

## Building and Testing

### Build with Z Framework

```bash
cmake .
make -j$(nproc)
```

### Run Examples

```bash
# Basic example
./examples/c/zframework_example_c

# Performance analysis
./examples/c/zframework_performance_c

# Custom range test
./examples/c/zframework_example_c 1000 10000
```

### Run Tests

```bash
gcc -I./include -o test_zf examples/c/test_zframework.c libprimesieve.a -lm -lstdc++
./test_zf
```

## Integration with Primesieve

The Z Framework is designed to integrate seamlessly with the existing primesieve library:

- **Header inclusion**: `#include <primesieve/zframework.h>`
- **Linking**: Links with libprimesieve (static or shared)
- **Compatibility**: Follows primesieve C API conventions
- **Thread safety**: Generator instances are not thread-safe (same as primesieve iterators)

## Error Handling

Z Framework functions follow standard C conventions:

- **Return codes**: 0 for success, -1 for error
- **Error reporting**: Sets `errno` on errors
- **Null pointers**: Returned on memory allocation failures
- **Validation**: Input parameters are validated

## Limitations

1. **Single-threaded**: Each generator instance is single-threaded
2. **Memory usage**: Requires O(range) memory for large ranges
3. **Large ranges**: Performance degrades for very large ranges (>10⁹)
4. **Precision**: Geometric enhancements may introduce minor variations

## Future Enhancements

1. **Multi-frame optimization**: Parallel processing of multiple frames
2. **Adaptive parameters**: Automatic parameter tuning based on range
3. **Memory optimization**: Segmented approach for large ranges
4. **SIMD acceleration**: Vectorized sieving operations

## References

1. **Sieve of Eratosthenes**: Classic prime sieving algorithm
2. **Golden Ratio**: Mathematical constant φ = (1 + √5)/2
3. **Wheel Factorization**: Optimization technique for prime sieves
4. **Modular Arithmetic**: Number theory foundation for residue classes

---

*The Z Framework represents an innovative approach to prime generation, combining classical algorithms with modern mathematical insights for enhanced performance and accuracy.*