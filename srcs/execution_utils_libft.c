#include "execute.h"

static bool	check_nonnum_overflow(unsigned long num, \
						char *str, int sign, size_t i)
{
	if (ft_isdigit(str[i]) == 0 || \
		(sign == 1 && LONG_MAX < num) || \
		(sign == -1 && LONG_MAX < num - 1))
		return (true);
	return (false);
}

long	ft_atol(char *str, bool *nonnum_check)
{
	size_t			i;
	unsigned long	num;
	int				sign;

	*nonnum_check = false;
	i = 0;
	num = 0;
	sign = 1;
	if (str[i] == '\0')
		*nonnum_check = true;
	else if (str[i] == '+')
		i++;
	else if (str[i] == '-' && ++i)
		sign = -1;
	while (*nonnum_check == false && str[i])
	{
		num *= 10;
		num += str[i] - '0';
		*nonnum_check = check_nonnum_overflow(num, str, sign, i);
		i++;
	}
	return ((long)num * sign);
}

int	ft_strcmp(char *s1, char *s2)
{
	if (s1 == NULL)
		return (-1);
	if (s2 == NULL)
		return (1);
	while (1)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		if (*s1 == 0)
			return (0);
		s1++;
		s2++;
	}
}

void	free_2d_array(char **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			free(array[i]);
		free(array);
	}
}
