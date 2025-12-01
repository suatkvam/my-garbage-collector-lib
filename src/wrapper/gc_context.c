/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_context.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 00:00:00 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"

/*
**	gc_wrapper_push_context - activate GC context for malloc/free
**	@ctx: context to use for subsequent allocations
**
**	Push context onto internal stack. All malloc calls will use this context.
**	Supports nested contexts (max 16 levels).
*/
void	gc_wrapper_push_context(t_gc_context *ctx)
{
	if (!ctx || g_ctx_stack.top >= MAX_GC_CONTEXTS - 1)
		return ;
	g_ctx_stack.contexts[++g_ctx_stack.top] = ctx;
}

/*
**	gc_wrapper_pop_context - deactivate current GC context
**
**	Pop context from stack. Falls back to previous context or real malloc.
*/
void	gc_wrapper_pop_context(void)
{
	if (g_ctx_stack.top >= 0)
		g_ctx_stack.top--;
}

t_gc_context	*gc_get_current(void)
{
	if (g_ctx_stack.top < 0)
		return (NULL);
	return (g_ctx_stack.contexts[g_ctx_stack.top]);
}
