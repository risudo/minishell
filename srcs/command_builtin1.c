/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builtin1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsudo <rsudo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:10:49 by rsudo             #+#    #+#             */
/*   Updated: 2021/09/20 22:10:50 by rsudo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Execute built-in commands.
** - echo
** - cd
** - pwd
*/

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

void	builtin_pwd(t_execdata *data)
{
	char	*pathname;

	if (write(STDOUT_FILENO, NULL, 0) == -1)
	{
		ft_perror("pwd: write error");
		g_status = 1;
		return ;
	}
	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		ft_perror("pwd");
		g_status = 1;
	}
	else
	{
		printf("%s\n", pathname);
		g_status = 0;
	}
	(void)data;
	free(pathname);
}
