/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:54:49 by myokono           #+#    #+#             */
/*   Updated: 2025/04/08 14:08:36 by yabukirento      ###   ########.fr       */
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

static int	setup_redirect_out(t_command *cmd, \
		char *filename, t_token_type type)
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

static int	setup_redirect_heredoc(t_command *cmd, char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		system_error("pipe");
		return (ERROR);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = pipe_fd[0];
	return (SUCCESS);
}

static int	setup_redir(t_command *cmd, t_token_type type, char *filename)
{
	if (type == TOKEN_REDIRECT_IN)
		return (setup_redirect_in(cmd, filename));
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND)
		return (setup_redirect_out(cmd, filename, type));
	else if (type == TOKEN_HEREDOC)
		return (setup_redirect_heredoc(cmd, filename));
	return (ERROR);
}

int	setup_redirects(t_command *cmd)
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
		if (setup_redir(cmd, token->type, next->value) == ERROR)
			return (ERROR);
		token = next->next;
	}
	return (SUCCESS);
}
