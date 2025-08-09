/**
 * @file   zframework.h
 * @brief  Z Framework experimental prime number generator using frame shift residue method.
 *         This implementation explores alternative mathematical approaches but is
 *         significantly slower than standard primesieve. Intended for educational use.
 * 
 * Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
 * 
 * This file is distributed under the BSD License.
 */

#ifndef ZFRAMEWORK_H
#define ZFRAMEWORK_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Z Framework configuration constants
 */
#define ZFRAMEWORK_GOLDEN_RATIO 1.6180339887498948482
#define ZFRAMEWORK_E2 7.38905609893065022723  /* e^2 */
#define ZFRAMEWORK_CURVATURE_K 0.3
#define ZFRAMEWORK_OPT_K 0.3  /* Optimal curvature parameter */
#define ZFRAMEWORK_MAX_FRAMES 32
#define ZFRAMEWORK_RESIDUE_CLASSES 30  /* 2*3*5 wheel basis */

/**
 * Z Framework prime generator structure
 */
typedef struct {
  uint64_t start;           /* Start of current segment */
  uint64_t stop;            /* End of current segment */
  uint64_t frame_size;      /* Current frame size */
  uint64_t frame_shift;     /* Frame shift offset */
  uint64_t residue_mask;    /* Active residue classes */
  double density_factor;    /* Prime density enhancement factor */
  uint32_t frame_count;     /* Number of active frames */
  uint8_t* sieve;          /* Sieve buffer */
  size_t sieve_size;       /* Size of sieve buffer */
  uint64_t pos;            /* Current position in sieve */
} zframework_generator_t;

/**
 * Initialize Z Framework prime generator.
 * @param gen    Generator structure to initialize
 * @param start  Starting number for prime generation
 * @param stop   Ending number for prime generation
 * @return 0 on success, -1 on error
 */
int zframework_init(zframework_generator_t* gen, uint64_t start, uint64_t stop);

/**
 * Generate next prime using Z Framework frame shift residue method.
 * @param gen  Initialized generator structure
 * @return Next prime number, or 0 if no more primes in range
 */
uint64_t zframework_next_prime(zframework_generator_t* gen);

/**
 * Count primes in range using Z Framework method.
 * @param start  Starting number
 * @param stop   Ending number
 * @return Number of primes in range [start, stop]
 */
uint64_t zframework_count_primes(uint64_t start, uint64_t stop);

/**
 * Generate array of primes using Z Framework method.
 * @param start  Starting number
 * @param stop   Ending number
 * @param size   Output parameter for array size
 * @return Array of primes (must be freed with zframework_free)
 */
uint64_t* zframework_generate_primes(uint64_t start, uint64_t stop, size_t* size);

/**
 * Free memory allocated by Z Framework functions.
 * @param ptr  Pointer to free
 */
void zframework_free(void* ptr);

/**
 * Clean up Z Framework generator.
 * @param gen  Generator to clean up
 */
void zframework_cleanup(zframework_generator_t* gen);

/**
 * Set Z Framework parameters for fine-tuning.
 * @param curvature_k     Curvature parameter (default 0.3)
 * @param frame_count     Number of frames (default adaptive)
 * @param density_boost   Density enhancement factor (default golden ratio based)
 */
void zframework_set_parameters(double curvature_k, uint32_t frame_count, double density_boost);

/**
 * Compute curvature kappa(n) = d(n) * ln(n+1) / e^2
 * where d(n) is the divisor function
 * @param n  Input number
 * @return Curvature value for adaptive geodesic optimization
 */
double zframework_kappa(uint64_t n);

/**
 * Adaptive frame size calculation: floor(sqrt(range) * phi * (1 + k * sin(phi * PI / 4)))
 * @param range  Range size for prime generation
 * @param k      Curvature parameter
 * @return Optimized frame size for given range and curvature
 */
uint64_t zframework_compute_frame_size(uint64_t range, double k);

/**
 * Density estimation: 1 / (log(n/phi) * frame_factor * density_boost)
 * @param n             Input number
 * @param frame_factor  Frame adjustment factor
 * @param density_boost Density enhancement multiplier
 * @return Estimated prime density for adaptive optimization
 */
double zframework_density(uint64_t n, double frame_factor, double density_boost);

#ifdef __cplusplus
}
#endif

#endif /* ZFRAMEWORK_H */