/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:15:00 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 18:29:11 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	readline_heredoc_loop(int pfd[2], const char *delimiter,
		bool is_delim_unquoted, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (is_delim_unquoted)
			line = expand_heredoc_line(line, shell);
		ft_putstr_fd(line, pfd[1]);
		ft_putstr_fd("\n", pfd[1]);
		free(line);
	}
}

int	read_heredoc(const char *delimiter, bool is_delim_unquoted, t_shell *shell)
{
	int		pfd[2];

	safe_pipe(pfd, shell);
	readline_heredoc_loop(pfd, delimiter, is_delim_unquoted, shell);
	safe_close(pfd[1], shell);
	return (pfd[0]);
}
