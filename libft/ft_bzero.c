#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ans;

	ans = (unsigned char *)s;
	while (n--)
		*(ans++) = 0;
}
