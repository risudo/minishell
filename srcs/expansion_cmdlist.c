#include "minishell.h"

/*
** Serch cmdlist and do the following.
** 1 Expand environment valiables.
** 2 Clear quotation.
** 3 If there are spaces after expanding, insert new list.
*/

static size_t	expansion_key_cmdlist(t_cmdlist *clist,
		t_envlist *envlist, char *doll_ptr)
{
	char	*key;
	char	*value;
	char	*front_key;
	char	*back_key;
	size_t	len;

	len = 1;
	while (!is_delimiter(doll_ptr[len]))
		len++;
	front_key = ft_xsubstr(clist->str, 0, doll_ptr - clist->str);
	key = ft_xsubstr(doll_ptr, 1, len - 1);
	back_key = ft_xsubstr(doll_ptr + len, 0, ft_strlen(doll_ptr + len));
	value = ft_getenv(envlist, key);
	xfree(clist->str);
	clist->str = ft_strjoin_three(front_key, value, back_key);
	len = ft_strlen(front_key) + ft_strlen(value);
	xfree(front_key), xfree(key), xfree(back_key);
	return (len);
}

static void	clear_quot_cmdlist(t_cmdlist *clist)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new_str = (char *)ft_xcalloc(
			ft_strlen_excluded_quot(clist->str, clist->quot) + 1,
			sizeof(char));
	while (clist->str[i])
	{
		if (is_delimiter_quot(clist->str[i], clist->quot[i]))
		{
			i++;
			continue ;
		}
		new_str[j++] = clist->str[i++];
	}
	xfree(clist->str);
	clist->str = new_str;
	clist->quot = get_removed_endflag(&clist->quot, '1');
	clist->quot = get_removed_endflag(&clist->quot, '2');
}

static t_cmdlist	*insert_new_cmdlist(t_cmdlist *clist, int i)
{
	t_cmdlist	*new;
	size_t		len;

	while (ft_isspace(clist->str[i]))
		i++;
	len = ft_strlen(clist->str + i);
	if (len == 0)
		return (clist);
	new = (t_cmdlist *)ft_xcalloc(1, sizeof(*new));
	new->str = ft_xsubstr(clist->str, i, len);
	new->quot = get_quot_flag(new->str);
	new->next = clist->next;
	clist->next = new;
	return (clist);
}

static void	serch_new_space_cmdlist(t_cmdlist *clist)
{
	size_t	i;
	size_t	len;
	char	*str;

	i = get_space_idx(clist);
	if (i == 0 && ft_isspace(clist->str[i]))
	{
		while (ft_isspace(clist->str[i]))
			i++;
		len = ft_strlen(clist->str + i);
		str = ft_xsubstr(clist->str, i, len);
		free(clist->str), free(clist->quot);
		clist->str = str;
		clist->quot = get_quot_flag(clist->str);
		serch_new_space_cmdlist(clist);
	}
	else if (ft_isspace(clist->str[i]))
	{
		str = ft_xsubstr(clist->str, 0, i);
		clist = insert_new_cmdlist(clist, i);
		free(clist->str), free(clist->quot);
		clist->str = str;
		clist->quot = get_quot_flag(clist->str);
	}
}

void	serch_env_cmdlist(t_cmdlist *clist, t_envlist *envlist)
{
	char	*doll_ptr;
	size_t	len;

	while (clist)
	{
		doll_ptr = ft_strdoll(clist->str);
		while (doll_ptr != NULL && clist->quot[doll_ptr - clist->str] != 'S')
		{
			len = expansion_key_cmdlist(clist, envlist, doll_ptr);
			xfree(clist->quot);
			clist->quot = get_quot_flag(clist->str);
			doll_ptr = ft_strdoll(clist->str + len);
		}
		if (ft_strchr(clist->quot, '1') || ft_strchr(clist->quot, '2'))
			clear_quot_cmdlist(clist);
		serch_new_space_cmdlist(clist);
		clist = clist->next;
	}
}
