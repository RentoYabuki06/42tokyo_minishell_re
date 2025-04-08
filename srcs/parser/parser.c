/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/08 11:33:00 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	add_arg(t_command *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		size;

	size = 0;
	if (cmd->args)
	{
		while (cmd->args[size])
			size++;
	}
	new_args = malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return (error_message("Memory allocation error"), ERROR);
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[size] = ft_strdup(arg);
	new_args[size + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	return (SUCCESS);
}

static int	handle_redirect(t_token **tokens, t_command *cmd)
{
	t_token_type	type;
	char			*filename;
	t_token			*redirect_token;
	t_token			*filename_token;

	type = (*tokens)->type;
	redirect_token = create_token(type, ft_strdup((*tokens)->value));
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		free_tokens(redirect_token);
		error_message("Syntax error near unexpected token");
		return (ERROR);
	}
	filename = (*tokens)->value;
	filename_token = create_token(TOKEN_WORD, ft_strdup(filename));
	*tokens = (*tokens)->next;

	redirect_token->next = filename_token;

	if (!cmd->redirects)
	{
		cmd->redirects = redirect_token;
	}
	else
	{
		t_token *current = cmd->redirects;
		while (current->next)
			current = current->next;
		current->next = redirect_token;
	}

	return (SUCCESS);
}

static int	handle_pipe(t_token **tokens, t_command *cmd, t_command **commands)
{
	if (!cmd->args)
	{
		error_message("Syntax error near unexpected token `|'");
		return (ERROR);
	}
	add_command(commands, cmd);
	*tokens = (*tokens)->next;
	return (SUCCESS);
}

int	parse(t_shell *shell)
{
	t_token		*current_token;
	t_command	*current_cmd;

	if (!shell->tokens)
		return (SUCCESS);
	shell->commands = NULL;
	current_token = shell->tokens;
	current_cmd = create_command();
	if (!current_cmd)
	{
		error_message("Memory allocation error");
		return (ERROR);
	}
	while (current_token)
	{
		if (current_token->type == TOKEN_WORD)
		{
			add_arg(current_cmd, current_token->value);
			current_token = current_token->next;
		}
		else if (current_token->type == TOKEN_REDIRECT_IN
			|| current_token->type == TOKEN_REDIRECT_OUT
			|| current_token->type == TOKEN_HEREDOC
			|| current_token->type == TOKEN_APPEND)
		{
			if (handle_redirect(&current_token, current_cmd) == ERROR)
			{
				free_commands(current_cmd);
				return (ERROR);
			}
		}
		else if (current_token->type == TOKEN_PIPE)
		{
			if (handle_pipe(&current_token, current_cmd, &shell->commands) == ERROR)
			{
				free_commands(current_cmd);
				free_commands(shell->commands);
				shell->commands = NULL;
				return (ERROR);
			}
			current_cmd = create_command();
		}
		else
		{
			error_message("Parser error: unknown token type");
			free_commands(current_cmd);
			free_commands(shell->commands);
			shell->commands = NULL;
			return (ERROR);
		}
	}
	if (current_cmd->args || current_cmd->input_fd != STDIN_FILENO
		|| current_cmd->output_fd != STDOUT_FILENO)
		add_command(&shell->commands, current_cmd);
	else
		free_commands(current_cmd);
	return (SUCCESS);
}
