/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by               #+#    #+#             */
/*   Updated: 2025/04/16 14:29:47 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_message(const char *msg)
{
	ft_fprintf1(STDERR_FILENO, "minishell: %s\n", msg);
}

void	print_error(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL)
		return ;
	ft_fprintf2(STDERR_FILENO, "minishell: %s: %s\n", s1, s2);
}

void	system_error(const char *prefix)
{
	if (prefix == NULL)
		return ;
	ft_fprintf2(STDERR_FILENO, "minishell: %s: %s\n", prefix, strerror(errno));
}
