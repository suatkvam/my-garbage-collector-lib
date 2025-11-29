#include "internal_collector.h"

int gc_isspace(int c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	gc_atoi(const char *str)
{
	int	result;
	int	sign;

	if (!str)
		return (0);
	result = 0;
	sign = 1;
	while (gc_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
