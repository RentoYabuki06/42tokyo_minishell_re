/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:38:19 by myokono           #+#    #+#             */
/*   Updated: 2025/04/13 17:40:56 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	setup_redir_return_fd(t_token_type type, char *delimiter)
{
	int		pipe_fd[2];
	char	*saved;

	if (type != TOKEN_HEREDOC)
		return (-1);
	saved = ft_strdup("");
	if (!saved)
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (-1);
	setup_signal_heredoc();
	if (!loop(pipe_fd, delimiter, &saved))
	{
		free(saved);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	free(saved);
	return (pipe_fd[0]);
}

int	handle_heredoc_fd(int *heredoc_fd, char *filename)
{
	int		new_fd;
	int		old_fd;

	old_fd = *heredoc_fd;
	if (old_fd != -1)
		close(old_fd);
	new_fd = setup_redir_return_fd(TOKEN_HEREDOC, filename);
	if (new_fd == -1)
		return (ERROR);
	*heredoc_fd = new_fd;
	return (SUCCESS);
}

int	process_redirect(t_command *cmd, t_token *token, \
		t_token *next, int *heredoc_fd)
{
	int	status;
	int	type;

	if (!next || !next->value)
		return (error_message("Missing filename for redirection"), ERROR);
	type = token->type;
	if (type == TOKEN_HEREDOC)
	{
		status = handle_heredoc_fd(heredoc_fd, next->value);
		if (status == ERROR)
			return (ERROR);
	}
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND || \
			type == TOKEN_REDIRECT_IN)
	{
		status = setup_redir(cmd, type, next->value);
		if (status == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}
