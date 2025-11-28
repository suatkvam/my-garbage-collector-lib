/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_example.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:55:35 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:55:35 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>

int main(void)
{
    t_gc_context *gc;
    t_gc_stats stats;
    int *array;
    char *str;
    void *ptr;

    printf("=== Debug Mode Example ===\n\n");

    gc = gc_create();
    if (!gc)
        return (1);

    // Enable debug mode
    printf("1. Enabling Debug Mode:\n");
    gc_set_debug(gc, GC_TRUE);
    printf("   Debug mode enabled\n\n");

    // Example 1: Track allocations
    printf("2. Tracking Allocations:\n");
    array = (int *)gc_malloc(gc, sizeof(int) * 100);
    str = gc_strdup(gc, "Debug string");
    ptr = gc_calloc(gc, 10, sizeof(double));
    
    printf("   Allocated:\n");
    printf("   - Integer array: %p\n", (void *)array);
    printf("   - String: %p (%s)\n", (void *)str, str);
    printf("   - Double array: %p\n", ptr);
    printf("\n");

    // Example 2: Get detailed statistics
    printf("3. Detailed Statistics:\n");
    gc_get_stats(gc, &stats);
    printf("   Total allocated: %zu bytes\n", stats.total_allocated);
    printf("   Total freed: %zu bytes\n", stats.total_freed);
    printf("   Current usage: %zu bytes\n", stats.current_usage);
    printf("   Peak usage: %zu bytes\n", stats.peak_usage);
    printf("   Allocation count: %zu\n", stats.allocation_count);
    printf("   Free count: %zu\n", stats.free_count);
    printf("   Scope depth: %zu\n\n", stats.scope_depth);

    // Example 3: Print formatted stats
    printf("4. Formatted Statistics:\n");
    gc_print_stats(gc);
    printf("\n");

    // Example 4: Test with scopes
    printf("5. Debug with Scopes:\n");
    gc_scope_push(gc);
    gc_malloc(gc, 512);
    gc_strdup(gc, "Scope test");
    
    printf("   Before scope pop:\n");
    gc_print_stats(gc);
    
    gc_scope_pop(gc);
    printf("   After scope pop:\n");
    gc_print_stats(gc);
    printf("\n");

    // Example 5: Collection statistics
    printf("6. Collection Test:\n");
    printf("   Before collection:\n");
    gc_print_stats(gc);
    
    gc_collect(gc);
    
    printf("\n   After collection:\n");
    gc_print_stats(gc);

    // Disable debug
    printf("\n7. Disabling Debug Mode:\n");
    gc_set_debug(gc, GC_FALSE);
    printf("   Debug mode disabled\n");

    gc_destroy(gc);
    printf("\n=== Example completed ===\n");
    return (0);
}
