#include "minishell.h"

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
	free(*quot);
	return (ret);
}

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

int	delone_cmdlist(t_cmdlist **cur, t_cmdlist *prev, t_cmdlist **head)
{
	t_cmdlist	*tmp;

	if (*cur == *head)
	{
		tmp = (*cur);
		(*cur) = (*cur)->next;
		free(tmp->str), free(tmp->quot), free(tmp);
		(*head) = (*cur);
	}
	else
	{
		tmp = (*cur);
		(*cur) = (*cur)->next;
		prev->next = (*cur);
		free(tmp->str), free(tmp->quot), free(tmp);
	}
	return (1);
}

char	*ft_strdol(char *s, char *quot, char *head)
{
	char	*doll_ptr;

	doll_ptr = ft_strchr(s, '$');
	if (doll_ptr
		&& (is_delimiter(*(doll_ptr + 1))
			|| quot[doll_ptr - head] == 'S'))
		return (ft_strdol(doll_ptr + 1, quot, head));
	else
		return (doll_ptr);
}
