/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:10:49 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:01:23 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_cd(t_execdata *data)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (data->cmdline[1] && \
		data->cmdline[1][0] && \
		chdir(data->cmdline[1]) == -1)
	{
		ft_perror("cd");
		g_status = 1;
		free(old_pwd);
		return ;
	}
	data->elst = ft_setenv(data->elst, ft_xstrdup("OLDPWD"), old_pwd, 0);
	if (ft_getenv(data->elst, "PWD"))
		data->elst = \
			ft_setenv(data->elst, ft_xstrdup("PWD"), getcwd(NULL, 0), 0);
	g_status = 0;
}
