#include "internal_collector.h"

static int	gc_unumlen(size_t n)
{
	int	len;

	len = 1;
	while (n >= 10)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*gc_uitoa(t_gc_context *contex, size_t n)
{
	char	*str;
	int		len;

	if (!contex)
		return (NULL);
	len = gc_unumlen(n);
	str = (char *)gc_malloc(contex, len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}
