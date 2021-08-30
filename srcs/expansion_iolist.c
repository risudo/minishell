#include "minishell.h"

/*
** Serch iolist and do the following.
** 1 Expand environment valiables.
** 2 Clear quotation.
** 3 If there are spaces after expanding, insert new list.
*/

static void	expansion_key_iolist(t_iolist *iolist,
		t_envlist *envlist, char *doll_ptr)
{
	char	*key;
	char	*value;
	char	*front;
	char	*back;
	size_t	len;

	len = 1;
	while (!is_delimiter(doll_ptr[len]))
		len++;
	key = ft_xsubstr(doll_ptr, 1, len - 1);
	front = ft_xsubstr(iolist->str, 0, doll_ptr - iolist->str);
	back = ft_xsubstr(doll_ptr + len, 0, ft_strlen(doll_ptr + len));
	value = ft_getenv(envlist, key);
	xfree(iolist->str);
	iolist->str = ft_strjoin_three(front, value, back);
	xfree(front), xfree(key), xfree(back);
}

static void	clear_quot_iolist(t_iolist *iolist)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new_str = ft_xcalloc(ft_strlen_excluded_quot(iolist->str, iolist->quot) + 1,
			sizeof(char));
	while (iolist->str[i])
	{
		if (is_delimiter_quot(iolist->str[i], iolist->quot[i]))
		{
			i++;
			continue ;
		}
		new_str[j++] = iolist->str[i++];
	}
	xfree(iolist->str);
	iolist->str = new_str;
	iolist->quot = get_removed_endflag(&iolist->quot, '1');
	iolist->quot = get_removed_endflag(&iolist->quot, '2');
}

static void	insert_new_iolist(t_iolist *iolist, size_t i)
{
	t_iolist	*new;
	size_t		len;

	new = (t_iolist *)ft_xcalloc(1, sizeof(*new));
	while (iolist->str[i] == ' ')
		i++;
	len = ft_strlen(iolist->str + i);
	new->str = ft_xsubstr(iolist->str, i, len);
	new->quot = get_quot_flag(new->str);
	new->c_type = ELSE;
	new->next = iolist->next;
	iolist->next = new;
}

static void	serch_new_space_iolist(t_iolist *iolist)
{
	size_t	i;
	char	*str;

	i = 0;
	while (iolist->str[i])
	{
		if (iolist->str[i] == ' ' && iolist->quot[i] == '0')
		{
			str = ft_xsubstr(iolist->str, 0, i);
			insert_new_iolist(iolist, i);
			xfree(iolist->str);
			xfree(iolist->quot);
			iolist->str = str;
			iolist->quot = get_quot_flag(iolist->str);
			i--;
		}
		i++;
	}
}

void	serch_env_iolist(t_iolist *iolist, t_envlist *envlist)
{
	t_iolist	*prev;
	char		*doll_ptr;

	prev = NULL;
	while (iolist)
	{
		doll_ptr = ft_strchr(iolist->str, '$');
		while (doll_ptr
			&& is_env_iolist(iolist->quot[doll_ptr - iolist->str], prev))
		{
			expansion_key_iolist(iolist, envlist, doll_ptr);
			xfree(iolist->quot);
			iolist->quot = get_quot_flag(iolist->str);
			doll_ptr = ft_strchr(iolist->str, '$');
		}
		if (ft_strchr(iolist->quot, '1') || ft_strchr(iolist->quot, '2'))
			clear_quot_iolist(iolist);
		serch_new_space_iolist(iolist);
		prev = iolist;
		iolist = iolist->next;
	}
}
