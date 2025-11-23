/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_set_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:00:55 by akivam              #+#    #+#           */
/*   Updated: 2025/11/23 16:13:54 by sora             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
	*set garbage collection mode
	*mode: GC_MODE_MANUAL,GC_MODE_AUTO or GC_MODE_HYBRID
*/
void gc_set_mode(t_gc_context *contex, t_gc_mode mode)
{
	if(!contex)
		return ;
	contex->mode = mode;
	if(mode == GC_MODE_AUTO || mode == GC_MODE_HYBRID)
		contex->auto_collect = 1;
	else
		contex->auto_collect = 0;
}
