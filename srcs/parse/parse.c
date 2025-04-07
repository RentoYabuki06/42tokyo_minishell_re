/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:19:33 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:39:42 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_operater(t_token *token, char *op)
{
	if (token->type != TOKEN_OPERATOR)
		return (false);
	return (ft_strcmp(token->value, op) == 0);
}

static bool	is_control_operator(t_token *tok)
{
	static char *const	operators[] = {"|", "\n"};
	size_t				i;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (is_operater(tok, operators[i]))
			return (true);
		i++;
	}
	return (false);
}


static void	append_commands(t_node *command, t_token **rest, t_token *token, t_shell *shell)
{
	if (token->type == TOKEN_WORD)
	{
		append_token(&command->args, tokendup(token, shell));
		token = token->next;
	}
	else if (is_operater(token, ">") && token->next->type == TOKEN_WORD)
		append_node(&command->redirects, redirect_out(&token, token, shell));
	else if (is_operater(token, "<") && token->next->type == TOKEN_WORD)
		append_node(&command->redirects, redirect_in(&token, token, shell));
	else if (is_operater(token, ">>") && token->next->type == TOKEN_WORD)
		append_node(&command->redirects, redirect_append(&token, token, shell));
	else if (is_operater(token, "<<") && token->next->type == TOKEN_WORD)
		append_node(&command->redirects, redirect_heredoc(&token, token, shell));
	else
		shell->flag_error = false;
	*rest = token;
}

static t_node	*simple_command(t_token **rest, t_token *token, t_shell *shell)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD, shell);
	append_commands(node, &token, token, shell);
	while (token && !is_eof(token) && !is_control_operator(token))
		append_commands(node, &token, token, shell);
	*rest = token;
	return (node);
}

static t_node	*pipeline(t_token **rest, t_token *token, t_shell *shell)
{
	t_node	*node;

	node = new_node(ND_PIPELINE, shell);
	node->inpipe[0] = STDIN_FILENO;
	node->inpipe[1] = -1;
	node->outpipe[0] = -1;
	node->outpipe[1] = STDOUT_FILENO;
	node->command = simple_command(&token, token, shell);
	if (is_operater(token, "|"))
		node->next = pipeline(&token, token->next, shell);
	*rest = token;
	return (node);
}


void	parse(t_shell *shell)
{
	shell->node = pipeline(&(shell->token), shell->token, shell);
}
