/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xclose.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:13:11 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 17:07:07 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
