#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	char	ch;
	size_t	i;

	str = (char *)s;
	ch = (char)c;
	i = ft_strlen(str);
	while (1)
	{
		if (str[i] == ch)
			return (&str[i]);
		if (i == 0)
			break ;
		i--;
	}
	return (NULL);
}
