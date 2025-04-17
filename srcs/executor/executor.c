/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:06:57 by myokono           #+#    #+#             */
/*   Updated: 2025/04/17 19:27:58 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redirect_if_needed(int src_fd, int dest_fd)
{
	if (src_fd != dest_fd)
	{
		if (dup2(src_fd, dest_fd) == -1)
			system_error("dup2");
		close(src_fd);
	}
}

static int	execute_builtin_forked(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (system_error("fork"), ERROR);
	if (pid == 0)
	{
		redirect_if_needed(cmd->input_fd, STDIN_FILENO);
		redirect_if_needed(cmd->output_fd, STDOUT_FILENO);
		status = execute_builtin(cmd, shell);
		exit(status);
	}
	if (waitpid(pid, &status, 0) == -1)
		return (system_error("waitpid"), ERROR);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (ERROR);
}

static int	execute_command(t_command *cmd, t_shell *shell)
{
	int	status;

	if (setup_redirects(cmd) == ERROR)
		return (ERROR);
	if (!cmd->args || !cmd->args[0])
		return (SUCCESS);
	if (is_builtin(cmd->args[0]))
	{
		if (cmd->input_fd != STDIN_FILENO || cmd->output_fd != STDOUT_FILENO)
			return (execute_builtin_forked(cmd, shell));
		return (execute_builtin(cmd, shell));
	}
	status = execute_external_standalone(cmd, shell);
	return (status);
}

static void	free_token(t_token *tok)
{
	if (!tok)
		return ;
	if (tok->value)
		free(tok->value);
	free(tok);
}

static int	process_heredocs_in_parent(t_shell *shell)
{
	t_command	*cmd;
	t_token		**p;
	t_token		*next;
	t_token		*tok;
	int			fd;

	cmd = shell->commands;
	while (cmd)
	{
		p = &cmd->redirects;
		while (*p)
		{
			tok = *p;
			if (tok->type == TOKEN_HEREDOC)
			{
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
				continue ;
			}
			p = &tok->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}

int	execute_commands(t_shell *shell)
{
	if (!shell->commands)
		return (SUCCESS);
	if (shell->commands->next)
	{
		if (process_heredocs_in_parent(shell) == ERROR)
			return (ERROR);
		if (setup_pipes(shell->commands) != SUCCESS)
			return (ERROR);
		return (execute_pipeline(shell->commands, shell));
	}
	if (process_heredocs_in_parent(shell) == ERROR)
		return (ERROR);
	return (execute_command(shell->commands, shell));
}
