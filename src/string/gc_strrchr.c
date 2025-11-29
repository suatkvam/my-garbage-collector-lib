#include "internal_collector.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	size_t	i;
	if(!s)
		return NULL;
	str = (char *)s;
	i = 0;
	while (str[i] != '\0')
		i = i + 1;
	if ((char)c == '\0')
		return (str + i);
	while (i-- > 0)
	{
		if (str[i] == (char)c)
			return (str + i);
	}
	return (NULL);
}
