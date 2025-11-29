#include "internal_collector.h"

char	*gc_strchr(const char *s, int c)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = 0;
	while (str[i] != '\0')
	{
		if ((unsigned char)(str[i]) == (unsigned char)c)
			return (str + i);
		i++;
	}
	if ((unsigned char)c == '\0')
		return (str + i);
	return (NULL);
}