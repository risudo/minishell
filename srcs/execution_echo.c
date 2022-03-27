/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:10:49 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:05:04 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(t_execdata *data)
{
	int		option;
	size_t	arg_i;

	if (write(STDOUT_FILENO, NULL, 0) == -1)
	{
		ft_perror("echo: write error");
		g_status = 1;
		return ;
	}
	option = 0;
	arg_i = 1;
	if (data->cmdline[arg_i] && \
		ft_strcmp(data->cmdline[arg_i], "-n") == 0 && \
		arg_i++)
		option++;
	while (data->cmdline[arg_i])
	{
		ft_putstr_fd(data->cmdline[arg_i], STDOUT_FILENO);
		if (data->cmdline[arg_i + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		arg_i++;
	}
	if (option == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	g_status = 0;
}
