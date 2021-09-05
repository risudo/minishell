#include "minishell.h"

/*
** Execute built-in commands.
** - unset
** - env
** - exit
*/

void	builtin_unset(t_execdata *data)
{
	size_t	i;

	i = 1;
	while (data->cmdline[i])
	{
		ft_unsetenv(data->elst, data->cmdline[i]);
		i++;
	}
	g_status = 0;
}

void	builtin_env(t_execdata *data)
{
	t_envlist	*move;

	move = data->elst;
	while (move)
	{
		if (move->value)
			printf("%s=%s\n", move->key, move->value);
		move = move->next;
	}
	g_status = 0;
}

void	builtin_exit(t_execdata *data)
{
	bool	nonnum_check;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (data->cmdline[1] == NULL)
		exit(0);
	if (data->cmdline[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		g_status = 1;
		return ;
	}
	g_status = (unsigned char)ft_atol(data->cmdline[1], &nonnum_check);
	if (nonnum_check)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(data->cmdline[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		g_status = 255;
	}
	exit(g_status);
}
