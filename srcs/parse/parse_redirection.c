/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:54:02 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:09:26 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redirect_out(t_token **rest, t_token *token, t_shell *shell)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT, shell);
	node->filename = tokendup(token->next, shell);
	node->targetfd = STDOUT_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_in(t_token **rest, t_token *token, t_shell *shell)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN, shell);
	node->filename = tokendup(token->next, shell);
	node->targetfd = STDIN_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_append(t_token **rest, t_token *token, t_shell *shell)
{
	t_node	*node;

	node = new_node(ND_REDIR_APPEND, shell);
	node->filename = tokendup(token->next, shell);
	node->targetfd = STDOUT_FILENO;
	*rest = token->next->next;
	return (node);
}

t_node	*redirect_heredoc(t_token **rest, t_token *token, t_shell *shell)
{
	t_node	*node;

	node = new_node(ND_REDIR_HEREDOC, shell);
	node->delimiter = tokendup(token->next, shell);
	if (ft_strchr(node->delimiter->value, SINGLE_QUOTE_CHAR) == NULL
		&& ft_strchr(node->delimiter->value, DOUBLE_QUOTE_CHAR) == NULL)
		node->is_delim_unquoted = true;
	node->targetfd = STDIN_FILENO;
	*rest = token->next->next;
	return (node);
}
