#include "minishell.h"

/*
** iolist and cmdlist have flag corresponding to each character of string.
** The flag meaning:
** D: begginning of or inside the double quot
** S: begginning of or inside the single quot
** 2: end of double quot
** 1: end of single quot
** 0: out of quot
*/

char	get_flag(char c, char flag)
{
	if ((c == '\"' && (flag == '0' || flag == '1' || flag == '2'))
		|| (flag == 'D' && c != '\"'))
		return ('D');
	else if ((c == '\'' && (flag == '0' || flag == '1' || flag == '2'))
		|| (flag == 'S' && c != '\''))
		return ('S');
	else if (c == '\"' && flag == 'D')
		return ('2');
	else if (c == '\'' && flag == 'S')
		return ('1');
	else
		return ('0');
}

char	*get_quot_flag(char *str)
{
	char	*ret;
	char	flag;
	size_t	i;

	ret = (char *)ft_xcalloc(ft_strlen(str) + 1, sizeof(char));
	i = 0;
	flag = '0';
	while (*str)
	{
		flag = get_flag(*str, flag);
		ret[i++] = flag;
		str++;
	}
	return (ret);
}

t_cmdlist	*new_clst(t_cmdlist *cur, t_token *token)
{
	t_cmdlist	*new;

	new = (t_cmdlist *)ft_xcalloc(1, sizeof(*new));
	cur->next = new;
	new->str = ft_xstrdup(token->str);
	new->quot = get_quot_flag(new->str);
	return (new);
}

bool	is_cmd(t_token *start)
{
	if (start->special == ELSE
		&& (start->prev == NULL
			|| start->prev->special == ELSE
			|| start->prev->special == PIPE))
		return (true);
	else
		return (false);
}

t_iolist	*new_iolst(t_iolist *cur, t_token *token)
{
	t_iolist	*new;

	new = (t_iolist *)ft_xcalloc(1, sizeof(*new));
	new->open_fd = -1;
	new->c_type = token->special;
	cur->next = new;
	new->str = ft_xstrdup(token->str);
	new->quot = get_quot_flag(new->str);
	return (new);
}
