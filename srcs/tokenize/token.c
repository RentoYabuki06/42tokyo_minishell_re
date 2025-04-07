/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:46:49 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 16:47:20 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_token_type type, t_shell *shell)
{
	t_token	*token;

	token = safe_malloc(sizeof(t_token), shell);
	token->value = value;
	token->type = type;
	return (token);
}

t_token	*tokendup(t_token *token, t_shell *shell)
{
	char	*value;

	value = safe_strdup(token->value, shell);
		return (new_token(value, token->type, shell));
}

void	append_token(t_token **tok, t_token *elm)
{
	if (*tok == NULL)
	{
		*tok = elm;
		return ;
	}
	append_token(&(*tok)->next, elm);
}
