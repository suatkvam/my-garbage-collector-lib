/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_set_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:00:12 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:00:12 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
 *enable or disable debug mode
 *enabled: 1 to enable, 0 to disable
 */

void	gc_set_debug(t_gc_context *contex, int enabled)
{
	t_gc_mode	mode;

	if (!contex)
		return ;
	pthread_mutex_lock(&contex->lock);
	mode = contex->mode;
	pthread_mutex_unlock(&contex->lock);
	contex->debug_mode = enabled;
}
