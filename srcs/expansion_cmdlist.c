#include "minishell.h"

/*
** expand_cmdlist() do the following.
** 1 Expand environment valiables.
** 2 Clear quotation.
** 3 If there are spaces after expanding, insert new list.
*/

static size_t	expand_key_cmdlist(t_cmdlist *clist,
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
	free(clist->str);
	clist->str = ft_strjoin_three(front_key, value, back_key);
	len = ft_strlen(front_key) + ft_strlen(value);
	free(front_key), free(key), free(back_key);
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
	free(clist->str);
	clist->str = new_str;
	clist->quot = get_removed_endflag(&clist->quot, '1');
	clist->quot = get_removed_endflag(&clist->quot, '2');
}

static t_cmdlist	*insert_new_cmdlist(t_cmdlist *clist, size_t *i, size_t idx)
{
	t_cmdlist	*new;
	size_t		len;

	while (ft_isspace(clist->str[*i]) && *i < idx)
		(*i)++;
	len = ft_strlen(clist->str + *i);
	if (len == 0)
		return (clist);
	new = (t_cmdlist *)ft_xcalloc(1, sizeof(*new));
	new->str = ft_xsubstr(clist->str, *i, len);
	new->quot = get_quot_flag(new->str);
	new->next = clist->next;
	clist->next = new;
	return (clist);
}

/*
** @(idx) index of end of environment variable after expanding.
*/

static size_t	search_new_space_cmdlist(t_cmdlist *clist, size_t idx)
{
	size_t	i;
	char	*str;

	i = get_space_idx(clist);
	if (i == 0 && ft_isspace(clist->str[i]) && idx != SIZE_MAX)
	{
		while (ft_isspace(clist->str[i]))
			i++;
		str = ft_xsubstr(clist->str, i, ft_strlen(clist->str + i));
		free(clist->str), free(clist->quot), clist->str = str;
		clist->quot = get_quot_flag(clist->str);
		idx = search_new_space_cmdlist(clist, idx - i);
	}
	else if (ft_isspace(clist->str[i]) && idx != SIZE_MAX)
	{
		str = ft_xsubstr(clist->str, 0, i);
		clist = insert_new_cmdlist(clist, &i, idx);
		free(clist->str), free(clist->quot), clist->str = str;
		clist->quot = get_quot_flag(clist->str);
		if (idx > i)
			idx -= i;
		else
			idx = SIZE_MAX;
	}
	return (idx);
}

void	expand_cmdlist(t_cmdlist **clist, t_envlist *envlist)
{
	char		*doll_ptr;
	size_t		len;
	t_cmdlist	*prev;
	t_cmdlist	*head;

	head = (*clist);
	while ((*clist))
	{
		doll_ptr = ft_strdoll((*clist)->str);
		while (doll_ptr && (*clist)->quot[doll_ptr - (*clist)->str] != 'S')
		{
			len = expand_key_cmdlist((*clist), envlist, doll_ptr);
			free((*clist)->quot);
			(*clist)->quot = get_quot_flag((*clist)->str);
			doll_ptr = ft_strdoll((*clist)->str + len);
		}
		if (!(*clist)->str[0] && delone_cmdlist(clist, prev, &head))
			continue ;
		if (ft_strchr((*clist)->quot, '1') || ft_strchr((*clist)->quot, '2'))
			clear_quot_cmdlist((*clist));
		len = search_new_space_cmdlist((*clist), len);
		prev = (*clist);
		(*clist) = (*clist)->next;
	}
	(*clist) = head;
}
