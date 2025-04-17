/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:55:54 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/17 17:39:38 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_sigint_in_loop(int *pipe_fd, char *line)
{
	free(line);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (ERROR);
}

static char	*read_line_from_input(char **saved)
{
	char	*line;

	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO, saved);
	return (line);
}

static bool	is_delimiter_or_null(char *line, char *delimiter)
{
	int	len;
	int	result;

	if (line)
	{
		len = ft_strlen(line);
		if (len > 0)
		{
			if (line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
	}
	result = (!line || ft_strcmp(line, delimiter) == 0);
	return (result);
}

bool	loop(int pipe_fd[2], char *delimiter, char **saved)
{
	char	*line;
	bool	is_end;

	while (1)
	{
		line = read_line_from_input(saved);
		if (g_signal_status == SIGINT)
			return (handle_sigint_in_loop(pipe_fd, line));
		is_end = is_delimiter_or_null(line, delimiter);
		if (is_end)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
	}
	return (SUCCESS);
}

int	setup_redirect_heredoc(t_command *cmd, char *delimiter)
{
	int		pipe_fd[2];
	char	*saved;

	saved = ft_strdup("");
	if (saved == NULL)
		return (ERROR);
	if (pipe(pipe_fd) == -1)
	{
		system_error("pipe");
		return (ERROR);
	}
	setup_signal_heredoc();
	if (loop(pipe_fd, delimiter, &saved) == ERROR)
		return (ERROR);
	if (pipe_fd[1])
		close(pipe_fd[1]);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = pipe_fd[0];
	free(saved);
	return (SUCCESS);
}
