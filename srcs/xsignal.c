/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper1 copy 4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:13:07 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 17:00:14 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	xsignal(int sig, void f(int))
{
	if (signal(sig, f) == SIG_ERR)
	{
		ft_perror("signal");
		exit(EXIT_FAILURE);
	}
}
