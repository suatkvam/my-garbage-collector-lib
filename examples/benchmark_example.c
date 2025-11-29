#include "../garbage_collector.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double get_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

void benchmark_gc_malloc(t_gc_context *gc, int iterations)
{
    double start, end;
    int i;
    
    printf("\n=== Benchmark: gc_malloc (%d iterations) ===\n", iterations);
    start = get_time_ms();
    
    for (i = 0; i < iterations; i++)
    {
        void *ptr = gc_malloc(gc, 1024);
        if (!ptr)
        {
            printf("Allocation failed at iteration %d\n", i);
            break;
        }
    }
    
    end = get_time_ms();
    printf("Time: %.2f ms\n", end - start);
    printf("Avg per allocation: %.4f ms\n", (end - start) / iterations);
}

void benchmark_gc_strdup(t_gc_context *gc, int iterations)
{
    double start, end;
    int i;
    const char *test_str = "This is a test string for benchmarking";
    
    printf("\n=== Benchmark: gc_strdup (%d iterations) ===\n", iterations);
    start = get_time_ms();
    
    for (i = 0; i < iterations; i++)
    {
        char *dup = gc_strdup(gc, test_str);
        if (!dup)
        {
            printf("Strdup failed at iteration %d\n", i);
            break;
        }
    }
    
    end = get_time_ms();
    printf("Time: %.2f ms\n", end - start);
    printf("Avg per strdup: %.4f ms\n", (end - start) / iterations);
}

void benchmark_gc_collect(t_gc_context *gc, int alloc_count)
{
    double start, end;
    int i;
    
    printf("\n=== Benchmark: gc_collect (after %d allocations) ===\n", alloc_count);
    
    // Allocate memory
    for (i = 0; i < alloc_count; i++)
    {
        gc_malloc(gc, 1024);
    }
    
    // Measure collection time
    start = get_time_ms();
    gc_collect(gc);
    end = get_time_ms();
    
    printf("Collection time: %.2f ms\n", end - start);
}

void benchmark_scope_operations(t_gc_context *gc, int iterations)
{
    double start, end;
    int i;
    
    printf("\n=== Benchmark: Scope push/pop (%d iterations) ===\n", iterations);
    start = get_time_ms();
    
    for (i = 0; i < iterations; i++)
    {
        gc_scope_push(gc);
        gc_malloc(gc, 512);
        gc_scope_pop(gc);
    }
    
    end = get_time_ms();
    printf("Time: %.2f ms\n", end - start);
    printf("Avg per scope cycle: %.4f ms\n", (end - start) / iterations);
}

void benchmark_vs_standard_malloc(int iterations)
{
    double start, end;
    int i;
    void **ptrs;
    
    printf("\n=== Benchmark: Standard malloc/free (%d iterations) ===\n", iterations);
    
    ptrs = malloc(sizeof(void *) * iterations);
    
    start = get_time_ms();
    for (i = 0; i < iterations; i++)
    {
        ptrs[i] = malloc(1024);
    }
    for (i = 0; i < iterations; i++)
    {
        free(ptrs[i]);
    }
    end = get_time_ms();
    
    free(ptrs);
    
    printf("Time: %.2f ms\n", end - start);
    printf("Avg per malloc+free: %.4f ms\n", (end - start) / iterations);
}

void benchmark_memory_overhead(t_gc_context *gc)
{
    t_gc_stats stats;
    int i;
    size_t user_memory = 0;
    size_t overhead;
    
    printf("\n=== Memory Overhead Analysis ===\n");
    
    // Allocate 1000 blocks of 100 bytes each
    for (i = 0; i < 1000; i++)
    {
        gc_malloc(gc, 100);
        user_memory += 100;
    }
    
    gc_get_stats(gc, &stats);
    overhead = stats.total_allocated - user_memory;
    
    printf("User memory requested: %zu bytes\n", user_memory);
    printf("Total allocated: %zu bytes\n", stats.total_allocated);
    printf("Overhead: %zu bytes (%.1f%%)\n", 
           overhead, (overhead * 100.0) / user_memory);
}

int main(void)
{
    t_gc_context *gc;
    
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║     Garbage Collector Performance Benchmark              ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    // Small scale tests
    printf("\n>>> SMALL SCALE (1,000 operations)\n");
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    
    benchmark_gc_malloc(gc, 1000);
    gc_destroy(gc);
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    benchmark_gc_strdup(gc, 1000);
    gc_destroy(gc);
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    benchmark_gc_collect(gc, 1000);
    gc_destroy(gc);
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    benchmark_scope_operations(gc, 1000);
    gc_destroy(gc);
    
    // Medium scale tests
    printf("\n>>> MEDIUM SCALE (10,000 operations)\n");
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    
    benchmark_gc_malloc(gc, 10000);
    gc_destroy(gc);
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    benchmark_gc_collect(gc, 10000);
    gc_destroy(gc);
    
    // Large scale tests
    printf("\n>>> LARGE SCALE (100,000 operations)\n");
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    
    benchmark_gc_malloc(gc, 100000);
    gc_destroy(gc);
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    benchmark_gc_collect(gc, 100000);
    gc_destroy(gc);
    
    // Comparison with standard malloc
    printf("\n>>> COMPARISON WITH STANDARD MALLOC\n");
    benchmark_vs_standard_malloc(10000);
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    benchmark_gc_malloc(gc, 10000);
    gc_destroy(gc);
    
    // Memory overhead
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    benchmark_memory_overhead(gc);
    gc_destroy(gc);
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                   Benchmark Complete                     ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    return 0;
}