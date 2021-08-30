#include "minishell.h"

/*
** Serch cmdlist and do the following.
** 1 Expand environment valiables.
** 2 Clear quotation.
** 3 If there are spaces after expanding, insert new list.
*/

static void	expansion_key_cmdlist(t_cmdlist *clist,
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
	xfree(front_key), xfree(key), xfree(back_key);
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

	new = (t_cmdlist *)ft_xcalloc(1, sizeof(*new));
	while (clist->str[i] == ' ')
		i++;
	len = ft_strlen(clist->str + i);
	new->str = ft_xsubstr(clist->str, i, len);
	new->quot = get_quot_flag(new->str);
	new->next = clist->next;
	clist->next = new;
	return (clist);
}

static void	serch_new_space_cmdlist(t_cmdlist *clist)
{
	size_t	i;
	char	*str;

	i = 0;
	while (clist->str[i])
	{
		if (clist->str[i] == ' ' && clist->quot[i] == '0')
		{
			str = ft_xsubstr(clist->str, 0, i);
			clist = insert_new_cmdlist(clist, i);
			xfree(clist->str);
			xfree(clist->quot);
			clist->str = str;
			clist->quot = get_quot_flag(clist->str);
			i--;
		}
		i++;
	}
}

void	serch_env_cmdlist(t_cmdlist *clist, t_envlist *envlist)
{
	char	*doll_ptr;

	while (clist)
	{
		doll_ptr = ft_strchr(clist->str, '$');
		while (doll_ptr != NULL && clist->quot[doll_ptr - clist->str] != 'S')
		{
			expansion_key_cmdlist(clist, envlist, doll_ptr);
			xfree(clist->quot);
			clist->quot = get_quot_flag(clist->str);
			doll_ptr = ft_strchr(clist->str, '$');
		}
		if (ft_strchr(clist->quot, '1') || ft_strchr(clist->quot, '2'))
			clear_quot_cmdlist(clist);
		serch_new_space_cmdlist(clist);
		clist = clist->next;
	}
}
