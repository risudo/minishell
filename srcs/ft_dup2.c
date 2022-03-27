/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:13:15 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 17:20:57 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dup2(int oldfd, int newfd)
{
	int	fd;

	fd = 0;
	if (oldfd < 0)
		return (-1);
	if (oldfd != newfd)
	{
		fd = dup2(oldfd, newfd);
		if (fd == -1)
		{
			if (errno == EBADF)
				ft_perror("file descriptor out of range");
			else
				ft_perror("dup2");
		}
		xclose(oldfd);
	}
	return (fd);
}
