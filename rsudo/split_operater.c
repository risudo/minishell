#include "lexer.h"

static t_token	*insert_new_token(t_token *list, char *ope)
{
	t_token	*new;
	t_token	*next;

	new = ft_calloc(1, sizeof(*list));
	if (new == NULL)
		return (NULL);
	new->str = ope;
	next = list->next;
	list->next = new;
	new->prev = list;
	new->next = next;
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
	int		excluded_i;
	int		newstr_i;

	newstr_i = -1;
	excluded_i = 0;
	newstr_len = get_newstr_len(list);
	newstr = (char *)malloc(sizeof(char) * (newstr_len + 1));
	excluded = (char *)malloc(sizeof(char)
			* (ft_strlen(list->str) - newstr_len + 1));
	if (newstr == NULL || excluded == NULL)
		return (NULL);
	while (++newstr_i < newstr_len)
		newstr[newstr_i] = list->str[newstr_i];
	newstr[newstr_i] = '\0';
	while (list->str[newstr_i])
		excluded[excluded_i++] = list->str[newstr_i++];
	excluded[excluded_i] = '\0';
	free(list->str);
	list->str = newstr;
	list = insert_new_token(list, excluded);
	return (list);
}

// split operater like pipe or ridirect

t_token	*split_operater(t_token *list)
{
	t_token	*head;

	head = list;
	while (list)
	{
		if (ft_strlen(list->str) >= 2 && (ft_strchr(list->str, '>')
				|| ft_strchr(list->str, '<') || ft_strchr(list->str, '|')))
		{
			list = get_newstr_list(list);
			if (list == NULL)
				clear_tokenlist(head);
			continue ;
		}
		list = list->next;
	}
	return (head);
}
