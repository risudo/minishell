#include "../includes/parse.h"

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

// allocate new str removed operater.
// insert new token that has remaining str

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
	xfree(list->str);
	list->str = str;
	list = insert_new_token(list, excluded);
	return (list);
}

// どこで切り取るかのポインタを返して、切り取る時にそのポインタを使うが良さそう
char	*is_delimiter_operater(char *str)
{
	static int	flag;
	size_t		i;

	i = 0;
	while (str[i])
	{
		if (flag == 0 && (str[i] == '\'' || str[i] == '\"'))
			flag = 1;
		else if (flag == 1 && (str[i] == '\'' || str[i] == '\"'))
			flag = 0;
		if (flag == 0 && (str[i] == '>' || str[i] == '<' || str[i] == '|'))
			return (str + i);
		i++;
	}
	return (NULL);
}
// Find pipe or ridirect that is not splited and insert new token
// that have splited operater.

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
