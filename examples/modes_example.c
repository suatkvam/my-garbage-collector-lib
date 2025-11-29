/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes_example.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:55:35 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:55:35 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>

void allocate_test_data(t_gc_context *gc, int count)
{
    int i;
    char *data;

    i = 0;
    while (i < count)
    {
        data = gc_malloc(gc, 1024);
        if (data)
            data[0] = 'X';
        i++;
    }
}

int main(void)
{
    t_gc_context *gc;
    t_gc_mode current_mode;

    printf("=== GC Modes Example ===\n\n");

    // Example 1: MANUAL MODE
    printf("1. MANUAL MODE:\n");
    printf("   Memory is only freed when you call gc_collect()\n\n");
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_MANUAL);
    
    printf("   Allocating memory...\n");
    allocate_test_data(gc, 10);
    gc_print_stats(gc);
    
    printf("\n   Calling gc_collect() manually...\n");
    gc_collect(gc);
    gc_print_stats(gc);
    
    gc_destroy(gc);
    printf("\n");

    // Example 2: AUTO MODE
    printf("2. AUTO MODE:\n");
    printf("   Garbage collection runs automatically when threshold is reached\n\n");
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_AUTO);
    
    current_mode = gc_get_mode(gc);
    printf("   Current mode: %d (AUTO=1)\n", current_mode);
    
    printf("   Allocating memory (auto-collect will trigger)...\n");
    allocate_test_data(gc, 50);
    gc_print_stats(gc);
    
    gc_destroy(gc);
    printf("\n");

    // Example 3: HYBRID MODE
    printf("3. HYBRID MODE:\n");
    printf("   Combines manual control with automatic collection\n\n");
    
    gc = gc_create();
    gc_set_mode(gc, GC_MODE_HYBRID);
    
    printf("   Allocating with hybrid mode...\n");
    allocate_test_data(gc, 20);
    
    printf("\n   Manual collect in hybrid mode:\n");
    gc_collect(gc);
    gc_print_stats(gc);
    
    gc_destroy(gc);
    printf("\n");

    // Example 4: Switching modes
    printf("4. SWITCHING MODES:\n");
    gc = gc_create();
    
    printf("   Starting with MANUAL mode\n");
    gc_set_mode(gc, GC_MODE_MANUAL);
    allocate_test_data(gc, 5);
    
    printf("   Switching to AUTO mode\n");
    gc_set_mode(gc, GC_MODE_AUTO);
    allocate_test_data(gc, 5);
    
    printf("   Switching to HYBRID mode\n");
    gc_set_mode(gc, GC_MODE_HYBRID);
    allocate_test_data(gc, 5);
    
    gc_print_stats(gc);
    gc_destroy(gc);

    printf("\n=== Example completed ===\n");
    return (0);
}
