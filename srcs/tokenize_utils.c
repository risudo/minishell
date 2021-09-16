#include "minishell.h"

void	set_special_c(t_token *list)
{
	while (list)
	{
		if (list->str[ft_strlen(list->str) - 1] == '>'
			&& list->str[ft_strlen(list->str) - 2] == '>')
			list->special = APPEND_REDIRECT;
		else if (list->str[ft_strlen(list->str) - 1] == '<'
			&& list->str[ft_strlen(list->str) - 2] == '<')
			list->special = HERE_DOC;
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

void	put_syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ",
		 STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}
