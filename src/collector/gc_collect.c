/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_collect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/11/28 19:57:00 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:57:00 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/* Internal collection without lock (called when already locked) */
static void	gc_collect_internal(t_gc_context *contex)
{
	if (!contex)
		return ;
	gc_mark(contex);
	gc_sweep(contex);
	contex->last_collect_count = contex->allocation_count;
}
/*
 *run garbage collection cycle
 *performs mark-and-sweep to free unreachable allocations,
 *updates last_collect_count for auto-collection tracking
 */

void	gc_collect(t_gc_context *contex)
{
	if (!contex)
		return ;
	pthread_mutex_lock(&contex->lock);
	gc_collect_internal(contex);
	pthread_mutex_unlock(&contex->lock);
}
