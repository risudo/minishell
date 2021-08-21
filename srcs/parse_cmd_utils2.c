#include "../includes/parse.h"

static t_token	*insert_new_token(t_token *list, char *ope)
{
	t_token	*new;
	t_token	*next;

	new = ft_xcalloc(1, sizeof(*new));
	new->str = ope;
	new->prev = list;
	next = list->next;
	list->next = new;
	new->next = next;
	next->prev = new;
	return (new);
}

static int	get_newstr_len(t_token *list)
{
	int	i;

	i = 0;
	while (list->str[i] != '>' && list->str[i] != '<' && list->str[i] != '|')
		i++;
	if (i == 0)
		i = 1;
	return (i);
}

// allocate new str removed operater.
// insert new token that has remaining str

static t_token	*get_newstr_list(t_token *list)
{
	char	*newstr;
	char	*excluded;
	int		newstr_len;

	newstr_len = get_newstr_len(list);
	newstr = ft_xsubstr(list->str, 0, newstr_len);
	excluded = ft_xsubstr(list->str, newstr_len,
			ft_strlen(list->str) - newstr_len);
	xfree(list->str);
	list->str = newstr;
	list = insert_new_token(list, excluded);
	return (list);
}

// Find pipe or ridirect that is not splited and insert new token
// that have splited operater.

int	split_operater(t_token *list)
{
	t_token	*head;

	head = list;
	while (list)
	{
		if (ft_strlen(list->str) >= 2 && (ft_strchr(list->str, '>')
				|| ft_strchr(list->str, '<') || ft_strchr(list->str, '|')))
		{
			list = get_newstr_list(list);
			continue ;
		}
		list = list->next;
	}
	return (0);
}
