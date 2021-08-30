#include "minishell.h"

t_token	*join_redirect(t_token *list)
{
	t_token	*next;

	next = NULL;
	if (list->next)
		next = list->next->next;
	if (list->next->str[0] == '>')
	{
		xfree(list->str);
		list->str = ft_xstrdup(">>");
	}
	if (list->next->str[0] == '<')
	{
		xfree(list->str);
		list->str = ft_xstrdup("<<");
	}
	if (list->next)
	{
		xfree(list->next->str);
		xfree(list->next);
		list->next = next;
	}
	if (next)
		next->prev = list;
	return (list);
}

bool	is_consecutive_redirect(t_token *list)
{
	if (list->next
		&& ((list->str[0] == '>' && list->next->str[0] == '>')
			|| (list->str[0] == '<' && list->next->str[0] == '<')))
		return (true);
	else
		return (false);
}

void	set_special_c(t_token *list)
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
