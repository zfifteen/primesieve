#include <primesieve/zframework.h>
#include <primesieve.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main() {
  printf("Debugging Z Framework vs Standard Primesieve\n");
  printf("============================================\n");
  
  /* Get standard primes */
  size_t std_count;
  uint64_t* std_primes = (uint64_t*)primesieve_generate_primes(1, 100, &std_count, UINT64_PRIMES);
  
  /* Get Z Framework primes */
  size_t zf_count;
  uint64_t* zf_primes = zframework_generate_primes(1, 100, &zf_count);
  
  printf("Standard found %" PRIu64 " primes:\n", (uint64_t)std_count);
  for (size_t i = 0; i < std_count; i++) {
    printf("%" PRIu64 " ", std_primes[i]);
  }
  printf("\n\n");
  
  printf("Z Framework found %" PRIu64 " primes:\n", (uint64_t)zf_count);
  for (size_t i = 0; i < zf_count; i++) {
    printf("%" PRIu64 " ", zf_primes[i]);
  }
  printf("\n\n");
  
  /* Find differences */
  printf("Differences:\n");
  for (size_t i = 0; i < zf_count; i++) {
    int found = 0;
    for (size_t j = 0; j < std_count; j++) {
      if (zf_primes[i] == std_primes[j]) {
        found = 1;
        break;
      }
    }
    if (!found) {
      printf("Z Framework extra: %" PRIu64 "\n", zf_primes[i]);
    }
  }
  
  for (size_t i = 0; i < std_count; i++) {
    int found = 0;
    for (size_t j = 0; j < zf_count; j++) {
      if (std_primes[i] == zf_primes[j]) {
        found = 1;
        break;
      }
    }
    if (!found) {
      printf("Z Framework missing: %" PRIu64 "\n", std_primes[i]);
    }
  }
  
  if (std_primes) primesieve_free(std_primes);
  if (zf_primes) zframework_free(zf_primes);
  
  return 0;
}