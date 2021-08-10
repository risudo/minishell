#include "libft.h"

static	size_t	count_is(const char *str, size_t i)
{
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	return (i);
}

static	int	re_int(const char *str, size_t i, int sign)
{
	long	num;
	long	tmp_num;
	long	ov_div;
	long	ov_mod;

	ov_div = LONG_MAX / 10;
	ov_mod = LONG_MAX % 10;
	num = 0;
	while ('0' <= str[i] && str[i] <= '9')
	{
		tmp_num = str[i] - '0';
		if (sign == 1 && (ov_div < num
				|| (num == ov_div && ov_mod < tmp_num)))
			return (-1);
		else if (sign == -1 && (ov_div < num
				|| (num == ov_div && ov_mod + 1 < tmp_num)))
			return (0);
		num *= 10;
		num += tmp_num;
		i++;
	}
	return ((int)num * sign);
}

int	ft_atoi(const char *str)
{
	size_t	i;

	i = 0;
	i = count_is(str, i);
	if (str[i] == '+')
		return (re_int(str, i + 1, 1));
	else if (str[i] == '-')
		return (re_int(str, i + 1, -1));
	else
		return (re_int(str, i, 1));
}
