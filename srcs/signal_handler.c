/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsudo <rsudo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:12:52 by rsudo             #+#    #+#             */
/*   Updated: 2021/09/20 22:12:53 by rsudo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_handler(int signo)
{
	(void)signo;
	ft_putstr_fd("\r\033[0K", STDOUT_FILENO);
	exit(130);
}

void	signal_handler(int signo)
{
	g_status = 128 + signo;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
