/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsudo <rsudo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:12:05 by rsudo             #+#    #+#             */
/*   Updated: 2021/09/20 22:12:06 by rsudo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_execdata *data)
{
	void (*cmd_func[CMD_NUM])(t_execdata *data);

	if (data->cmd_type == NON_CMD)
		return ;
	cmd_func[ECHO] = builtin_echo;
	cmd_func[CD] = builtin_cd;
	cmd_func[PWD] = builtin_pwd;
	cmd_func[EXPORT] = builtin_export;
	cmd_func[UNSET] = builtin_unset;
	cmd_func[ENV] = builtin_env;
	cmd_func[EXIT] = builtin_exit;
	cmd_func[OTHER] = non_builtin;
	cmd_func[NON_CMD] = NULL;
	cmd_func[data->cmd_type](data);
}

void	free_2d_array(char **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			free(array[i]);
		free(array);
	}
}

void	set_status_from_child_status(int wstatus)
{
	if (WIFEXITED(wstatus))
		g_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		g_status = WTERMSIG(wstatus) + 128;
	}
}
