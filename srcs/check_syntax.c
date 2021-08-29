#include "../includes/parse.h"

/*
** Check iolist if it has multiple redirection destinations.
** If it has erorr, set the status 1.
*/

void	put_syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ",
		 STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}

int	check_iolist(t_iolist *list)
{
	t_iolist	*prev;

	prev = NULL;
	while (list)
	{
		if (prev && prev->c_type == ELSE
			&& list->c_type == ELSE)
		{
			ft_putendl_fd("minishell: ambiguous redirect", STDERR_FILENO);
			return (-1);
		}
		prev = list;
		list = list->next;
	}
	return (0);
}

t_execdata	*check_syntax(t_execdata *data)
{
	t_execdata	*head;

	head = data;
	while (data)
	{
		if (check_iolist(data->iolst) == -1)
		{
			*(data->status) = 1;
			return (head);
		}
		data = data->next;
	}
	return (head);
}
