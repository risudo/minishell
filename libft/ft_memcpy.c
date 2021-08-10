#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;

	if (!(dst || src))
		return (NULL);
	s1 = (unsigned char *)dst;
	s2 = (unsigned char *)src;
	while (n--)
		*(s1++) = *(s2++);
	return (dst);
}
