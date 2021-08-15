#include "lexer.h"

t_token	*join_redirect(t_token *list)
{
	t_token	*next;

	next = list->next->next;
	if (list->next->str[0] == '>')
	{
		free(list->str);
		list->str = ft_strdup(">>");
	}
	if (list->next->str[0] == '<')
	{
		free(list->str);
		list->str = ft_strdup("<<");
	}
	if (list->str == NULL)
		return (NULL);
	free(list->next->str);
	free(list->next);
	list->next = next;
	next->prev = list;
	return (list);
}

bool	is_consecutive_redirect(t_token *list)
{
	if ((list->str[0] == '>' && list->next->str[0] == '>')
		|| (list->str[0] == '<' && list->next->str[0] == '<'))
		return (true);
	else
		return (false);
}

t_token	*trim_list(t_token *list)
{
	t_token	*next;
	t_token	*prev;

	next = list->next;
	prev = list->prev;
	free(list->str);
	free(list);
	prev->next = next;
	next->prev = prev;
	return (next);
}

bool	is_space_after_quot(t_token *list)
{
	if (!list->prev || !list->next)
		return (false);
	if (ft_strchr(list->str, ' ')
		&& (ft_strchr(list->prev->str, '\'')
			|| ft_strchr(list->prev->str, '\"'))
		&& !(ft_strchr(list->next->str, '\'')
			|| ft_strchr(list->next->str, '\"')))
		return (true);
	else
		return (false);
}

t_token	*parse_tokenlist(t_token *list)
{
	t_token	*head;

	head = list;
	while (list)
	{
		if (is_space_after_quot(list))
			list = trim_list(list);
		if (is_consecutive_redirect(list))
			list = join_redirect(list);
		if (list == NULL)
			clear_tokenlist(head);
		list = list->next;
	}
	return (head);
}

void	parse_cmd(char *command)
{
	t_token	*list;

	list = tokenize_cmd_by_space(command);
	// put_list(list);
	list = split_operater(list);
	list = parse_tokenlist(list);
	put_list(list);
	clear_tokenlist(list);
}
