/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_example.c                                    :+:      :+:    :+:   */
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
    int *numbers;
    char *message;
    int i;

    printf("=== Basic Usage Example ===\n\n");

    // Step 1: Create garbage collector context
    printf("1. Creating GC context...\n");
    gc = gc_create();
    if (!gc)
    {
        printf("Error: Failed to create GC context\n");
        return (1);
    }
    printf("   GC context created successfully\n\n");

    // Step 2: Allocate integer array with gc_malloc
    printf("2. Allocating integer array (10 elements)...\n");
    numbers = (int *)gc_malloc(gc, sizeof(int) * 10);
    if (!numbers)
    {
        printf("Error: Memory allocation failed\n");
        gc_destroy(gc);
        return (1);
    }

    // Fill array with values
    i = 0;
    while (i < 10)
    {
        numbers[i] = i * 10;
        i++;
    }

    printf("   Array values: ");
    i = 0;
    while (i < 10)
    {
        printf("%d ", numbers[i]);
        i++;
    }
    printf("\n\n");

    // Step 3: Allocate string with gc_strdup
    printf("3. Allocating string...\n");
    message = gc_strdup(gc, "Hello from GC!");
    if (message)
        printf("   Message: %s\n\n", message);

    // Step 4: Use gc_calloc (allocate and zero-initialize)
    printf("4. Using gc_calloc (zeroed memory)...\n");
    int *zeroed = (int *)gc_calloc(gc, 5, sizeof(int));
    if (zeroed)
    {
        printf("   Zeroed array: ");
        i = 0;
        while (i < 5)
        {
            printf("%d ", zeroed[i]);
            i++;
        }
        printf("\n\n");
    }

    // Step 5: Show statistics
    printf("5. Current statistics:\n");
    gc_print_stats(gc);
    printf("\n");

    // Step 6: Manual collection
    printf("6. Running manual collection...\n");
    gc_collect(gc);
    printf("   Collection completed\n\n");

    // Step 7: Cleanup
    printf("7. Destroying GC context...\n");
    gc_destroy(gc);
    printf("   All memory freed\n");

    printf("\n=== Example completed ===\n");
    return (0);
}
