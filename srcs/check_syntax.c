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
