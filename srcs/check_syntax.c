#include "../includes/parse.h"

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
		if (prev && prev->c_type <= OUT_HERE_DOC
			&& list->c_type <= OUT_HERE_DOC)
		{
			put_syntax_error(list->str);
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
			clear_execdata(head);
			return (NULL);
		}
		data = data->next;
	}
	return (head);
}
