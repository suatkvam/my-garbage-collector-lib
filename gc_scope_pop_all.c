/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_scope_pop_all.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 23:26:15 by akivam              #+#    #+#             */
/*   Updated: 2025/11/22 23:26:17 by akivam             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
/*
	*pop all scopes from stack
	*cleans up all scope-based allocations at once
	*used for cleanup or error recovery
*/
void gc_scope_pop_all(t_gc_context *contex)
{
	if(!contex)
		return ;
	while(contex->current_scope)
		gc_scope_pop(contex);
}
