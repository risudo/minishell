/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:12:00 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:54:01 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Convert type list to type char 2D array
*/

static size_t	envlist_size(t_envlist *head)
{
	t_envlist	*move;
	size_t		cnt;

	cnt = 0;
	move = head;
	while (move)
	{
		cnt++;
		move = move->next;
	}
	return (cnt);
}

static size_t	cmdlist_size(t_cmdlist *head)
{
	t_cmdlist	*move;
	size_t		cnt;

	cnt = 0;
	move = head;
	while (move)
	{
		cnt++;
		move = move->next;
	}
	return (cnt);
}

char	**convert_envlist_2dchar(t_envlist *elst)
{
	char		**array;
	t_envlist	*move;
	size_t		cnt;
	char		*tmp;

	cnt = envlist_size(elst);
	array = (char **)xft_calloc((cnt + 1), sizeof(char *));
	cnt = 0;
	move = elst;
	while (move)
	{
		array[cnt] = xft_strjoin(move->key, "=");
		tmp = array[cnt];
		array[cnt] = xft_strjoin(array[cnt], move->value);
		free(tmp);
		cnt++;
		move = move->next;
	}
	return (array);
}

char	**convert_cmdlist_2dchar(t_cmdlist *clst)
{
	char		**array;
	t_cmdlist	*move;
	size_t		cnt;

	cnt = cmdlist_size(clst);
	array = (char **)xft_calloc((cnt + 1), sizeof(char *));
	cnt = 0;
	move = clst;
	while (move)
	{
		array[cnt] = xft_strjoin(move->str, NULL);
		cnt++;
		move = move->next;
	}
	return (array);
}
