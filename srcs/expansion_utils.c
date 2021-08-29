#include "../includes/parse.h"

bool	is_delimiter(char c)
{
	if (c == ' ' || c == '$' || c == '\"' || c == '\'' || c == '\0')
		return (true);
	else
		return (false);
}

char	*ft_strjoin_three(char *str1, char *str2, char *str3)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(str1, str2);//TODO: use wrapper
	ret = ft_strjoin(tmp, str3);
	xfree(tmp);
	return (ret);
}

size_t	ft_strlen_excluded_quot(char *str, char *quot)
{
	size_t	len;
	size_t	i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && quot[i] == '1')|| (str[i] == '\"' && quot[i] == '2'))
		{
			i++;
			continue ;
		}
		len++;
		i++;
	}
	return (len);
}

bool	is_delimiter_quot(char c, char flag)
{
	if (((c == '\'' && (flag == '1' || flag == 'S'))
			|| (c == '\"' && (flag == '2' || flag == 'D'))))
	{
		return (true);
	}
	else
		return (false);
}

char	*get_new_flag(char **quot, char *end_quot)
{
	char	*ret;
	long	i;
	long	j;

	i = 0;
	j = 0;
	ret = ft_xcalloc(ft_strlen(*quot), sizeof(char));
	while ((*quot)[j])
	{
		if (j == end_quot - *quot || j == end_quot - *quot - 1)
		{
			j++;
			continue ;
		}
		ret[i++] = (*quot)[j++];
	}
	xfree(*quot);
	return (ret);
}

// return : the new flag
char	*get_removed_endflag(char **quot, char flag)
{
	char	*end_quot;

	end_quot = ft_strchr(*quot, flag);
	while (end_quot != NULL)
	{
		*quot = get_new_flag(quot, end_quot);
		end_quot = ft_strchr(*quot, flag);
	}
	return (*quot);
}
