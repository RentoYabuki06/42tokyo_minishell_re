/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:55:54 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/12 21:58:47 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_sigint_in_loop(int *pipe_fd, char *line)
{
	free(line);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	g_signal_status = 0;
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

static bool	loop(int pipe_fd[2], char *delimiter, char **saved)
{
	char	*line;
	bool	is_end;

	while (true)
	{
		line = read_line_from_input(saved);
		if (g_signal_status == -1)
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
	return (true);
}

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
	if (loop(pipe_fd, delimiter, &saved) == ERROR)
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
