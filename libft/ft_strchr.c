/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:55 by yabukirento       #+#    #+#             */
/*   Updated: 2024/04/21 19:00:38 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	pc;

	i = 0;
	pc = c;
	while (s[i])
	{
		if (s[i] == pc)
		{
			return ((char *)(s + i));
		}
		i++;
	}
	if (s[i] == pc)
		return ((char *)(s + i));
	return ((void *)0);
}
