/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_get_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:00:12 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:00:12 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
 *get current garbage collection mode
 *returns current mode or GC_MODE_MANUAL if contex is NULL
 */

t_gc_mode	gc_get_mode(t_gc_context *contex)
{
	t_gc_mode	mode;

	if (!contex)
		return (GC_MODE_MANUAL);
	pthread_mutex_lock(&contex->lock);
	mode = contex->mode;
	pthread_mutex_unlock(&contex->lock);
	return (contex->mode);
}
