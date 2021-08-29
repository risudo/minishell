#include "../includes/parse.h"

bool	is_env_iolist(char flag, t_iolist *prev)
{
	if (flag != 'S' && (prev == NULL || prev->c_type != IN_HERE_DOC))
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
