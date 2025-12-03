/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:34:37 by akivam            #+#    #+#             */
/*   Updated: 2025/12/03 16:34:39 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 8
#define ALLOCS_PER_THREAD 1000

typedef struct s_thread_data
{
	t_gc_context	*gc;
	int				thread_id;
	int				*success;
}					t_thread_data;

void	*thread_worker(void *arg)
{
	t_thread_data	*data;
	int				i;
	void			*ptr;

	data = (t_thread_data *)arg;
	i = 0;
	while (i < ALLOCS_PER_THREAD)
	{
		ptr = gc_malloc(data->gc, sizeof(int) * 100);
		if (!ptr)
		{
			*data->success = 0;
			return (NULL);
		}
		if (i % 100 == 0)
			gc_collect(data->gc);
		i++;
	}
	printf("Thread %d: %d allocations OK\n", data->thread_id,
		ALLOCS_PER_THREAD);
	return (NULL);
}

int	test_concurrent_alloc(void)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	data[NUM_THREADS];
	t_gc_context	*gc;
	t_gc_stats		stats;
	int				success;
	int				i;

	gc = gc_create();
	if (!gc)
		return (0);
	success = 1;
	i = 0;
	while (i < NUM_THREADS)
	{
		data[i].gc = gc;
		data[i].thread_id = i;
		data[i].success = &success;
		pthread_create(&threads[i], NULL, thread_worker, &data[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	gc_get_stats(gc, &stats);
	printf("\nTotal: %zu allocs\n", stats.allocation_count);
	gc_destroy(gc);
	return (success);
}

void	*scope_thread(void *arg)
{
	t_thread_data	*data;
	int				i;
	void			*ptr;

	data = (t_thread_data *)arg;
	i = 0;
	while (i < 500)
	{
		gc_scope_push(data->gc);
		ptr = gc_malloc(data->gc, sizeof(double) * 50);
		if (!ptr)
		{
			*data->success = 0;
			return (NULL);
		}
		gc_scope_pop(data->gc);
		i++;
	}
	printf("Thread %d: scope OK\n", data->thread_id);
	return (NULL);
}

int	test_concurrent_scopes(void)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	data[NUM_THREADS];
	t_gc_context	*gc;
	t_gc_stats		stats;
	int				success;
	int				i;

	gc = gc_create();
	if (!gc)
		return (0);
	success = 1;
	i = 0;
	while (i < NUM_THREADS)
	{
		data[i].gc = gc;
		data[i].thread_id = i;
		data[i].success = &success;
		pthread_create(&threads[i], NULL, scope_thread, &data[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	gc_get_stats(gc, &stats);
	printf("Remaining: %zu allocs\n", stats.allocation_count);
	gc_destroy(gc);
	return (success);
}

void	*string_thread(void *arg)
{
	t_thread_data	*data;
	char			*str;
	int				i;

	data = (t_thread_data *)arg;
	i = 0;
	while (i < 300)
	{
		str = gc_strdup(data->gc, "Thread-safe test");
		str = gc_strjoin(data->gc, str, " OK");
		if (!str)
		{
			*data->success = 0;
			return (NULL);
		}
		i++;
	}
	printf("Thread %d: string OK\n", data->thread_id);
	return (NULL);
}

int	test_concurrent_strings(void)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	data[NUM_THREADS];
	t_gc_context	*gc;
	int				success;
	int				i;

	gc = gc_create();
	if (!gc)
		return (0);
	success = 1;
	i = 0;
	while (i < NUM_THREADS)
	{
		data[i].gc = gc;
		data[i].thread_id = i;
		data[i].success = &success;
		pthread_create(&threads[i], NULL, string_thread, &data[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
	gc_destroy(gc);
	return (success);
}

int	main(void)
{
	printf("=== GC Thread-Safety Tests ===\n\n");
	printf("[1/3] Concurrent Allocations.. .\n");
	if (!test_concurrent_alloc())
	{
		printf("❌ FAILED\n");
		return (1);
	}
	printf("✅ PASSED\n\n");
	printf("[2/3] Concurrent Scopes...\n");
	if (!test_concurrent_scopes())
	{
		printf("❌ FAILED\n");
		return (1);
	}
	printf("✅ PASSED\n\n");
	printf("[3/3] Concurrent Strings...\n");
	if (!test_concurrent_strings())
	{
		printf("❌ FAILED\n");
		return (1);
	}
	printf("✅ PASSED\n\n");
	printf("🎉 All thread-safety tests passed!\n");
	return (0);
}
