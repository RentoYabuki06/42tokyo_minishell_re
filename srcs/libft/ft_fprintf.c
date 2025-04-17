/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:59:35 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/17 10:51:58 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_fprintf1(int fd, const char *fmt, const char *arg)
{
	char		buf[BUFFER_SIZE];
	int			i;
	int			j;

	i = 0;
	if (arg == NULL)
		return ;
	while (*fmt && i < BUFFER_SIZE - 1)
	{
		if (*fmt == '%' && *(fmt + 1) == 's')
		{
			j = 0;
			while (arg[j] && i < BUFFER_SIZE - 1)
				buf[i++] = arg[j++];
			fmt += 2;
		}
		else
			buf[i++] = *fmt++;
	}
	buf[i] = '\0';
	write(fd, buf, i);
}

void	ft_fprintf2(int fd, const char *fmt, const char *s1, const char *s2)
{
	char		buf[BUFFER_SIZE];
	int			i;
	int			j;
	const char	*s;

	i = 0;
	s = s1;
	if (s1 == NULL || s2 == NULL)
		return ;
	while (*fmt && i < BUFFER_SIZE - 1)
	{
		if (*fmt == '%' && *(fmt + 1) == 's')
		{
			j = 0;
			while (s[j] && i < BUFFER_SIZE - 1)
				buf[i++] = s[j++];
			s = s2;
			fmt += 2;
		}
		else
			buf[i++] = *fmt++;
	}
	buf[i] = '\0';
	write(fd, buf, i);
}
