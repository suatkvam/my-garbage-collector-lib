/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 23:32:32 by akivam              #+#    #+#             */
/*   Updated: 2025/11/22 23:32:37 by akivam             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*free all remaining allocations in global list*/
static void gc_free_all_allocations(t_gc_context *contex)
{
	t_gc_allocation *current;
	t_gc_allocation *next;

	current = contex->all_allocations;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
}

/*free all remaining scopes in stack*/
static void gc_free_all_scopes(t_gc_context *contex)
{
	t_gc_scope *current;
	t_gc_scope *next;

	current = contex->current_scope;
	while (current)
	{
		next = current->prev;
		free(current);
		current = next;
	}
}

/*
	*destroy gc context and free all resources
	*cleans up all allocations and scopes
*/

void gc_destroy(t_gc_context *contex)
{
	if(!contex)
		return ;
	gc_free_all_allocations(contex);
	gc_free_all_scopes(contex);
	free(contex);
}
