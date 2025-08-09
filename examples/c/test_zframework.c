/** @example test_zframework.c
 *  Test and validation program for Z Framework prime generator. */

#include <primesieve/zframework.h>
#include <primesieve.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

void test_small_primes() {
  printf("=== Testing Small Primes (1-100) ===\n");
  
  /* Compare with standard primesieve */
  size_t std_count;
  uint64_t* std_primes = (uint64_t*)primesieve_generate_primes(1, 100, &std_count, UINT64_PRIMES);
  
  size_t zf_count;
  uint64_t* zf_primes = zframework_generate_primes(1, 100, &zf_count);
  
  printf("Standard primesieve found: %" PRIu64 " primes\n", (uint64_t)std_count);
  printf("Z Framework found: %" PRIu64 " primes\n", (uint64_t)zf_count);
  
  if (std_count > 0) {
    printf("Standard primes: ");
    for (size_t i = 0; i < std_count && i < 25; i++) {
      printf("%" PRIu64 " ", std_primes[i]);
    }
    printf("\n");
  }
  
  if (zf_count > 0) {
    printf("Z Framework primes: ");
    for (size_t i = 0; i < zf_count && i < 25; i++) {
      printf("%" PRIu64 " ", zf_primes[i]);
    }
    printf("\n");
  }
  
  if (std_primes) primesieve_free(std_primes);
  if (zf_primes) zframework_free(zf_primes);
  printf("\n");
}

void test_iterator() {
  printf("=== Testing Iterator Interface ===\n");
  
  zframework_generator_t gen;
  if (zframework_init(&gen, 1, 50) == 0) {
    printf("Iterating primes 1-50: ");
    
    uint64_t prime;
    int count = 0;
    while ((prime = zframework_next_prime(&gen)) != 0 && count < 20) {
      printf("%" PRIu64 " ", prime);
      count++;
    }
    printf("\n");
    
    zframework_cleanup(&gen);
  } else {
    printf("Failed to initialize Z Framework generator\n");
  }
  printf("\n");
}

void test_count_function() {
  printf("=== Testing Count Function ===\n");
  
  uint64_t ranges[][2] = {
    {1, 10},
    {1, 100},
    {1, 1000}
  };
  
  for (int i = 0; i < 3; i++) {
    uint64_t std_count = primesieve_count_primes(ranges[i][0], ranges[i][1]);
    uint64_t zf_count = zframework_count_primes(ranges[i][0], ranges[i][1]);
    
    printf("Range [%" PRIu64 ", %" PRIu64 "]: Standard=%" PRIu64 ", Z Framework=%" PRIu64 "\n", 
           ranges[i][0], ranges[i][1], std_count, zf_count);
  }
  printf("\n");
}

void test_parameters() {
  printf("=== Testing Parameter Configuration ===\n");
  
  /* Test default parameters */
  printf("Testing with default parameters...\n");
  uint64_t count1 = zframework_count_primes(1, 100);
  printf("Default: %" PRIu64 " primes\n", count1);
  
  /* Test modified parameters */
  printf("Testing with k=0.1...\n");
  zframework_set_parameters(0.1, 0, ZFRAMEWORK_GOLDEN_RATIO);
  uint64_t count2 = zframework_count_primes(1, 100);
  printf("k=0.1: %" PRIu64 " primes\n", count2);
  
  /* Reset to defaults */
  zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, ZFRAMEWORK_GOLDEN_RATIO);
  printf("\n");
}

int main() {
  printf("Z Framework Test Suite\n");
  printf("======================\n\n");
  
  test_small_primes();
  test_iterator();
  test_count_function();
  test_parameters();
  
  printf("Test suite completed.\n");
  return 0;
}