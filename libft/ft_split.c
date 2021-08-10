#include "libft.h"

static	int	cnt_row(char const *s, char c)
{
	int	row_nb;
	int	flag;

	flag = 1;
	row_nb = 0;
	while (*s)
	{
		if (*s == c)
		{
			if (flag == 0)
				flag = 1;
		}
		else
		{
			if (flag == 1)
			{
				row_nb++;
				flag = 0;
			}
		}
		s++;
	}
	return (row_nb);
}

static	int	cnt_len(char const *s, char c)
{
	int	len;

	len = 0;
	while (*s)
	{
		if (*s == c)
			break ;
		len++;
		s++;
	}
	return (len);
}

static	char	*malloc_str(char **ans, int i, int len)
{
	ans[i] = (char *)malloc(sizeof(char) * (len + 1));
	if (!ans[i])
	{
		while (--i >= 0)
			free(ans[i]);
		free(ans);
		return (NULL);
	}
	return (ans[i]);
}

static	char	**set_str(char const *s, char c, char **ans, int row_nb)
{
	int	i;
	int	j;
	int	len;

	i = -1;
	while (++i < row_nb)
	{
		while (1)
		{
			len = cnt_len(s, c);
			if (len)
				break ;
			s++;
		}
		if (!malloc_str(ans, i, len))
			return (NULL);
		j = 0;
		while (len--)
			ans[i][j++] = *s++;
		ans[i][j] = '\0';
	}
	ans[i] = NULL;
	return (ans);
}

char	**ft_split(char const *s, char c)
{
	char	**ans;
	int		row_nb;

	if (!s)
		return (NULL);
	row_nb = cnt_row(s, c);
	ans = (char **)malloc(sizeof(char *) * (row_nb + 1));
	if (!ans)
		return (NULL);
	if (!set_str(s, c, ans, row_nb))
		return (NULL);
	return (ans);
}
