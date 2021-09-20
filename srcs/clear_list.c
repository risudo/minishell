/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsudo <rsudo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:10:46 by rsudo             #+#    #+#             */
/*   Updated: 2021/09/20 22:10:47 by rsudo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Clear lists that is defined in minishell.h
*/

void	clear_envlist(t_envlist *envlist)
{
	t_envlist	*next;

	while (envlist)
	{
		next = envlist->next;
		free(envlist->key);
		free(envlist->value);
		free(envlist);
		envlist = next;
	}
}

void	clear_tokenlist(t_token *list)
{
	t_token	*next;

	while (list)
	{
		next = list->next;
		free(list->str);
		free(list);
		list = next;
	}
}

static void	clear_cmdlist(t_cmdlist *list)
{
	t_cmdlist	*next;

	while (list)
	{
		next = list->next;
		free(list->str);
		free(list->quot);
		free(list);
		list = next;
	}
}

static void	clear_iolist(t_iolist *list)
{
	t_iolist	*next;

	while (list)
	{
		next = list->next;
		xclose(list->open_fd);
		free(list->str);
		free(list->quot);
		free(list);
		list = next;
	}
}

void	clear_execdata(t_execdata *data)
{
	t_execdata	*next;

	while (data)
	{
		next = data->next;
		clear_cmdlist(data->clst);
		clear_iolist(data->iolst);
		free_2d_array(data->cmdline);
		free(data);
		data = next;
	}
}
