/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 00:03:51 by akivam              #+#    #+#             */
/*   Updated: 2025/11/23 00:03:53 by akivam             ###   ########.tr       */
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

/*join two strings with gc tracking*/
char *gc_strjoin(t_gc_context *contex, const char *s1, const char *s2)
{
	char *joined;
	size_t len1;
	size_t len2;
	size_t i;

	if(!contex || !s1 || !s2)
		return(NULL);
	len1 = gc_strlen(s1);
	len2 = gc_strlen(s2);
	joined = (char *)gc_malloc(contex,len1 + len2 + 1);
	if(!joined)
		return (NULL);
	i = -1;
	while (++i < len1)
	{
		joined[i] = s1[i];
	}
	i = -1;
	while (++i < len2)
	{
		joined[len1 + i] = s2[i];
	}
	joined[len1 + len2] = '\0';
	return (joined);
}
