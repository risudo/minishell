/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <rakiyama@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:13:07 by rsudo             #+#    #+#             */
/*   Updated: 2022/03/27 16:54:01 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.h"

void	*xft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(count, size);
	if (!ptr)
	{
		ft_perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

char	*xft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;

	ret = ft_substr(s, start, len);
	if (ret == NULL)
	{
		ft_perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

char	*xft_strdup(const char *src)
{
	char	*ret;

	ret = ft_strdup(src);
	if (ret == NULL)
	{
		ft_perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

char	*xft_itoa(int n)
{
	char	*ret;

	ret = ft_itoa(n);
	if (ret == NULL)
	{
		ft_perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ret);
}

void	xsignal(int sig, void f(int))
{
	if (signal(sig, f) == SIG_ERR)
	{
		ft_perror("signal");
		exit(EXIT_FAILURE);
	}
}
