/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:55 by yabukirento       #+#    #+#             */
/*   Updated: 2024/04/23 17:23:56 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	len_needle;
	size_t	len_haystack;
	char	*hay;

	i = 0;
	if (len == 0 && *needle == '\0')
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	len_needle = ft_strlen(needle);
	len_haystack = ft_strlen(haystack);
	if (*needle == '\0')
		return ((char *)haystack);
	hay = (char *)haystack;
	while (i < len && len - i >= len_needle && i < len_haystack)
	{
		if (ft_strncmp((hay + i), needle, len_needle) == 0)
		{
			return ((char *)(hay + i));
		}
		i++;
	}
	return ((void *)0);
}
