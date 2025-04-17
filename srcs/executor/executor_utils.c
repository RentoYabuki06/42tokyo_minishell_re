/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:30:19 by myokono           #+#    #+#             */
/*   Updated: 2025/04/17 19:35:06 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_token(t_token *tok)
{
	if (!tok)
		return ;
	if (tok->value)
		free(tok->value);
	free(tok);
}

static int	handle_heredoc_token(t_token **p, t_command *cmd)
{
	t_token	*tok;
	t_token	*next;
	int		fd;

	tok = *p;
	next = tok->next;
	if (!next)
		return (ERROR);
	fd = setup_redir_return_fd(TOKEN_HEREDOC, next->value);
	if (fd < 0)
		return (ERROR);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = fd;
	*p = next->next;
	free_token(next);
	free_token(tok);
	return (SUCCESS);
}

static int	process_heredocs_in_command(t_command *cmd)
{
	t_token	**p;
	t_token	*tok;
	int		result;

	p = &cmd->redirects;
	while (*p)
	{
		tok = *p;
		if (tok->type == TOKEN_HEREDOC)
		{
			result = handle_heredoc_token(p, cmd);
			if (result == ERROR)
				return (ERROR);
			continue ;
		}
		p = &tok->next;
	}
	return (SUCCESS);
}

int	process_heredocs_in_parent(t_shell *shell)
{
	t_command	*cmd;
	int			result;

	cmd = shell->commands;
	while (cmd)
	{
		result = process_heredocs_in_command(cmd);
		if (result == ERROR)
			return (ERROR);
		cmd = cmd->next;
	}
	return (SUCCESS);
}
