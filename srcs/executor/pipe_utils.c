/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 20:34:01 by myokono           #+#    #+#             */
/*   Updated: 2025/04/18 10:21:29 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external_forked(t_command *cmd, t_shell *shell)
{
	char	*exec_path;
	int		code;

	exec_path = find_executable(cmd->args[0], shell->env_list);
	if (!exec_path)
	{
		print_error(cmd->args[0], "command not found");
		return (127);
	}
	execve(exec_path, cmd->args, shell->env_array);
	system_error(exec_path);
	if (access(exec_path, F_OK) != 0)
		code = 127;
	else if (access(exec_path, X_OK) != 0)
		code = 126;
	else
		code = 1;
	free(exec_path);
	exit(code);
}

static void	cleanup_pipes(t_command *commands)
{
	t_command	*cmd;

	cmd = commands;
	while (cmd)
	{
		if (cmd->input_fd != STDIN_FILENO)
		{
			close(cmd->input_fd);
			cmd->input_fd = STDIN_FILENO;
		}
		if (cmd->output_fd != STDOUT_FILENO)
		{
			close(cmd->output_fd);
			cmd->output_fd = STDOUT_FILENO;
		}
		cmd = cmd->next;
	}
}

int	setup_pipes(t_command *commands)
{
	t_command	*current;
	t_command	*next;
	int			pipe_fd[2];

	current = commands;
	while (current && current->next)
	{
		next = current->next;
		if (pipe(pipe_fd) == -1)
		{
			system_error("pipe");
			cleanup_pipes(commands);
			return (ERROR);
		}
		if (current->output_fd == STDOUT_FILENO)
			current->output_fd = pipe_fd[1];
		else
			close(pipe_fd[1]);
		if (next->input_fd == STDIN_FILENO)
			next->input_fd = pipe_fd[0];
		else
			close(pipe_fd[0]);
		current = next;
	}
	return (SUCCESS);
}

static void	setup_child_io_redirections(t_command *cmd)
{
	if (cmd->input_fd != STDIN_FILENO)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
		{
			system_error("dup2");
			close(cmd->input_fd);
			exit(EXIT_FAILURE);
		}
		if (cmd->input_fd != cmd->output_fd)
			close(cmd->input_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
		{
			system_error("dup2");
			close(cmd->output_fd);
			exit(EXIT_FAILURE);
		}
		close(cmd->output_fd);
	}
}

void	setup_child_io(t_command *cmd)
{
	int	i;

	default_signals();
	setup_child_io_redirections(cmd);
	i = 3;
	while (i < MAXNUM_FD)
	{
		if (i != cmd->input_fd && i != cmd->output_fd)
			close(i);
		i++;
	}
}
