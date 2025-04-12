/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:55 by yabukirento       #+#    #+#             */
/*   Updated: 2024/04/21 19:36:17 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	src_len = ft_strlen(src);
	i = 0;
	if (dstsize == 0)
		return (src_len);
	dst_len = ft_strlen(dst);
	if (dst_len < dstsize)
	{
		while (src[i] != '\0' && dst_len < dstsize - 1)
		{
			dst[dst_len] = src[i];
			i++;
			dst_len++;
		}
		dst[dst_len] = '\0';
		return (dst_len + src_len - i);
	}
	else
		return (src_len + dstsize);
}
