#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*str;
	char	ch;
	size_t	i;

	str = (char *)s;
	ch = (char)c;
	i = 0;
	while (1)
	{
		if (str[i] == ch)
			return (&str[i]);
		if (!str[i])
			break ;
		i++;
	}
	return (NULL);
}
