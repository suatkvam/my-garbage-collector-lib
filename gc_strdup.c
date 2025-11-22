/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 23:49:20 by akivam              #+#    #+#             */
/*   Updated: 2025/11/22 23:49:22 by akivam             ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/* calculate string length*/
static size_t gc_strlen(const char *s)
{
	int i;
	
	i = 0;
	while(s[i])
		i++;
	return i;
}

/*
	*duplicate string with gc tracking
	*s: source string to duplicate
	*returns new string or NULL on failure
*/
char *gc_strdup(t_gc_context *contex, const char *s)
{
	char *duplicate;
	size_t len;
	size_t i;

	if(!contex || !s)
		return(NULL);
	len = gc_strlen(s);
	duplicate = (char *)gc_malloc(contex, len + 1);
	if(!duplicate)
		return (NULL);
	i = 0;
	while (i < len)
	{
		duplicate[i] = s[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}
