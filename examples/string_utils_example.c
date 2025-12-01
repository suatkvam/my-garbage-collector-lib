/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils_example.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:55:35 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 14:20:01 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>

int	main(void)
{
	t_gc_context *gc;
	char *str1;
	char *str2;
	char *joined;
	char *partial;

	printf("=== String Utils Example ===\n\n");

	// Create GC context
	gc = gc_create();
	if (!gc)
	{
		printf("Failed to create GC context\n");
		return (1);
	}

	// Example 1: gc_strdup - duplicate string
	printf("1. gc_strdup Example:\n");
	str1 = gc_strdup(gc, "Hello, World!");
	if (str1)
		printf("   Duplicated: %s\n", str1);
	printf("\n");

	// Example 2: gc_strjoin - join two strings
	printf("2. gc_strjoin Example:\n");
	str2 = gc_strdup(gc, "Garbage Collector");
	joined = gc_strjoin(gc, str1, " from ");
	joined = gc_strjoin(gc, joined, str2);
	if (joined)
		printf("   Joined: %s\n", joined);
	printf("\n");

	// Example 3: gc_strndup - duplicate n characters
	printf("3. gc_strndup Example:\n");
	partial = gc_strndup(gc, "Copy only first 5 chars", 5);
	if (partial)
		printf("   Partial copy: %s\n", partial);
	printf("\n");

	// Example 4: Working with string arrays
	printf("4. String Array Example:\n");
	char **names = (char **)gc_malloc(gc, sizeof(char *) * 3);
	if (names)
	{
		names[0] = gc_strdup(gc, "Alice");
		names[1] = gc_strdup(gc, "Bob");
		names[2] = gc_strdup(gc, "Charlie");

		printf("   Names:\n");
		for (int i = 0; i < 3; i++)
			printf("   - %s\n", names[i]);
	}
	printf("\n");

	// Print stats and cleanup
	printf("Statistics:\n");
	gc_print_stats(gc);
	gc_destroy(gc);

	printf("\n=== Example completed ===\n");
	return (0);
}
