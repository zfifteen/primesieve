/** @file comprehensive_benchmark.c
 *  Comprehensive comparative testing suite for Z Framework vs Standard Primesieve
 *  This program provides extensive benchmarking and statistical analysis.
 */

#include <primesieve/zframework.h>
#include <primesieve.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <sys/resource.h>

#define MAX_RUNS 5
#define NUM_TEST_RANGES 12

typedef struct {
    uint64_t start;
    uint64_t stop;
    const char* description;
} test_range_t;

typedef struct {
    double mean;
    double stddev;
    double min;
    double max;
    long memory_kb;
} benchmark_stats_t;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

long get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // In kilobytes on Linux
}

void calculate_stats(double* times, int count, benchmark_stats_t* stats) {
    double sum = 0.0;
    stats->min = times[0];
    stats->max = times[0];
    
    for (int i = 0; i < count; i++) {
        sum += times[i];
        if (times[i] < stats->min) stats->min = times[i];
        if (times[i] > stats->max) stats->max = times[i];
    }
    
    stats->mean = sum / count;
    
    double variance = 0.0;
    for (int i = 0; i < count; i++) {
        variance += (times[i] - stats->mean) * (times[i] - stats->mean);
    }
    stats->stddev = sqrt(variance / count);
}

int benchmark_range(uint64_t start, uint64_t stop, 
                   benchmark_stats_t* std_stats, benchmark_stats_t* zf_stats) {
    double std_times[MAX_RUNS];
    double zf_times[MAX_RUNS];
    uint64_t std_count = 0, zf_count = 0;
    
    printf("Benchmarking range [%" PRIu64 ", %" PRIu64 "]...\n", start, stop);
    
    // Standard primesieve runs
    for (int run = 0; run < MAX_RUNS; run++) {
        long mem_before = get_memory_usage();
        double start_time = get_time();
        uint64_t count = primesieve_count_primes(start, stop);
        double elapsed = get_time() - start_time;
        long mem_after = get_memory_usage();
        
        std_times[run] = elapsed;
        if (run == 0) {
            std_count = count;
            std_stats->memory_kb = mem_after - mem_before;
        }
    }
    
    // Z Framework runs
    for (int run = 0; run < MAX_RUNS; run++) {
        long mem_before = get_memory_usage();
        double start_time = get_time();
        uint64_t count = zframework_count_primes(start, stop);
        double elapsed = get_time() - start_time;
        long mem_after = get_memory_usage();
        
        zf_times[run] = elapsed;
        if (run == 0) {
            zf_count = count;
            zf_stats->memory_kb = mem_after - mem_before;
        }
    }
    
    // Validate correctness
    if (std_count != zf_count) {
        printf("❌ ERROR: Count mismatch! Standard=%" PRIu64 ", ZF=%" PRIu64 "\n", 
               std_count, zf_count);
        return 0;
    }
    
    calculate_stats(std_times, MAX_RUNS, std_stats);
    calculate_stats(zf_times, MAX_RUNS, zf_stats);
    
    return 1;
}

void print_detailed_results(const char* description, uint64_t start, uint64_t stop,
                           benchmark_stats_t* std_stats, benchmark_stats_t* zf_stats,
                           uint64_t prime_count) {
    printf("\n=== %s ===\n", description);
    printf("Range: [%" PRIu64 ", %" PRIu64 "] (%" PRIu64 " primes found)\n", 
           start, stop, prime_count);
    
    printf("\nStandard Primesieve:\n");
    printf("  Time: %.6f ± %.6f seconds (min: %.6f, max: %.6f)\n",
           std_stats->mean, std_stats->stddev, std_stats->min, std_stats->max);
    printf("  Memory: %ld KB\n", std_stats->memory_kb);
    
    printf("\nZ Framework:\n");
    printf("  Time: %.6f ± %.6f seconds (min: %.6f, max: %.6f)\n",
           zf_stats->mean, zf_stats->stddev, zf_stats->min, zf_stats->max);
    printf("  Memory: %ld KB\n", zf_stats->memory_kb);
    
    double speed_ratio = zf_stats->mean / std_stats->mean;
    if (speed_ratio > 1.0) {
        printf("\n📊 Result: Standard Primesieve is %.2fx FASTER\n", speed_ratio);
    } else {
        printf("\n📊 Result: Z Framework is %.2fx FASTER\n", 1.0 / speed_ratio);
    }
    
    double memory_ratio = (double)zf_stats->memory_kb / std_stats->memory_kb;
    if (memory_ratio > 1.0) {
        printf("💾 Memory: Z Framework uses %.2fx MORE memory\n", memory_ratio);
    } else {
        printf("💾 Memory: Z Framework uses %.2fx LESS memory\n", 1.0 / memory_ratio);
    }
}

void test_scalability() {
    printf("\n🔬 SCALABILITY ANALYSIS\n");
    printf("========================\n");
    
    test_range_t scalability_ranges[] = {
        {1, 1000, "Small (1K)"},
        {1, 10000, "Medium (10K)"},
        {1, 100000, "Large (100K)"},
        {1, 1000000, "XLarge (1M)"},
        {1, 10000000, "XXLarge (10M)"}
    };
    
    printf("\nRange Size | Standard (s) | Z Framework (s) | Ratio (ZF/Std) | Primes\n");
    printf("-----------|--------------|-----------------|----------------|--------\n");
    
    for (int i = 0; i < 5; i++) {
        benchmark_stats_t std_stats, zf_stats;
        if (benchmark_range(scalability_ranges[i].start, scalability_ranges[i].stop,
                          &std_stats, &zf_stats)) {
            uint64_t count = primesieve_count_primes(scalability_ranges[i].start, 
                                                   scalability_ranges[i].stop);
            double ratio = zf_stats.mean / std_stats.mean;
            printf("%-10s | %12.6f | %15.6f | %14.2f | %6" PRIu64 "\n",
                   scalability_ranges[i].description, std_stats.mean, zf_stats.mean, 
                   ratio, count);
        }
    }
}

void test_parameter_effects() {
    printf("\n⚙️  PARAMETER OPTIMIZATION ANALYSIS\n");
    printf("====================================\n");
    
    uint64_t test_start = 10000;
    uint64_t test_stop = 100000;
    
    printf("Testing parameter effects on range [%" PRIu64 ", %" PRIu64 "]:\n\n", 
           test_start, test_stop);
    
    // Test curvature parameter
    printf("Curvature Parameter k:\n");
    printf("k     | Time (s)  | Primes\n");
    printf("------|-----------|-------\n");
    
    double k_values[] = {0.1, 0.3, 0.5, 0.7, 0.9};
    for (int i = 0; i < 5; i++) {
        zframework_set_parameters(k_values[i], 0, ZFRAMEWORK_GOLDEN_RATIO);
        
        double start_time = get_time();
        uint64_t count = zframework_count_primes(test_start, test_stop);
        double elapsed = get_time() - start_time;
        
        printf("%.1f   | %.6f | %" PRIu64 "\n", k_values[i], elapsed, count);
    }
    
    // Test golden ratio enhancement
    printf("\nGolden Ratio Enhancement:\n");
    printf("Factor | Time (s)  | Primes\n");
    printf("-------|-----------|-------\n");
    
    double phi = ZFRAMEWORK_GOLDEN_RATIO;
    double enhancements[] = {1.0, phi, phi * phi, 2.0 * phi};
    const char* names[] = {"None", "φ", "φ²", "2φ"};
    
    for (int i = 0; i < 4; i++) {
        zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, enhancements[i]);
        
        double start_time = get_time();
        uint64_t count = zframework_count_primes(test_start, test_stop);
        double elapsed = get_time() - start_time;
        
        printf("%-6s | %.6f | %" PRIu64 "\n", names[i], elapsed, count);
    }
    
    // Reset to defaults
    zframework_set_parameters(ZFRAMEWORK_CURVATURE_K, 0, ZFRAMEWORK_GOLDEN_RATIO);
}

void generate_performance_report() {
    printf("\n📋 COMPREHENSIVE PERFORMANCE REPORT\n");
    printf("=====================================\n");
    
    test_range_t test_ranges[] = {
        {1, 1000, "Small Range (1-1K)"},
        {1, 10000, "Small Range (1-10K)"},
        {1, 100000, "Medium Range (1-100K)"},
        {1, 1000000, "Large Range (1-1M)"},
        {100000, 200000, "Segment (100K-200K)"},
        {1000000, 1100000, "Segment (1M-1.1M)"},
        {10000000, 10100000, "Large Segment (10M-10.1M)"},
        {1, 10000000, "Very Large (1-10M)"}
    };
    
    int num_ranges = sizeof(test_ranges) / sizeof(test_ranges[0]);
    double total_std_speedup = 0.0;
    int faster_count = 0;
    
    for (int i = 0; i < num_ranges; i++) {
        benchmark_stats_t std_stats, zf_stats;
        if (benchmark_range(test_ranges[i].start, test_ranges[i].stop,
                          &std_stats, &zf_stats)) {
            uint64_t count = primesieve_count_primes(test_ranges[i].start, 
                                                   test_ranges[i].stop);
            print_detailed_results(test_ranges[i].description,
                                 test_ranges[i].start, test_ranges[i].stop,
                                 &std_stats, &zf_stats, count);
            
            double speedup = zf_stats.mean / std_stats.mean;
            total_std_speedup += speedup;
            if (speedup > 1.0) faster_count++;
        }
    }
    
    printf("\n🏁 FINAL SUMMARY\n");
    printf("=================\n");
    printf("Tests completed: %d\n", num_ranges);
    printf("Standard Primesieve faster: %d times\n", faster_count);
    printf("Z Framework faster: %d times\n", num_ranges - faster_count);
    printf("Average performance ratio (ZF/Std): %.2fx\n", total_std_speedup / num_ranges);
    
    if (faster_count > num_ranges / 2) {
        printf("\n🏆 VERDICT: Standard Primesieve is consistently faster\n");
        printf("    Average speedup: %.2fx\n", total_std_speedup / num_ranges);
    } else {
        printf("\n🏆 VERDICT: Z Framework shows performance advantages\n");
        printf("    Average speedup: %.2fx\n", num_ranges / total_std_speedup);
    }
}

int main(int argc, char** argv) {
    printf("🧪 COMPREHENSIVE Z FRAMEWORK BENCHMARK SUITE\n");
    printf("==============================================\n");
    printf("Performing extensive comparative testing between Z Framework and Standard Primesieve\n");
    printf("Each test runs %d times for statistical analysis\n\n", MAX_RUNS);
    
    // Correctness validation first
    printf("🔍 CORRECTNESS VALIDATION\n");
    printf("==========================\n");
    test_range_t validation_ranges[] = {
        {1, 100, "Basic"},
        {1000, 2000, "Mid-range"},
        {10000, 11000, "Large"}
    };
    
    int all_correct = 1;
    for (int i = 0; i < 3; i++) {
        uint64_t std_count = primesieve_count_primes(validation_ranges[i].start, 
                                                   validation_ranges[i].stop);
        uint64_t zf_count = zframework_count_primes(validation_ranges[i].start, 
                                                  validation_ranges[i].stop);
        
        if (std_count == zf_count) {
            printf("✅ %s [%" PRIu64 ", %" PRIu64 "]: %" PRIu64 " primes\n",
                   validation_ranges[i].description,
                   validation_ranges[i].start, validation_ranges[i].stop, std_count);
        } else {
            printf("❌ %s [%" PRIu64 ", %" PRIu64 "]: Mismatch! (Std: %" PRIu64 ", ZF: %" PRIu64 ")\n",
                   validation_ranges[i].description,
                   validation_ranges[i].start, validation_ranges[i].stop, 
                   std_count, zf_count);
            all_correct = 0;
        }
    }
    
    if (!all_correct) {
        printf("\n❌ CRITICAL: Correctness validation failed! Stopping benchmark.\n");
        return 1;
    }
    
    printf("\n✅ All correctness tests passed!\n");
    
    // Run comprehensive benchmarks
    test_scalability();
    test_parameter_effects();
    generate_performance_report();
    
    printf("\n🎯 Comprehensive benchmark completed successfully!\n");
    printf("   Results show detailed performance characteristics of both implementations.\n");
    
    return 0;
}