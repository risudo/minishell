#include "minishell.h"

void	ft_perror(char *perror_str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(perror_str);
}
