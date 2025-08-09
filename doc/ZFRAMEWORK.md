# Z Framework Prime Generator (Experimental)

## Overview

⚠️ **Performance Analysis**: Comprehensive benchmarking reveals that standard primesieve significantly outperforms Z Framework, with standard primesieve being **37x faster on average** across tested ranges. Z Framework shows minor advantages only on very small ranges (≤1K primes).

The Z Framework explores advanced mathematical approaches to prime generation using **frame shift residue methods** and **golden ratio geometric transformations**. While the implementation demonstrates innovative mathematical concepts, it serves primarily as an educational and experimental implementation rather than a performance-optimized replacement for primesieve.

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
- **Enhancement overhead**: Geometric calculations add minimal cost

### Benchmark Results

📊 **Comprehensive Performance Analysis**: Extensive testing reveals standard primesieve's significant performance superiority:

| Range Size | Standard Time (s) | Z Framework Time (s) | Performance Ratio |
|------------|-------------------|---------------------|-------------------|
| 1-1,000 | 0.000014 | 0.000004 | **Z Framework 3.5x faster** |
| 1-10,000 | 0.000015 | 0.000042 | **Standard 2.8x faster** |
| 1-100,000 | 0.000018 | 0.000439 | **Standard 24.9x faster** |
| 1-1,000,000 | 0.000053 | 0.004428 | **Standard 82.9x faster** |
| 1-10,000,000 | 0.000558 | 0.043910 | **Standard 78.7x faster** |

*Benchmarks performed on Intel processor with GCC 13.3.0, -O3 optimization, averaged over 5 runs*

**Key Findings:**
- **Overall**: Standard primesieve is **37x faster on average**
- **Small ranges (≤1K)**: Z Framework shows minor performance advantage
- **Medium to large ranges**: Standard primesieve demonstrates dramatic superiority
- **Scalability**: Performance gap increases significantly with range size

The analysis shows that while Z Framework demonstrates interesting mathematical concepts, it is **not a performance replacement** for standard primesieve. The framework is better suited for educational purposes and algorithm experimentation.

### Parameter Effects

**Curvature Parameter k:**
- k = 0.1: Minimal effect on performance
- k = 0.3: Default setting  
- k = 0.7: Minimal effect on performance

**Golden Ratio Enhancement:**
- 1.0: No enhancement
- φ (1.618): Optimal performance settings
- φ² (2.618): Enhanced geometric filtering  
- 2φ (3.236): Maximum density optimization

**Note**: Comprehensive performance testing shows that golden ratio enhancements provide minimal performance impact (< 2% variation). The mathematical concepts are interesting for educational purposes but do not provide meaningful optimization benefits over standard algorithms.

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

1. **Performance**: Standard primesieve is significantly faster (37x average) for all but very small ranges
2. **Scalability**: Performance degrades rapidly with increasing range size compared to primesieve
3. **Memory usage**: Higher memory consumption (1 byte per integer) vs primesieve's optimized memory usage
4. **Algorithm efficiency**: Uses basic sieve of Eratosthenes vs primesieve's optimized segmented sieve with SIMD
5. **Single-threaded**: Each generator instance is single-threaded (multi-threading enhancements planned)
6. **Educational focus**: Better suited for learning and experimentation rather than production use

## Recommended Use Cases

**Suitable for:**
- Educational purposes and algorithm learning
- Mathematical research and experimentation
- Very small prime ranges (≤1,000 numbers)
- Demonstrating alternative mathematical approaches

**Not recommended for:**
- Production applications requiring performance
- Large-scale prime generation
- Performance-critical computations
- Replacement of standard primesieve

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

*The Z Framework demonstrates innovative mathematical concepts for prime generation through frame shift residue methods and golden ratio transformations. While not a performance replacement for standard primesieve, it serves as an educational implementation exploring alternative mathematical approaches to prime sieving algorithms. For production use, standard primesieve remains the recommended choice due to its superior performance optimization.*

**🔗 Related Documentation:**
- [Comprehensive Performance Report](ZFRAMEWORK_PERFORMANCE_REPORT.md) - Detailed benchmark analysis and findings