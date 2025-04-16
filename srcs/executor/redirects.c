/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:54:49 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 13:06:06 by yabukirento      ###   ########.fr       */
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
		printf("file name is %s\n", filename); //test
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
		printf("file name is %s\n", filename); ///test
		return (ERROR);
	}
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = fd;
	return (SUCCESS);
}

int	setup_redir(t_command *cmd, t_token_type type, char *filename)
{
	if (type == TOKEN_REDIRECT_IN)
		return (setup_redirect_in(cmd, filename));
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND)
		return (setup_redir_out(cmd, filename, type));
	else if (type == TOKEN_HEREDOC)
		return (setup_redirect_heredoc(cmd, filename));
	return (ERROR);
}

static void	apply_heredoc_fd(t_command *cmd, int heredoc_fd)
{
	int	old_fd;

	if (heredoc_fd != -1)
	{
		old_fd = cmd->input_fd;
		if (old_fd != STDIN_FILENO)
			close(old_fd);
		cmd->input_fd = heredoc_fd;
	}
}

int	setup_redirects(t_command *cmd)
{
	t_token	*token;
	t_token	*next;
	int		status;
	int		heredoc_fd;

	heredoc_fd = -1;
	token = cmd->redirects;
	while (token)
	{
		next = token->next;
		status = process_redirect(cmd, token, next, &heredoc_fd);
		if (status == ERROR)
			return (ERROR);
		token = next->next;
	}
	apply_heredoc_fd(cmd, heredoc_fd);
	return (SUCCESS);
}
