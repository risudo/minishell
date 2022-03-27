/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:11:11 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:05:10 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_execdata *data)
{
	t_envlist	*move;

	move = data->elst;
	while (move)
	{
		if (move->value)
			printf("%s=%s\n", move->key, move->value);
		move = move->next;
	}
	g_status = 0;
}
