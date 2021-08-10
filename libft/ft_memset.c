#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ans;
	unsigned char	ch;

	ans = (unsigned char *)b;
	ch = (unsigned char)c;
	while (len--)
		*(ans++) = ch;
	return (b);
}
