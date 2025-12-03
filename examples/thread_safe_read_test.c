/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_read_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 20:56:15 by akivam            #+#    #+#             */
/*   Updated: 2025/12/03 20:56:18 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // Only for rand()
#include <unistd.h> // For usleep()

#define DEFAULT_THREADS 4
#define ITERATIONS 1000

typedef struct s_thread_data
{
	t_gc_context	*gc;
	int				thread_id;
	int				*active;
}					t_thread_data;

/*
** READER THREAD:
** Reads statistics. If there are thread-safety issues,
** this might fail while the list is being modified.
*/
void	*stat_reader(void *arg)
{
	t_thread_data	*data;
	t_gc_stats		stats;

	data = (t_thread_data *)arg;
	while (*(data->active))
	{
		// Thread-safe statistics reading test
		gc_get_stats(data->gc, &stats);
		usleep(1000); // Wait 1ms
	}
	printf("Reader Thread %d finished.\n", data->thread_id);
	return (NULL);
}

/*
** WRITER (WORKER) THREAD:
** Continuously allocates/deallocates to stress the list.
*/
void	*alloc_worker(void *arg)
{
	t_thread_data	*data;
	void			*ptr;
	int				i;
	char			*s;

	data = (t_thread_data *)arg;
	i = 0;
	while (i < ITERATIONS)
	{
		if (gc_scope_push(data->gc))
		{
			// Random size allocation
			ptr = gc_malloc(data->gc, 1024 + (i % 1024));
			// String operations test
			if (ptr && (i % 5 == 0))
			{
				s = gc_strdup(data->gc, "Thread Safe Test String");
				(void)s;
			}
			gc_scope_pop(data->gc);
		}
		// Trigger collection occasionally
		if (i % 200 == 0)
			gc_collect(data->gc);
		usleep(500);
		i++;
	}
	printf("Worker Thread %d finished.\n", data->thread_id);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	pthread_t *readers;
	pthread_t *workers;
	t_thread_data *r_data;
	t_thread_data *w_data;
	t_gc_context *gc;
	t_gc_stats final_stats;
	int num_threads;
	int active;
	int i;

	// 1. Using gc_atoi (from src/string/gc_atoi.c)
	if (argc > 1)
		num_threads = gc_atoi(argv[1]);
	else
		num_threads = DEFAULT_THREADS;

	if (num_threads <= 0)
		num_threads = DEFAULT_THREADS;

	printf("=== Thread Safe Read/Write Test ===\n");
	printf("Configuration: %d Reader, %d Worker Threads\n\n", num_threads,
		num_threads);

	gc = gc_create();
	if (!gc)
	{
		fprintf(stderr, "Failed to create GC Context!\n");
		return (1);
	}

	// 2. Using gc_malloc (Allocating test structures using our GC)
	// src/memory/gc_malloc.c
	readers = (pthread_t *)gc_malloc(gc, sizeof(pthread_t) * num_threads);
	workers = (pthread_t *)gc_malloc(gc, sizeof(pthread_t) * num_threads);
	r_data = (t_thread_data *)gc_malloc(gc, sizeof(t_thread_data)
			* num_threads);
	w_data = (t_thread_data *)gc_malloc(gc, sizeof(t_thread_data)
			* num_threads);

	if (!readers || !workers || !r_data || !w_data)
	{
		fprintf(stderr, "Failed to allocate memory for test\n");
		gc_destroy(gc);
		return (1);
	}

	active = 1;

	// Start Worker Threads
	printf("-> Starting worker threads...\n");
	i = 0;
	while (i < num_threads)
	{
		w_data[i].gc = gc;
		w_data[i].thread_id = i;
		w_data[i].active = &active;
		pthread_create(&workers[i], NULL, alloc_worker, &w_data[i]);
		i++;
	}

	// Start Reader Threads
	printf("-> Starting reader threads...\n");
	i = 0;
	while (i < num_threads)
	{
		r_data[i].gc = gc;
		r_data[i].thread_id = i;
		r_data[i].active = &active;
		pthread_create(&readers[i], NULL, stat_reader, &r_data[i]);
		i++;
	}

	// Wait for workers to finish
	i = 0;
	while (i < num_threads)
		pthread_join(workers[i++], NULL);

	printf("\n-> Workers finished. Stopping readers...\n");

	active = 0;

	// Wait for readers to finish
	i = 0;
	while (i < num_threads)
		pthread_join(readers[i++], NULL);

	printf("\n=== Test Completed Successfully ===\n");
	
	// Show final statistics
	gc_get_stats(gc, &final_stats);
	gc_print_stats(gc);

	printf("\nRemaining Allocation Count: %zu (Should be 0)\n",
		final_stats.allocation_count);

	// Clean up context
	gc_destroy(gc);

	return (0);
}