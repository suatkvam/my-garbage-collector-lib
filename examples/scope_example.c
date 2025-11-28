/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scope_example.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:55:35 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:55:35 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>

void inner_function(t_gc_context *gc)
{
    char *temp;

    printf("   Entering inner function scope\n");
    
    // Push new scope
    gc_scope_push(gc);
    
    // Allocate in inner scope
    temp = gc_strdup(gc, "Temporary data in inner scope");
    printf("   Allocated: %s\n", temp);
    
    // Pop scope - frees all allocations in this scope
    printf("   Popping inner scope...\n");
    gc_scope_pop(gc);
    printf("   Inner scope memory freed\n");
}

int main(void)
{
    t_gc_context *gc;
    char *global_data;
    t_gc_stats stats;

    printf("=== Scope Management Example ===\n\n");

    gc = gc_create();
    if (!gc)
        return (1);

    // Example 1: Basic scope usage
    printf("1. Basic Scope Usage:\n");
    global_data = gc_strdup(gc, "Global data");
    printf("   Global allocation: %s\n\n", global_data);

    // Example 2: Nested scopes
    printf("2. Nested Scopes:\n");
    
    // Outer scope
    gc_scope_push(gc);
    char *outer = gc_strdup(gc, "Outer scope");
    printf("   Outer scope: %s\n", outer);
    
    // Inner scope
    gc_scope_push(gc);
    char *inner = gc_strdup(gc, "Inner scope");
    printf("   Inner scope: %s\n", inner);
    
    // Pop inner scope only
    gc_scope_pop(gc);
    printf("   Inner scope popped\n");
    
    // Outer still exists
    printf("   Outer still exists: %s\n", outer);
    
    // Pop outer scope
    gc_scope_pop(gc);
    printf("   Outer scope popped\n\n");

    // Example 3: Function with scope
    printf("3. Function Scope:\n");
    inner_function(gc);
    printf("\n");

    // Example 4: Scope statistics
    printf("4. Current Statistics:\n");
    gc_get_stats(gc, &stats);
    printf("   Scope depth: %zu\n", stats.scope_depth);
    printf("   Total allocated: %zu bytes\n", stats.total_allocated);
    printf("   Current usage: %zu bytes\n\n", stats.current_usage);

    // Example 5: Pop all scopes
    printf("5. Pop All Scopes:\n");
    gc_scope_push(gc);
    gc_scope_push(gc);
    gc_scope_push(gc);
    printf("   Created 3 nested scopes\n");
    gc_scope_pop_all(gc);
    printf("   All scopes popped\n\n");

    gc_destroy(gc);
    printf("=== Example completed ===\n");
    return (0);
}
