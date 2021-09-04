#include "minishell.h"

/*
** Serch iolist and do the following.
** 1 Expand environment valiables.
** 2 Clear quotation.
** 3 If there are spaces after expanding, insert new list.
*/

char	*ft_strdoll(const char *s)
{
	char *doll_ptr;

	doll_ptr = ft_strchr(s, '$');
	if (doll_ptr && is_delimiter(*(doll_ptr + 1)))
		return (ft_strdoll(doll_ptr + 1));
	else
		return (doll_ptr);
}

void	expansion_key_io(char **line, \
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
	front_key = ft_xsubstr(*line, 0, doll_ptr - *line);
	key = ft_xsubstr(doll_ptr, 1, len - 1);
	back_key = ft_xsubstr(doll_ptr + len, 0, ft_strlen(doll_ptr + len));
	value = ft_getenv(envlist, key);
	xfree(*line);
	*line = ft_strjoin_three(front_key, value, back_key);
	xfree(front_key), xfree(key), xfree(back_key);
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

static size_t	pass_space_idx(char *filename, char *filequot)
{
	size_t	i;

	i = 0;
	while (filename[i] && \
		ft_isspace(filename[i]) && filequot[i] == '0')
		i++;
	return (i);
}

static size_t	get_filename_len(char *filename, char *filequot)
{
	size_t	i;

	i = 0;
	while (filename[i] && \
		(!ft_isspace(filename[i]) || filequot[i] != '0'))
		i++;
	return (i);
}

static char *rm_isspace(char *filename, char *filequot)
{
	size_t	start;
	size_t	len;
	size_t	back;
	char	*clear_name;

	start = 0;
	len = 0;
	back = 0;
	start = pass_space_idx(filename, filequot);
	len = get_filename_len(filename + start, filequot + start);
	back = pass_space_idx(filename + start + len, filequot + start + len);
	if (filename[start + len + back])
		clear_name = NULL;
	else
		clear_name = ft_xsubstr(filename, start, len);
	free(filename);
	return (clear_name);
}

static int	check_filename(t_iolist *iolist, \
				char *filename, char *filequot)
{
	filename = rm_isspace(filename, filequot);
	if (!filename)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(iolist->str, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect", STDERR_FILENO);
		free(filequot);
		return (-1);
	}
	free(iolist->str);
	free(iolist->quot);
	iolist->str = filename;
	iolist->str = filequot;
	return (0);
}

int	serch_env_iolist(t_iolist *iolist, t_envlist *envlist)
{
	char		*doll_ptr;
	char		*filename;
	char		*filequot;

	filename = ft_xstrdup(iolist->str);
	filequot = get_quot_flag(filename);
	doll_ptr = ft_strdoll(filename);
	while (doll_ptr && filequot[doll_ptr - filename] != 'S')
	{
		expansion_key_io(&filename, envlist, doll_ptr);
		xfree(filequot);
		filequot = get_quot_flag(filename);
		doll_ptr = ft_strdoll(filename);
	}
	if (ft_strchr(filequot, '1') || ft_strchr(filequot, '2'))
		clear_quot_filename(&filename, &filequot);
	return (check_filename(iolist, filename, filequot));
}
