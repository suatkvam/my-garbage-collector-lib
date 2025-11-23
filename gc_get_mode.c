/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_get_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:04:59 by akivam              #+#    #+#             */
/*   Updated: 2025/11/23 16:05:01 by akivam             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
	*get current garbage collection mode
	*returns current mode or GC_MODE_MANUAL if contex is NULL
*/

t_gc_mode gc_get_mode(t_gc_context *contex)
{
	if(!contex)
		return (GC_MODE_MANUAL);
	return (contex->mode);
}