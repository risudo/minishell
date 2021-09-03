#include "minishell.h"

/*
** Serch iolist and do the following.
** 1 Expand environment valiables.
** 2 Clear quotation.
** 3 If there are spaces after expanding, insert new list.
*/

void	expansion_key_io(char **line,
		t_envlist *envlist, char *doll_ptr, int recursive)
{
	char	*key;
	char	*value;
	char	*front_key;
	char	*back_key;
	size_t	len;

	if (!(doll_ptr && !is_delimiter(*(doll_ptr + 1))))
		return ;
	len = 1;
	while (!is_delimiter(doll_ptr[len]))
		len++;
	front_key = ft_xsubstr(*line, 0, doll_ptr - *line);
	key = ft_xsubstr(doll_ptr, 1, len - 1);
	back_key = ft_xsubstr(doll_ptr + len, 0, ft_strlen(doll_ptr + len));
	value = ft_getenv(envlist, key);
	xfree(*line);
	*line = ft_strjoin_three(front_key, value, back_key);
	xfree(front_key), xfree(key), xfree(back_key);
	if (recursive)
	{
		doll_ptr = ft_strchr(*line, '$');
		if (doll_ptr && !is_delimiter(*(doll_ptr + 1)))
			expansion_key_io(line, \
				envlist, doll_ptr, recursive);
	}
}

void	clear_quot_filename(char **filename, char **filequot)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new_str = ft_xcalloc(\
			ft_strlen_excluded_quot(*filename, *filequot) + 1, \
			sizeof(char));
	while ((*filename)[i])
	{
		if (is_delimiter_quot((*filename)[i], (*filequot)[i]))
		{
			i++;
			continue ;
		}
		new_str[j++] = (*filename)[i++];
	}
	xfree(*filename);
	*filename = new_str;
	*filequot = get_removed_endflag(filequot, '1');
	*filequot = get_removed_endflag(filequot, '2');
}
/*
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
*/

static size_t	space_idx(char *filename, char *filequot)
{
	size_t	i;

	i = 0;
	while (filename[i]
		&& (!ft_isspace(filename[i]) || filequot[i] != '0'))
		i++;
	return (i);
}

static int	check_filename(t_iolist *iolist, \
				char *filename, char *filequot)
{
	size_t	i;
	size_t	len;
	char	*str;

	if (filename[0] == '\0')
	{
		free(filename), free(filequot);
		return (-1);
	}
	i = space_idx(filename, filequot);
	if (i == 0 && ft_isspace(filename[i]))
	{
		while (ft_isspace(filename[i]))
			i++;
		len = ft_strlen(filename + i);
		free(filename), free(filequot);
		filename = ft_xsubstr(filename, i, len);//
		filequot = get_quot_flag(filename);
		check_filename(iolist, filename, filequot);
	}
	else if (ft_isspace(filename[i]))
	{
		str = ft_xsubstr(filename, 0, i);
		free(filename), free(filequot);
		filename = str;
		filequot = get_quot_flag(filename);
		i = 0;
		while (ft_isspace(filename[i]))
			i++;
	}
	if (filename[i] != '\0')
	{
		free(filename), free(filequot);
		return (-1);
	}
	free(iolist->str), free(filequot);
	iolist->str = filename;
	return (0);
}

int	serch_env_iolist(t_iolist *iolist, t_envlist *envlist)
{
	char		*doll_ptr;
	char		*filename;
	char		*filequot;

	doll_ptr = ft_strchr(filename, '$');
	if (!(doll_ptr && !is_delimiter(*(doll_ptr + 1))))
		return (0);
	filename = ft_xstrdup(iolist->str);
	filequot = get_quot_flag(filename);
	while (doll_ptr && filequot[doll_ptr - filename] != 'S'
		&& !is_delimiter(*(doll_ptr + 1)))
	{
		expansion_key_io(&filename, envlist, doll_ptr, 0);
		xfree(filequot);
		filequot = get_quot_flag(filename);
		doll_ptr = ft_strchr(filename, '$');
	}
	if (ft_strchr(filequot, '1') || ft_strchr(filequot, '2'))
		clear_quot_filename(&filename, &filequot);
	return (check_filename(iolist, filename, filequot));
}
