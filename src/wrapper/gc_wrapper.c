/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_wrapper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:54:49 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:54:54 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"

#define MAX_GC_CONTEXTS 16

typedef struct s_gc_stack
{
	t_gc_context	*contexts[MAX_GC_CONTEXTS];
	int				top;
}	t_gc_stack;

static t_gc_stack	g_ctx_stack = {.top = -1};

void	*__real_malloc(size_t size);
void	*__real_calloc(size_t nmemb, size_t size);
void	*__real_realloc(void *ptr, size_t size);
void	__real_free(void *ptr);

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

static t_gc_context	*gc_get_current(void)
{
	if (g_ctx_stack.top < 0)
		return (NULL);
	return (g_ctx_stack.contexts[g_ctx_stack.top]);
}

void	*__wrap_malloc(size_t size)
{
	t_gc_context	*ctx;

	ctx = gc_get_current();
	if (!ctx)
		return (__real_malloc(size));
	return (gc_malloc(ctx, size));
}

void	*__wrap_calloc(size_t nmemb, size_t size)
{
	t_gc_context	*ctx;

	ctx = gc_get_current();
	if (!ctx)
		return (__real_calloc(nmemb, size));
	return (gc_calloc(ctx, nmemb, size));
}

void	*__wrap_realloc(void *ptr, size_t size)
{
	t_gc_context	*ctx;

	ctx = gc_get_current();
	if (!ctx)
		return (__real_realloc(ptr, size));
	return (gc_realloc(ctx, ptr, size));
}

void	__wrap_free(void *ptr)
{
	t_gc_context	*ctx;

	ctx = gc_get_current();
	if (!ctx)
		__real_free(ptr);
}
