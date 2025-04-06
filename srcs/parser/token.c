/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/06 17:43:14 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (value == NULL)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

int	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!tokens || !new_token)
		return (ERROR);
	if (!*tokens)
	{
		*tokens = new_token;
		return (SUCCESS);
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
	return (SUCCESS);
}
