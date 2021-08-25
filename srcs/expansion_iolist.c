#include "../includes/parse.h"

static char *ft_getenv(t_envlist *elst, char *search) //あきやまさんのを使う
{
	while (elst)
	{
		if (ft_strncmp(elst->key, search, ft_strlen(search) + 1) == 0)
			return (elst->value);
		elst = elst->next;
	}
	return (NULL);
}

bool	is_delimiter(char c)
{
	if (c == ' ' || c == '$' || c == '\"' || c == '\0')
		return (true);
	else
		return (false);
}

char	*ft_strjoin_three(char *str1, char *str2, char *str3)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(str1, str2);
	ret = ft_strjoin(tmp, str3);
	xfree(tmp);
	return (ret);
}

// Expand env_variable and set iolist->str it.
// key : env_variable's key
// value : env_variable's value
// front : the iolist's string before env_variable
// back : the iolist's string after env_variable
// len : env_variable's length
void	expansion_key_iolist(t_iolist *iolist,
		t_envlist *envlist, char *doll_ptr)
{
	char	*key;
	char	*value;
	char	*front;
	char	*back;
	int		len;

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

// Set iolist->str new string that quot is removed.
void	clear_quot_iolist(t_iolist *iolist)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	iolist->quot = get_removed_endflag(&iolist->quot, '1');
	iolist->quot = get_removed_endflag(&iolist->quot, '2');
	new_str = ft_xcalloc(ft_strlen_excluded_quot(iolist->str) + 1,
			sizeof(char));
	while (iolist->str[i])
	{
		if (iolist->str[i] == '\'' || iolist->str[i] == '\"')
		{
			i++;
			continue ;
		}
		new_str[j++] = iolist->str[i++];
	}
	xfree(iolist->str);
	iolist->str = new_str;
}

void	insert_new_iolist(t_iolist *iolist, int i)
{
	t_iolist	*new;
	char		*str;
	int			len;

	new = ft_xcalloc(1, sizeof(*new));
	len = 0;
	while (iolist->str[i] == ' ')
		i++;
	while (iolist->str[i + len] != '\0')
		len++;
	str = ft_xsubstr(iolist->str, i, len);
	new->str = str;
	new->quot = get_quot_flag(new->str);
	new->next = iolist->next;
	iolist->next = new;
}

// If there are spaces, trim the stirng and insert new iolist.
void	serch_new_space_iolist(t_iolist *iolist)
{
	int		i;
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

// Serch env_variable and enpand it.
// Delete quot.
// If there are spaces after exnpanding, insert new iolist.
void	serch_env_iolist(t_iolist *iolist, t_envlist *envlist)
{
	char	*doll_ptr;

	while (iolist)
	{
		doll_ptr = ft_strchr(iolist->str, '$');
		while (doll_ptr != NULL && iolist->quot[doll_ptr - iolist->str] != 'S')
		{
			expansion_key_iolist(iolist, envlist, doll_ptr);
			xfree(iolist->quot);
			iolist->quot = get_quot_flag(iolist->str);
			doll_ptr = ft_strchr(iolist->str, '$');
		}
		if (ft_strchr(iolist->quot, '1') || ft_strchr(iolist->quot, '2'))
			clear_quot_iolist(iolist);
		serch_new_space_iolist(iolist);
		iolist = iolist->next;
	}
}
