/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:42:56 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:12:40 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_error(char *point, char **rest, char *input)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected character ", STDERR_FILENO);
	ft_putstr_fd(point, STDERR_FILENO);
	ft_putstr_fd(" in ", STDERR_FILENO);
	ft_putstr_fd(input, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	while(*input)
		input++;
	*rest = input;
}

void	parse_error(char *point, t_token **rest, t_token *token)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected character ", STDERR_FILENO);
	ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putstr_fd(" in ", STDERR_FILENO);
	ft_putstr_fd(point, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	while(token && is_eof(token))
		token = token->next;
	*rest = token;
}