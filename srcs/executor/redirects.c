/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:54:49 by myokono           #+#    #+#             */
/*   Updated: 2025/04/12 21:56:22 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	setup_redirect_in(t_command *cmd, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		system_error(filename);
		return (ERROR);
	}
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = fd;
	return (SUCCESS);
}

static int	setup_redir_out(t_command *cmd, char *filename, t_token_type type)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (type == TOKEN_REDIRECT_OUT)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		system_error(filename);
		return (ERROR);
	}
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = fd;
	return (SUCCESS);
}

static int	heredoc(t_command *cmd, t_token *token, int last_fd, int heredoc_fd)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		if (!next)
			break ;
		if (token->type == TOKEN_HEREDOC)
		{
			heredoc_fd = setup_redir_return_fd(token->type, next->value);
			if (heredoc_fd == -1)
				return (ERROR);
			if (last_fd != -1)
				close(last_fd);
			last_fd = heredoc_fd;
		}
		token = next->next;
	}
	if (last_fd != -1)
	{
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		cmd->input_fd = last_fd;
	}
	return (SUCCESS);
}

static int	redir_in_helper(t_command *cmd)
{
	t_token	*token;
	t_token	*next;

	token = cmd->redirects;
	while (token)
	{
		next = token->next;
		if (!next)
		{
			error_message("Missing filename for redirection");
			return (ERROR);
		}
		if (token->type == TOKEN_REDIRECT_IN)
		{
			if (setup_redirect_in(cmd, token->value) == ERROR)
				return (ERROR);
		}
		token = next->next;
	}
	return (SUCCESS);
}

int	setup_redirects(t_command *cmd)
{
	t_token	*token;
	t_token	*next;

	token = cmd->redirects;
	if (heredoc(cmd, cmd->redirects, -1, 0) == false)
		return (ERROR);
	token = cmd->redirects;
	while (token)
	{
		next = token->next;
		if (!next)
			break ;
		if (token->type == TOKEN_REDIRECT_OUT || token->type == TOKEN_APPEND)
		{
			if (setup_redir_out(cmd, token->value, token->type) == ERROR)
				return (ERROR);
		}
		token = next->next;
	}
	if (redir_in_helper(cmd) == false)
		return (ERROR);
	return (SUCCESS);
}
