/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:55 by yabukirento       #+#    #+#             */
/*   Updated: 2024/04/28 10:34:38 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	pc;

	i = 0;
	pc = c;
	while (s[i])
		i++;
	while (i > 0)
	{
		if (s[i] == pc)
		{
			return ((char *)(s + i));
		}
		i--;
	}
	if (s[i] == pc)
		return ((char *)(s + i));
	return ((void *)0);
}
