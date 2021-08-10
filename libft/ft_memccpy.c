#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char		*s1;
	unsigned char		*s2;
	unsigned char		ch;

	s1 = (unsigned char *)dst;
	s2 = (unsigned char *)src;
	ch = (unsigned char)c;
	while (n--)
	{
		*s1 = *s2;
		if (*s2 == ch)
			return ((void *)(s1 + 1));
		s1++;
		s2++;
	}
	return (NULL);
}
