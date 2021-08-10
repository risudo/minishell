#include "libft.h"

static	size_t	cnt_digit(long n)
{
	size_t	cnt;

	cnt = 1;
	while (n > 9)
	{
		n /= 10;
		cnt++;
	}
	return (cnt);
}

static	char	*set_str(char *ans, long nb, size_t len, size_t msign)
{
	ans[--len] = '\0';
	if (nb == 0)
		ans[--len] = '0';
	while (nb > 0)
	{
		ans[--len] = nb % 10 + '0';
		nb /= 10;
	}
	if (msign)
		ans[--len] = '-';
	return (&ans[len]);
}

char	*ft_itoa(int n)
{
	size_t	msign;
	long	nb;
	size_t	digit;
	char	*ans;
	size_t	len;

	nb = n;
	msign = 0;
	if (nb < 0)
	{
		msign = 1;
		nb *= -1;
	}
	digit = cnt_digit(nb);
	len = digit + msign + 1;
	ans = (char *)malloc(sizeof(char) * len);
	if (!ans)
		return (NULL);
	return (set_str(ans, nb, len, msign));
}
