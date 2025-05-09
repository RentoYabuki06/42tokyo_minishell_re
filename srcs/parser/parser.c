/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:52:05 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/17 13:47:10 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_token(t_token **current_token, t_command **current_cmd,
	t_command **commands)
{
	if ((*current_token)->type == TOKEN_WORD)
	{
		add_arg(*current_cmd, (*current_token)->value);
		*current_token = (*current_token)->next;
	}
	else if ((*current_token)->type == TOKEN_REDIRECT_IN
		|| (*current_token)->type == TOKEN_REDIRECT_OUT
		|| (*current_token)->type == TOKEN_HEREDOC
		|| (*current_token)->type == TOKEN_APPEND)
	{
		if (handle_redirect(current_token, *current_cmd) == ERROR)
			return (ERROR);
	}
	else if ((*current_token)->type == TOKEN_PIPE)
	{
		if (handle_pipe(current_token, *current_cmd, commands) == ERROR)
			return (ERROR);
		*current_cmd = create_command();
	}
	else
		return (ERROR);
	return (SUCCESS);
}

static int	finalize_parse(t_command *current_cmd, t_command **commands)
{
	if (current_cmd->args || current_cmd->input_fd != STDIN_FILENO
		|| current_cmd->output_fd != STDOUT_FILENO || current_cmd->redirects)
		add_command(commands, current_cmd);
	else
		free_commands(current_cmd);
	return (SUCCESS);
}

int	parse(t_shell *shell)
{
	t_token		*cur_token;
	t_command	*cur_cmd;

	if (!shell->tokens)
		return (SUCCESS);
	shell->commands = NULL;
	cur_token = shell->tokens;
	cur_cmd = create_command();
	if (!cur_cmd)
		return (error_message("Memory allocation error"), ERROR);
	while (cur_token)
	{
		if (process_token(&cur_token, &cur_cmd, &shell->commands) == ERROR)
			return (free_commands(cur_cmd), ERROR);
	}
	return (finalize_parse(cur_cmd, &shell->commands));
}
