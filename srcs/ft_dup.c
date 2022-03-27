/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:13:15 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 17:20:36 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dup(t_execdata *data, t_stdfd type, int oldfd)
{
	int	newfd;

	newfd = dup(oldfd);
	if (newfd == -1)
		ft_perror("dup");
	else
		data->stdfd[type] = newfd;
	return (newfd);
}
