#include "minishell.h"

/*
** Split tokenlist that have special character like pipe.
** Example: ls|cat -> [0] ls [1] | [2]cat
*/

static t_token	*insert_new_token(t_token *list, char *str)
{
	t_token	*new;
	t_token	*next;

	new = (t_token *)ft_xcalloc(1, sizeof(*new));
	new->str = str;
	new->prev = list;
	next = list->next;
	list->next = new;
	new->next = next;
	if (next)
		next->prev = new;
	return (new);
}

static t_token	*get_newstr_list(t_token *list, char *delimiter_ptr)
{
	char	*str;
	char	*excluded;
	size_t	len;

	if (delimiter_ptr == list->str)
		len = 1;
	else
		len = delimiter_ptr - list->str;
	str = ft_xsubstr(list->str, 0, len);
	excluded = ft_xsubstr(list->str, len, ft_strlen(list->str) - len);
	free(list->str);
	list->str = str;
	list = insert_new_token(list, excluded);
	return (list);
}

static bool	is_delimiter_to_split(char *ptr, char *head, bool nodigit)
{
	if (*ptr == '|'
		|| (ptr != head
			&& ((*ptr != '<' && *ptr != '>'
					&& (*(ptr - 1) == '<' || *(ptr - 1) == '>'))
				|| (nodigit && ((*ptr == '<' && *(ptr - 1) != '<')
						|| (*ptr == '>' && *(ptr - 1) != '>'))))))
	{
		return (true);
	}
	return (false);
}

/*
** @param(str): token list's str
** Return pointer to the character to be delimited.
*/
char	*get_delimiter_ptr(char *str)
{
	t_quottype	flag;
	bool		nodigit;
	char		*head;

	head = str;
	nodigit = false;
	flag = DEFAULT;
	while (*str)
	{
		if (head != str && !ft_isdigit(*(str - 1)))
			nodigit = true;
		if (is_delimiter_to_split(str, head, nodigit)
			&& flag != D_QUOT && flag != S_QUOT
			&& ((!nodigit && *(str + 1) != '\0') || nodigit))
		{
			if (nodigit == false && head != str)
				str++;
			return (str);
		}
		flag = get_flag_quot(str, flag);
		str++;
	}
	return (NULL);
}

int	split_operater(t_token *list)
{
	char	*delimiter_ptr;

	while (list)
	{
		delimiter_ptr = get_delimiter_ptr(list->str);
		if (ft_strlen(list->str) >= 2 && delimiter_ptr)
		{
			list = get_newstr_list(list, delimiter_ptr);
			continue ;
		}
		list = list->next;
	}
	return (0);
}
