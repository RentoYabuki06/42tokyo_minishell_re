/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:55:54 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/12 18:46:17 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	loop(int pipe_fd[2], char *delimiter, char **saved)
{
	char	*line;
	int len;

	while (true)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO, saved);
		if (g_signal_status == -1)
		{
			free(line);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			g_signal_status = 0;
			// free(saved);
			return (ERROR);
		}
		if (line)
		{
			len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
		if (!line || ft_strcmp(line, delimiter) == 0)
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
	close(pipe_fd[1]);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = pipe_fd[0];
	free(saved);
	return (SUCCESS);
}
