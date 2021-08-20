#include "lexer.h"

static t_token	*join_redirect(t_token *list)
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

static bool	is_consecutive_redirect(t_token *list)
{
	if ((list->str[0] == '>' && list->next->str[0] == '>')
		|| (list->str[0] == '<' && list->next->str[0] == '<'))
		return (true);
	else
		return (false);
}

static t_token	*trim_list(t_token *list)
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

static bool	is_space_after_quot(t_token *list)
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

static void	set_special_c(t_token *list)
{
	while (list)
	{
		if (!ft_strncmp(list->str, ">", 2))
			list->special = OUT_REDIRECT;
		else if (!ft_strncmp(list->str, "<", 2))
			list->special = IN_REDIRECT;
		else if (!ft_strncmp(list->str, ">>", 3))
			list->special = OUT_HERE_DOC;
		else if (!ft_strncmp(list->str, "<<", 3))
			list->special = IN_HERE_DOC;
		else if (!ft_strncmp(list->str, "|", 2))
			list->special = PIPE;
		else
			list->special = ELSE;
		list = list->next;
	}
}

int	parse_tokenlist(t_token *list)
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
		{
			clear_tokenlist(head);
			return (-1);
		}
		list = list->next;
	}
	set_special_c(head);
	return (0);
}
