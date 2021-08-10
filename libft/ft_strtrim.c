#include "libft.h"

static	int	detect_set(char const c, char const *set)
{
	int	i;

	i = -1;
	while (set[++i])
	{
		if (c == set[i])
			break ;
	}
	if (set[i])
		return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	while (detect_set(s1[i], set) && s1[i])
		i++;
	j = ft_strlen(s1) - 1;
	while (detect_set(s1[j], set) && i < j)
		j--;
	return (ft_substr(s1, i, (j - i + 1)));
}
