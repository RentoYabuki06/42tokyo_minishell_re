/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:15:17 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:40:21 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_eof(t_token *token)
{
	return (token->type == TOKEN_EOF);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

bool	is_metacharacter(char c)
{
	if (is_space(c))
		return (true);
	return (c && ft_strchr("|<>\n", c));
}

bool	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

bool	is_prefix(const char *s, const char *keyword)
{
	return (ft_memcmp(s, keyword, ft_strlen(keyword)) == 0);
}
