/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:10:49 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 15:46:37 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
