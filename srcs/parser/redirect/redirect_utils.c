/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:00 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/08 14:31:36 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_token	*process_redirect_tokens(t_token **tokens, t_token_type type)
{
	t_token	*redirect_token;
	t_token	*filename_token;

	redirect_token = create_token(type, ft_strdup((*tokens)->value));
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		free_tokens(redirect_token);
		error_message("Syntax error near unexpected token");
		return (NULL);
	}
	filename_token = create_token(TOKEN_WORD, ft_strdup((*tokens)->value));
	*tokens = (*tokens)->next;

	redirect_token->next = filename_token;
	return (redirect_token);
}

void	add_redirect_to_command(t_command *cmd, t_token *redirect_token)
{
	t_token	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = redirect_token;
	}
	else
	{
		current = cmd->redirects;
		while (current->next)
			current = current->next;
		current->next = redirect_token;
	}
}

int	handle_redirect(t_token **tokens, t_command *cmd)
{
	t_token_type	type;
	t_token			*redirect_token;

	type = (*tokens)->type;
	redirect_token = process_redirect_tokens(tokens, type);
	if (!redirect_token)
		return (ERROR);
	
	add_redirect_to_command(cmd, redirect_token);
	return (SUCCESS);
} 