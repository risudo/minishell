#include "minishell.h"

/*
** Split tokenlist that have special character like pipe.
** Example: ls|cat
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

/*
** @param(str): token list's str
** Return pointer to the character to be delimited.
*/
char	*is_delimiter_operater(char *str)
{
	t_quottype	flag;
	bool		nodigit;
	char		*head;

	head = str;
	nodigit = false;
	flag = DEFAULT;
	while (*str)
	{
		flag = get_flag_quot(str, flag);
		if (head != str && !ft_isdigit(*(str - 1)))
			nodigit = true;
		if ((*str == '>' || *str == '<' || *str == '|')
			&& flag != D_QUOT && flag != S_QUOT
			&& ((!nodigit && *(str + 1) != '\0') || nodigit))
		{
			if (nodigit == false && head != str)
				str++;
			return (str);
		}
		str++;
	}
	return (NULL);
}

int	split_operater(t_token *list)
{
	t_token	*head;
	char	*delimiter_ptr;

	head = list;
	while (list)
	{
		delimiter_ptr = is_delimiter_operater(list->str);
		if (ft_strlen(list->str) >= 2 && delimiter_ptr)
		{
			list = get_newstr_list(list, delimiter_ptr);
			continue ;
		}
		list = list->next;
	}
	return (0);
}
