/** @example zframework_performance.c
 *  Performance comparison between Z Framework and standard primesieve.
 *  Note: Z Framework is significantly slower and intended for educational use only. */

#include <primesieve/zframework.h>
#include <primesieve.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void compare_performance(uint64_t start, uint64_t stop) {
  printf("Comparing performance on range [%" PRIu64 ", %" PRIu64 "]:\n", start, stop);
  
  /* Test standard primesieve */
  double start_time = get_time();
  uint64_t std_count = primesieve_count_primes(start, stop);
  double std_time = get_time() - start_time;
  
  /* Test Z Framework */
  start_time = get_time();
  uint64_t zf_count = zframework_count_primes(start, stop);
  double zf_time = get_time() - start_time;
  
  printf("Standard primesieve: %" PRIu64 " primes in %.6f seconds\n", std_count, std_time);
  printf("Z Framework:         %" PRIu64 " primes in %.6f seconds\n", zf_count, zf_time);
  
  if (std_count == zf_count) {
    printf("✓ Results match!\n");
  } else {
    printf("✗ Results differ! Standard=%" PRIu64 ", Z Framework=%" PRIu64 "\n", std_count, zf_count);
  }
  
  if (zf_time > 0 && std_time > 0) {
    double speedup = std_time / zf_time;
    if (speedup > 1.0) {
      printf("Z Framework is %.2fx faster\n", speedup);
    } else {
      printf("Standard primesieve is %.2fx faster\n", 1.0 / speedup);
    }
  }
  printf("\n");
}

void test_golden_ratio_enhancement() {
  printf("=== Golden Ratio Enhancement Analysis ===\n");
  
  uint64_t test_ranges[][2] = {
    {1, 1000},
    {10000, 20000},
    {100000, 200000},
    {1000000, 1100000}
  };
  
  double phi = ZFRAMEWORK_GOLDEN_RATIO;
  
  for (int i = 0; i < 4; i++) {
    uint64_t start = test_ranges[i][0];
    uint64_t stop = test_ranges[i][1];
    
    printf("Range [%" PRIu64 ", %" PRIu64 "]:\n", start, stop);
    
    /* Test with different golden ratio multipliers */
    double multipliers[] = {1.0, phi, phi * phi, 2.0 * phi};
    const char* names[] = {"No enhancement", "φ", "φ²", "2φ"};
    
    for (int j = 0; j < 4; j++) {
      zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, multipliers[j]);
      
      double start_time = get_time();
      uint64_t count = zframework_count_primes(start, stop);
      double elapsed = get_time() - start_time;
      
      printf("  %s (%.3f): %" PRIu64 " primes, %.6f seconds\n", 
             names[j], multipliers[j], count, elapsed);
    }
    
    /* Reset to default */
    zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, ZFRAMEWORK_GOLDEN_RATIO);
    printf("\n");
  }
}

void test_curvature_parameter() {
  printf("=== Curvature Parameter k Analysis ===\n");
  
  uint64_t start = 10000;
  uint64_t stop = 50000;
  
  printf("Testing curvature parameter k on range [%" PRIu64 ", %" PRIu64 "]:\n", start, stop);
  
  double k_values[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
  
  for (int i = 0; i < 9; i++) {
    zframework_set_parameters(k_values[i], 0, ZFRAMEWORK_GOLDEN_RATIO);
    
    double start_time = get_time();
    uint64_t count = zframework_count_primes(start, stop);
    double elapsed = get_time() - start_time;
    
    printf("k=%.1f: %" PRIu64 " primes, %.6f seconds\n", k_values[i], count, elapsed);
  }
  
  /* Reset to default */
  zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, ZFRAMEWORK_GOLDEN_RATIO);
  printf("\n");
}

void validate_correctness() {
  printf("=== Correctness Validation ===\n");
  
  uint64_t test_ranges[][2] = {
    {1, 100},
    {1000, 2000},
    {10000, 11000},
    {100000, 101000}
  };
  
  int all_passed = 1;
  
  for (int i = 0; i < 4; i++) {
    uint64_t start = test_ranges[i][0];
    uint64_t stop = test_ranges[i][1];
    
    uint64_t std_count = primesieve_count_primes(start, stop);
    uint64_t zf_count = zframework_count_primes(start, stop);
    
    printf("Range [%" PRIu64 ", %" PRIu64 "]: ", start, stop);
    
    if (std_count == zf_count) {
      printf("✓ PASS (%" PRIu64 " primes)\n", std_count);
    } else {
      printf("✗ FAIL (std=%" PRIu64 ", zf=%" PRIu64 ")\n", std_count, zf_count);
      all_passed = 0;
    }
  }
  
  printf("\nOverall correctness: %s\n\n", all_passed ? "✓ PASS" : "✗ FAIL");
}

int main(int argc, char** argv) {
  printf("Z Framework Performance Analysis\n");
  printf("================================\n\n");
  printf("⚠️  NOTICE: Z Framework is experimental and significantly slower than primesieve.\n");
  printf("    This analysis is for educational/research purposes only.\n\n");
  
  validate_correctness();
  
  /* Performance comparison on various ranges */
  compare_performance(1, 10000);
  compare_performance(1, 100000);
  compare_performance(100000, 200000);
  compare_performance(1000000, 1100000);
  
  test_golden_ratio_enhancement();
  test_curvature_parameter();
  
  /* Custom range from command line */
  if (argc == 3) {
    uint64_t start = strtoull(argv[1], NULL, 10);
    uint64_t stop = strtoull(argv[2], NULL, 10);
    printf("=== Custom Range Performance Test ===\n");
    compare_performance(start, stop);
  }
  
  printf("Performance analysis completed.\n");
  return 0;
}