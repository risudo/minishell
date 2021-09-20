/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <ryojiro25@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:11:17 by rakiyama          #+#    #+#             */
/*   Updated: 2021/09/20 22:11:28 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ans;
	size_t	msize;

	msize = count * size;
	ans = malloc(msize);
	if (!ans)
		return (NULL);
	if (!msize)
		msize = 1;
	ft_bzero(ans, msize);
	return (ans);
}
