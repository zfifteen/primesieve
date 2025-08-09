/**
 * @file   zframework.cpp
 * @brief  Implementation of Z Framework experimental prime number generator.
 *
 * The Z Framework explores alternative approaches to prime generation using:
 * 1. Frame shift methodology for segment processing
 * 2. Residue class filtering based on modular arithmetic
 * 3. Golden ratio spacing experiments (minimal practical benefit)
 * 4. Curvature parameter k=0.3 for geometric transformations
 *
 * Note: This implementation is significantly slower than standard primesieve
 * and is intended for educational/research purposes only.
 *
 * Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
 * 
 * This file is distributed under the BSD License.
 */

#include <primesieve/zframework.h>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cerrno>

/* Global Z Framework parameters */
static double g_curvature_k = ZFRAMEWORK_CURVATURE_K;
static uint32_t g_frame_count = 0;  /* 0 = adaptive */
static double g_density_boost = ZFRAMEWORK_GOLDEN_RATIO;

/* Residue classes for wheel factorization (mod 30) */
static const uint8_t residue_classes_30[] = {
  1, 7, 11, 13, 17, 19, 23, 29
};

/**
 * Calculate optimal frame size based on adaptive geodesic optimization.
 */
static uint64_t calculate_frame_size(uint64_t range) {
  return zframework_compute_frame_size(range, g_curvature_k);
}

/**
 * Calculate frame shift offset using golden ratio sequence.
 */
static uint64_t calculate_frame_shift(uint32_t frame_index) {
  double phi = ZFRAMEWORK_GOLDEN_RATIO;
  double shift_factor = std::pow(phi, frame_index % 8) * g_curvature_k;
  return (uint64_t)(shift_factor * 256) & 0xFFFF;
}

/**
 * Apply adaptive geodesic transformation for prime density enhancement.
 */
static double apply_geometric_transform(uint64_t n, uint64_t frame_offset) {
  double phi = ZFRAMEWORK_GOLDEN_RATIO;
  double frame_factor = 1.0 + g_curvature_k * std::cos(frame_offset * M_PI / phi);
  
  /* Use adaptive density estimation with geodesic optimization */
  return zframework_density(n, frame_factor, g_density_boost);
}

/**
 * Check if number belongs to valid residue class.
 */
static int is_valid_residue(uint64_t n) {
  uint32_t mod30 = n % 30;
  for (int i = 0; i < 8; i++) {
    if (mod30 == residue_classes_30[i]) {
      return 1;
    }
  }
  return 0;
}

/**
 * Sieve primes in current frame using frame shift residue method.
 */
static void sieve_frame(zframework_generator_t* gen) {
  uint64_t frame_start = gen->start + gen->frame_shift;
  uint64_t frame_end = frame_start + gen->frame_size;
  
  if (frame_end > gen->stop) {
    frame_end = gen->stop;
  }
  
  /* Clear sieve - assume all are prime initially */
  std::memset(gen->sieve, 1, gen->sieve_size);
  
  /* Mark 0 and 1 as not prime */
  if (frame_start <= 1) {
    if (frame_start == 0) gen->sieve[0] = 0;
    if (frame_start <= 1 && frame_end > 1) gen->sieve[1 - frame_start] = 0;
  }
  
  /* Basic sieve of Eratosthenes for the frame */
  uint64_t sqrt_end = (uint64_t)std::sqrt(frame_end) + 1;
  
  for (uint64_t p = 2; p <= sqrt_end; p++) {
    /* Skip if p itself is already marked as composite */
    if (p >= 2 && p < frame_start + gen->sieve_size && 
        frame_start <= p && p < frame_end && 
        gen->sieve[p - frame_start] == 0) {
      continue;
    }
    
    /* Find first multiple of p in range [frame_start, frame_end] */
    uint64_t start_multiple = frame_start;
    if (start_multiple % p != 0) {
      start_multiple = frame_start + (p - (frame_start % p));
    }
    
    /* If p^2 > frame_start, start from p^2, but only if p^2 >= frame_start */
    if (p * p >= frame_start && p * p > start_multiple) {
      start_multiple = p * p;
    }
    
    /* Sieve multiples of p, but don't mark p itself as composite */
    for (uint64_t multiple = start_multiple; multiple <= frame_end; multiple += p) {
      if (multiple >= frame_start && multiple != p) {
        uint64_t idx = multiple - frame_start;
        if (idx < gen->sieve_size) {
          gen->sieve[idx] = 0;
        }
      }
    }
  }
  
  /* Apply residue class filtering with golden ratio enhancement */
  for (uint64_t i = 0; i < gen->frame_size && i < gen->sieve_size; i++) {
    if (gen->sieve[i]) {
      uint64_t n = frame_start + i;
      
      /* Skip numbers <= 1 */
      if (n <= 1) {
        gen->sieve[i] = 0;
        continue;
      }
      
      /* 2 and 3 are always prime */
      if (n == 2 || n == 3) {
        continue;
      }
      
      /* Apply basic residue class filtering for efficiency */
      if (!is_valid_residue(n)) {
        /* Apply golden ratio probability enhancement */
        double density = apply_geometric_transform(n, gen->frame_shift);
        
        /* Only reject if density is very low */
        if (density < g_curvature_k * 0.1) {
          gen->sieve[i] = 0;
        }
      }
    }
  }
}

int zframework_init(zframework_generator_t* gen, uint64_t start, uint64_t stop) {
  if (!gen || start > stop) {
    errno = EINVAL;
    return -1;
  }
  
  std::memset(gen, 0, sizeof(*gen));
  
  gen->start = start;
  gen->stop = stop;
  
  /* Simplify frame approach - use single frame covering entire range for now */
  uint64_t range = stop - start + 1;
  gen->frame_size = range;
  gen->frame_shift = 0;
  gen->residue_mask = 0xFF; /* All residue classes active */
  gen->density_factor = g_density_boost;
  gen->frame_count = 1; /* Single frame for correctness */
  gen->sieve_size = range;
  gen->pos = 0;
  
  /* Allocate sieve buffer */
  gen->sieve = (uint8_t*)std::malloc(gen->sieve_size);
  if (!gen->sieve) {
    errno = ENOMEM;
    return -1;
  }
  
  /* Initialize frame */
  sieve_frame(gen);
  
  return 0;
}

uint64_t zframework_next_prime(zframework_generator_t* gen) {
  if (!gen || !gen->sieve) {
    return 0;
  }
  
  uint64_t frame_start = gen->start + gen->frame_shift;
  
  while (gen->pos < gen->frame_size && frame_start + gen->pos <= gen->stop) {
    if (gen->sieve[gen->pos]) {
      uint64_t candidate = frame_start + gen->pos;
      gen->pos++;
      
      /* Basic validation */
      if (candidate <= 1) {
        continue;
      }
      
      return candidate;
    }
    gen->pos++;
  }
  
  return 0; /* No more primes */
}

uint64_t zframework_count_primes(uint64_t start, uint64_t stop) {
  zframework_generator_t gen;
  
  if (zframework_init(&gen, start, stop) != 0) {
    return 0;
  }
  
  uint64_t count = 0;
  uint64_t prime;
  
  while ((prime = zframework_next_prime(&gen)) != 0) {
    count++;
  }
  
  zframework_cleanup(&gen);
  return count;
}

uint64_t* zframework_generate_primes(uint64_t start, uint64_t stop, size_t* size) {
  if (!size) {
    errno = EINVAL;
    return NULL;
  }
  
  *size = 0;
  
  /* Estimate array size */
  uint64_t estimated_count = (stop - start) / (uint64_t)std::log(stop - start + 1) + 100;
  uint64_t* primes = (uint64_t*)std::malloc(estimated_count * sizeof(uint64_t));
  if (!primes) {
    errno = ENOMEM;
    return NULL;
  }
  
  zframework_generator_t gen;
  if (zframework_init(&gen, start, stop) != 0) {
    std::free(primes);
    return NULL;
  }
  
  uint64_t prime;
  size_t count = 0;
  
  while ((prime = zframework_next_prime(&gen)) != 0 && count < estimated_count) {
    primes[count++] = prime;
  }
  
  zframework_cleanup(&gen);
  
  /* Resize array to actual size */
  if (count > 0) {
    uint64_t* resized = (uint64_t*)std::realloc(primes, count * sizeof(uint64_t));
    if (resized) {
      primes = resized;
    }
  }
  
  *size = count;
  return primes;
}

void zframework_free(void* ptr) {
  std::free(ptr);
}

void zframework_cleanup(zframework_generator_t* gen) {
  if (gen) {
    if (gen->sieve) {
      std::free(gen->sieve);
      gen->sieve = NULL;
    }
    std::memset(gen, 0, sizeof(*gen));
  }
}

void zframework_set_parameters(double curvature_k, uint32_t frame_count, double density_boost) {
  if (curvature_k > 0.0 && curvature_k <= 1.0) {
    g_curvature_k = curvature_k;
  }
  
  if (frame_count <= ZFRAMEWORK_MAX_FRAMES) {
    g_frame_count = frame_count;
  }
  
  if (density_boost > 0.0) {
    g_density_boost = density_boost;
  }
}

/**
 * Count divisors of n using trial division.
 * This is the d(n) divisor function used in curvature calculation.
 */
static uint64_t count_divisors(uint64_t n) {
  if (n == 0) return 0;
  if (n == 1) return 1;
  
  uint64_t count = 0;
  uint64_t sqrt_n = (uint64_t)std::sqrt(n);
  
  for (uint64_t i = 1; i <= sqrt_n; i++) {
    if (n % i == 0) {
      count++;
      if (i != n / i) {
        count++;
      }
    }
  }
  
  return count;
}

double zframework_kappa(uint64_t n) {
  if (n == 0) return 0.0;
  
  uint64_t d_n = count_divisors(n);
  double ln_n_plus_1 = std::log(n + 1);
  
  return (double)d_n * ln_n_plus_1 / ZFRAMEWORK_E2;
}

uint64_t zframework_compute_frame_size(uint64_t range, double k) {
  if (range == 0) return 1024;
  
  double phi = ZFRAMEWORK_GOLDEN_RATIO;
  double base_size = std::sqrt(range) * phi;
  
  /* Apply adaptive curvature adjustment: (1 + k * sin(phi * PI / 4)) */
  double curvature_factor = 1.0 + k * std::sin(phi * M_PI / 4.0);
  uint64_t frame_size = (uint64_t)(base_size * curvature_factor);
  
  /* Ensure minimum frame size */
  if (frame_size < 1024) frame_size = 1024;
  
  /* Align to cache-friendly size (64-byte boundary) */
  return (frame_size + 63) & ~63ULL;
}

double zframework_density(uint64_t n, double frame_factor, double density_boost) {
  if (n <= 1) return 0.0;
  
  double phi = ZFRAMEWORK_GOLDEN_RATIO;
  double x = (double)n / phi;
  
  /* Prevent log(0) or negative values */
  if (x <= 1.0) x = 2.0;
  
  double log_x = std::log(x);
  if (log_x <= 0.0) log_x = 1.0;
  
  /* Density estimation: 1 / (log(n/phi) * frame_factor * density_boost) */
  return 1.0 / (log_x * frame_factor * density_boost);
}