#include "internal_collector.h"

char	*gc_substr(t_gc_context *contex, const char *s, size_t start, size_t len)
{
	char	*substr;
	size_t	s_len;
	size_t	i;

	if (!contex || !s)
		return (NULL);
	s_len = gc_strlen(s);
	if (start >= s_len)
		return (gc_strdup(contex, ""));
	if (start + len > s_len)
		len = s_len - start;
	substr = (char *)gc_malloc(contex, len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}