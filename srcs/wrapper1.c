/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsudo <rsudo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:13:07 by rsudo             #+#    #+#             */
/*   Updated: 2021/09/20 22:13:08 by rsudo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.h"

void	*ft_xcalloc(size_t count, size_t size)
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

char	*ft_xsubstr(char const *s, unsigned int start, size_t len)
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

char	*ft_xstrdup(const char *src)
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

char	*ft_xitoa(int n)
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
