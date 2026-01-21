/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 18:52:01 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 18:52:01 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>

void test_small_allocations(t_gc_context *gc)
{
    char *small1, *small2, *small3;
    
    printf("\n=== Test 1: Small Allocations (<256 bytes) ===\n");
    
    small1 = (char *)gc_malloc(gc, 50);
    small2 = (char *)gc_malloc(gc, 100);
    small3 = (char *)gc_malloc(gc, 200);
    
    if (!small1 || !small2 || !small3)
    {
        printf("❌ Allocation failed!\n");
        return;
    }
    
    printf("✅ Small allocations successful:\n");
    printf("   small1 (50 bytes):  %p\n", (void *)small1);
    printf("   small2 (100 bytes): %p\n", (void *)small2);
    printf("   small3 (200 bytes): %p\n", (void *)small3);
    
    // Check if they're close (from same pool)
    long diff1 = (char *)small2 - (char *)small1;
    long diff2 = (char *)small3 - (char *)small2;
    
    printf("\n   Address differences:\n");
    printf("   small2 - small1 = %ld bytes\n", diff1);
    printf("   small3 - small2 = %ld bytes\n", diff2);
    
    if (diff1 > 0 && diff1 < 512 && diff2 > 0 && diff2 < 512)
        printf("   ✅ Allocations are close (likely from pool)\n");
    else
        printf("   ⚠️  Allocations are far apart\n");
}

void test_large_allocations(t_gc_context *gc)
{
    char *large1, *large2;
    
    printf("\n=== Test 2: Large Allocations (>=256 bytes) ===\n");
    
    large1 = (char *)gc_malloc(gc, 500);
    large2 = (char *)gc_malloc(gc, 1000);
    
    if (!large1 || !large2)
    {
        printf("❌ Allocation failed!\n");
        return;
    }
    
    printf("✅ Large allocations successful:\n");
    printf("   large1 (500 bytes):  %p\n", (void *)large1);
    printf("   large2 (1000 bytes): %p\n", (void *)large2);
}

void test_mixed_allocations(t_gc_context *gc)
{
    int i;
    void *ptrs[20];
    
    printf("\n=== Test 3: Mixed Small/Large Allocations ===\n");
    
    for (i = 0; i < 20; i++)
    {
        if (i % 2 == 0)
            ptrs[i] = gc_malloc(gc, 100);  // Small
        else
            ptrs[i] = gc_malloc(gc, 300);  // Large
        
        if (!ptrs[i])
        {
            printf("❌ Allocation %d failed!\n", i);
            return;
        }
    }
    
    printf("✅ Mixed allocations: 10 small + 10 large successful\n");
}

void test_pool_stress(t_gc_context *gc)
{
    int i;
    char *ptrs[1000];
    
    printf("\n=== Test 4: Pool Stress Test (1000 small allocs) ===\n");
    
    for (i = 0; i < 1000; i++)
    {
        ptrs[i] = (char *)gc_malloc(gc, 50);
        if (!ptrs[i])
        {
            printf("❌ Allocation failed at iteration %d\n", i);
            return;
        }
    }
    
    printf("✅ 1000 small allocations successful\n");
    
    t_gc_stats stats;
    gc_get_stats(gc, &stats);
    printf("   Total allocated: %zu bytes\n", stats.total_allocated);
    printf("   Current usage: %zu bytes\n", stats.current_usage);
}

void test_scope_with_pool(t_gc_context *gc)
{
    t_gc_stats stats_before, stats_after;
    
    printf("\n=== Test 5: Scope Pop with Pool ===\n");
    
    gc_get_stats(gc, &stats_before);
    
    gc_scope_push(gc);
    
    // Allocate in scope
    for (int i = 0; i < 10; i++)
        gc_malloc(gc, 100);
    
    printf("   Allocated 10 x 100 bytes in scope\n");
    
    gc_scope_pop(gc);
    printf("   ✅ Scope popped (should not crash)\n");
    
    gc_get_stats(gc, &stats_after);
    
    printf("   Allocations freed: %zu\n", 
           stats_after.free_count - stats_before.free_count);
}

int main(void)
{
    t_gc_context *gc;
    
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║       Memory Pool Functionality Tests         ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    gc = gc_create();
    if (!gc)
    {
        printf("❌ Failed to create GC context\n");
        return 1;
    }
    
    gc_set_mode(gc, GC_MODE_MANUAL);
    
    test_small_allocations(gc);
    test_large_allocations(gc);
    test_mixed_allocations(gc);
    test_pool_stress(gc);
    test_scope_with_pool(gc);
    
    printf("\n=== Final Statistics ===\n");
    gc_print_stats(gc);
    
    gc_destroy(gc);
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║            All Tests Completed ✅              ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    return 0;
}
