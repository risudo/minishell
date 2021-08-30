#include "minishell.h"

//unset
void	builtin_unset(t_execdata *data)
{
	size_t	i;

	i = 1;
	while (data->cmdline[i])
	{
		ft_unsetenv(data->elst, data->cmdline[i]);
		i++;
	}
	*(data->status) = 0;
}

//env
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
	*(data->status) = 0;
}

//exit
void	builtin_exit(t_execdata *data)
{
	bool	nonnum_check;

	if (data->cmdline[1] == NULL)
		exit(0);
	ft_putendl_fd("exit", STDERR_FILENO);
	if (data->cmdline[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		*(data->status) = 1;
		return ;
	}
	*(data->status) = (unsigned char)ft_atol(data->cmdline[1], &nonnum_check);
	if (nonnum_check)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(data->cmdline[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		*(data->status) = 255;
	}
	exit(*(data->status));
}
