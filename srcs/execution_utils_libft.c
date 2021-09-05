#include "minishell.h"

static bool	is_overflow(unsigned long num, \
						int sign, bool *nonnum_check)
{
	if ((sign == 1 && LONG_MAX < num) || \
		(sign == -1 && LONG_MAX < num - 1))
		return (true);
	*nonnum_check = false;
	return (false);
}

static size_t	pass_isspace(char *str, size_t i)
{
	while(ft_isspace(str[i]))
		i++;
	return (i);
}

long	ft_atol(char *str, bool *nonnum_check)
{
	size_t			i;
	unsigned long	num;
	int				sign;

	i = 0;
	num = 0;
	sign = 1;
	i = pass_isspace(str, i);
	if (str[i] == '+')
		i++;
	else if (str[i] == '-' && ++i)
		sign = -1;
	*nonnum_check = true;
	while (str[i] && ft_isdigit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		if (is_overflow(num, sign, nonnum_check))
			break ;
		i++;
	}
	i = pass_isspace(str, i);
	if (i == 0 || str[i] != '\0')
		*nonnum_check = true;
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
