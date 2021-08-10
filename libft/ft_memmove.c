#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*s1;
	unsigned char		*s2;

	s1 = (unsigned char *)dst;
	s2 = (unsigned char *)src;
	if (s1 > s2)
	{
		s1 += len;
		s2 += len;
		while (len--)
			*(--s1) = *(--s2);
	}
	else if (s1 < s2)
	{
		while (len--)
			*(s1++) = *(s2++);
	}
	return (dst);
}
