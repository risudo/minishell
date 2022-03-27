/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimit_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:11:32 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:54:01 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_iolist	*insert_redirect_iolist(t_iolist *cur,
			char *redirect, t_special_c type)
{
	t_iolist	*new;

	new = (t_iolist *)xft_calloc(1, sizeof(*new));
	new->str = redirect;
	new->quot = get_quot_flag(new->str);
	new->open_fd = -1;
	new->c_type = type;
	cur->next = new;
	return (new);
}

t_iolist	*delimit_fd(t_iolist *cur)
{
	size_t		i;
	char		*fd;
	char		*redirect;
	t_special_c	type;

	i = 0;
	while (ft_isdigit(cur->str[i]))
		i++;
	fd = xft_substr(cur->str, 0, i);
	redirect = xft_substr(cur->str, i, ft_strlen(cur->str + i));
	free(cur->quot), free(cur->str);
	cur->str = fd;
	cur->quot = get_quot_flag(cur->str);
	type = cur->c_type;
	cur->c_type = FD;
	return (insert_redirect_iolist(cur, redirect, type));
}
