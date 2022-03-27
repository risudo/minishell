/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:13:11 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:54:01 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*xft_strjoin(char *str1, char *str2)
{
	char	*joined_str;

	joined_str = ft_strjoin(str1, str2);
	if (!joined_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (joined_str);
}

char	**xft_split(char *src_str, char cut_char)
{
	char	**splited_str;

	splited_str = ft_split(src_str, cut_char);
	if (!splited_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (splited_str);
}

void	xclose(int fd)
{
	if (0 <= fd && close(fd) == -1)
	{
		if (errno != EBADF)
		{
			ft_perror("close");
			exit(EXIT_FAILURE);
		}
	}
}

pid_t	xfork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	xwaitpid(pid_t pid, int *wstatus, int options)
{
	if (waitpid(pid, wstatus, options) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
}
