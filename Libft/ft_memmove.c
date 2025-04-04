/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:55 by yabukirento       #+#    #+#             */
/*   Updated: 2024/04/30 20:24:43 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

static void	ft_cpy(unsigned char *pdst, const unsigned char *psrc, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		*(pdst++) = *(psrc++);
		i++;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*pdst;
	unsigned char	*psrc;
	size_t			i;

	i = 0;
	if (!dst && !src)
		return (dst);
	pdst = (unsigned char *)dst;
	psrc = (unsigned char *)src;
	if (dst > src && dst < src + len)
	{
		pdst += len - 1;
		psrc += len - 1;
		while (i < len && *pdst && *psrc)
		{
			*(pdst--) = *(psrc--);
			i++;
		}
	}
	else
		ft_cpy(pdst, psrc, len);
	return (dst);
}
