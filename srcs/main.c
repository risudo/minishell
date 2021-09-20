/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsudo <rsudo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:12:30 by rsudo             #+#    #+#             */
/*   Updated: 2021/09/20 22:12:31 by rsudo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	g_status = 0;

void	minishell_loop(char **envp)
{
	t_envlist		*elst;
	t_execdata		*data;
	char			*line;

	elst = create_envlist(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			line = ft_xstrdup("exit");
		if (line[0] != '\0')
		{
			data = parse_cmd(line, elst);
			execute_start(data);
			elst = data->elst;
			clear_execdata(data);
			add_history(line);
		}
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	xsignal(SIGINT, signal_handler);
	xsignal(SIGQUIT, SIG_IGN);
	minishell_loop(envp);
	(void)argc;
	(void)argv;
	return (g_status);
}
