/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:11:11 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:05:12 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(t_execdata *data)
{
	bool	nonnum_check;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (data->cmdline[1])
	{
		g_status = (unsigned char)ft_atol(data->cmdline[1], &nonnum_check);
		if (nonnum_check)
		{
			ft_puterror("exit", data->cmdline[1], \
							"numeric argument required");
			g_status = 2;
		}
		else if (data->cmdline[2])
		{
			ft_puterror("exit", "too many arguments", NULL);
			g_status = 1;
			return ;
		}
	}
	exit(g_status);
}
