/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakiyama <ryojiro25@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 22:12:44 by rakiyama          #+#    #+#             */
/*   Updated: 2021/09/20 22:12:45 by rakiyama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ans;
	unsigned char	ch;

	ans = (unsigned char *)b;
	ch = (unsigned char)c;
	while (len--)
		*(ans++) = ch;
	return (b);
}
