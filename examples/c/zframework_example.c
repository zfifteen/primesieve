/** @example zframework_example.c
 *  Example usage of Z Framework prime generator with frame shift residue method. */

#include <primesieve/zframework.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

void demo_basic_usage(void) {
  printf("=== Z Framework Basic Usage ===\n");
  
  /* Generate primes using Z Framework */
  size_t count;
  uint64_t* primes = zframework_generate_primes(1, 1000, &count);
  
  if (primes) {
    printf("Found %" PRIu64 " primes between 1 and 1000 using Z Framework:\n", (uint64_t)count);
    
    /* Print first 20 primes */
    printf("First 20 primes: ");
    for (size_t i = 0; i < count && i < 20; i++) {
      printf("%" PRIu64 " ", primes[i]);
    }
    printf("\n");
    
    zframework_free(primes);
  } else {
    printf("Error generating primes\n");
  }
  printf("\n");
}

void demo_iterator_usage(void) {
  printf("=== Z Framework Iterator Usage ===\n");
  
  zframework_generator_t gen;
  
  if (zframework_init(&gen, 1000000, 1001000) == 0) {
    printf("Primes between 1,000,000 and 1,001,000:\n");
    
    uint64_t prime;
    int count = 0;
    while ((prime = zframework_next_prime(&gen)) != 0 && count < 10) {
      printf("%" PRIu64 " ", prime);
      count++;
    }
    if (count == 10) {
      printf("... (showing first 10)");
    }
    printf("\n");
    
    zframework_cleanup(&gen);
  } else {
    printf("Error initializing Z Framework generator\n");
  }
  printf("\n");
}

void demo_count_performance(void) {
  printf("=== Z Framework Performance Demo ===\n");
  
  uint64_t ranges[][2] = {
    {1, 10000},
    {1, 100000},
    {1, 1000000},
    {1000000, 2000000}
  };
  
  for (int i = 0; i < 4; i++) {
    clock_t start = clock();
    uint64_t count = zframework_count_primes(ranges[i][0], ranges[i][1]);
    clock_t end = clock();
    
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Range [%" PRIu64 ", %" PRIu64 "]: %" PRIu64 " primes in %.6f seconds\n", 
           ranges[i][0], ranges[i][1], count, time_taken);
  }
  printf("\n");
}

void demo_parameter_tuning(void) {
  printf("=== Z Framework Parameter Tuning ===\n");
  
  /* Test different curvature parameters */
  double curvature_values[] = {0.1, 0.3, 0.5, 0.7};
  uint64_t test_range_start = 10000;
  uint64_t test_range_end = 20000;
  
  printf("Testing curvature parameter k on range [%" PRIu64 ", %" PRIu64 "]:\n", 
         test_range_start, test_range_end);
  
  for (int i = 0; i < 4; i++) {
    zframework_set_parameters(curvature_values[i], 0, ZFRAMEWORK_GOLDEN_RATIO);
    
    clock_t start = clock();
    uint64_t count = zframework_count_primes(test_range_start, test_range_end);
    clock_t end = clock();
    
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("k=%.1f: %" PRIu64 " primes, %.6f seconds\n", 
           curvature_values[i], count, time_taken);
  }
  
  /* Reset to default parameters */
  zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, ZFRAMEWORK_GOLDEN_RATIO);
  printf("\n");
}

void demo_golden_ratio_enhancement(void) {
  printf("=== Golden Ratio Enhancement Demo ===\n");
  
  uint64_t test_start = 1000000;
  uint64_t test_end = 1100000;
  
  printf("Comparing different density boost factors on range [%" PRIu64 ", %" PRIu64 "]:\n", 
         test_start, test_end);
  
  double boost_factors[] = {1.0, ZFRAMEWORK_GOLDEN_RATIO, 2.0, 2.618};
  const char* names[] = {"No boost", "Golden ratio", "2x boost", "φ²"};
  
  for (int i = 0; i < 4; i++) {
    zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, boost_factors[i]);
    
    clock_t start = clock();
    uint64_t count = zframework_count_primes(test_start, test_end);
    clock_t end = clock();
    
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("%s (%.3f): %" PRIu64 " primes, %.6f seconds\n", 
           names[i], boost_factors[i], count, time_taken);
  }
  
  /* Reset to default */
  zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, ZFRAMEWORK_GOLDEN_RATIO);
  printf("\n");
}

int main(int argc, char** argv) {
  printf("Z Framework Prime Generator Demo\n");
  printf("Leveraging frame shift residue method with golden ratio optimization\n");
  printf("=====================================================================\n\n");
  
  /* Parse command line arguments for custom range */
  uint64_t custom_start = 1, custom_end = 1000;
  if (argc == 3) {
    custom_start = strtoull(argv[1], NULL, 10);
    custom_end = strtoull(argv[2], NULL, 10);
  }
  
  demo_basic_usage();
  demo_iterator_usage();
  demo_count_performance();
  demo_parameter_tuning();
  demo_golden_ratio_enhancement();
  
  if (argc == 3) {
    printf("=== Custom Range Test ===\n");
    printf("Testing Z Framework on user-specified range [%" PRIu64 ", %" PRIu64 "]:\n", 
           custom_start, custom_end);
    
    clock_t start = clock();
    uint64_t count = zframework_count_primes(custom_start, custom_end);
    clock_t end = clock();
    
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Found %" PRIu64 " primes in %.6f seconds\n", count, time_taken);
  }
  
  printf("Demo completed successfully!\n");
  return 0;
}