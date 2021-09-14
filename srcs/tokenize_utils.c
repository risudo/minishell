#include "minishell.h"

void	set_special_c(t_token *list)
{
	while (list)
	{
		if (list->str[ft_strlen(list->str) - 1] == '>'
			&& list->str[ft_strlen(list->str) - 2] == '>')
			list->special = OUT_HERE_DOC;
		else if (list->str[ft_strlen(list->str) - 1] == '<'
			&& list->str[ft_strlen(list->str) - 2] == '<')
			list->special = IN_HERE_DOC;
		else if (list->str[ft_strlen(list->str) - 1] == '>')
			list->special = OUT_REDIRECT;
		else if (list->str[ft_strlen(list->str) - 1] == '<')
			list->special = IN_REDIRECT;
		else if (!ft_strncmp(list->str, "|", ft_strlen(list->str)))
			list->special = PIPE;
		else
			list->special = ELSE;
		list = list->next;
	}
}
