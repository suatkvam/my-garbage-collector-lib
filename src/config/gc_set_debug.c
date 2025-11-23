/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_set_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sora <username@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:14:09 by sora              #+#    #+#             */
/*   Updated: 2025/11/23 16:14:12 by sora             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
/*
	*enable or disable debug mode
	*enabled: 1 to enable, 0 to disable
*/

void gc_set_debug(t_gc_context *contex, int enabled)
{
	if(!contex)
		return ;
	contex->debug_mode = enabled;
}
