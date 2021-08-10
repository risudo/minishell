#include "libft.h"

static	char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *src)
{
	char	*dest;
	size_t	src_len;

	src_len = ft_strlen(src);
	dest = (char *)malloc(sizeof(char) * (src_len + 1));
	if (!dest)
		return (NULL);
	return (ft_strcpy(dest, src));
}
