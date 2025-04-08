/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:23:15 by myokono           #+#    #+#             */
/*   Updated: 2025/04/08 12:03:01 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	fork_and_exec_child(t_command *cmd, t_shell *shell)
{
	default_signals();
	setup_child_io(cmd);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	exit(execute_external_forked(cmd, shell));
}

static void	close_fds_and_record_pid(t_command *cmd, pid_t pid, pid_t *last_pid)
{
	*last_pid = pid;
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
}

static int	wait_for_last_pid(pid_t last_pid)
{
	int		status;
	int		last_status;
	pid_t	pid;

	last_status = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
	return (last_status);
}

int	execute_pipeline(t_command *commands, t_shell *shell)
{
	t_command	*current;
	pid_t		pid;
	pid_t		last_pid;

	current = commands;
	ignore_signals();
	while (current)
	{
		if (!current->args || !current->args[0])
		{
			current = current->next;
			continue ;
		}
		pid = fork();
		if (pid == -1)
			return (system_error("fork"), ERROR);
		if (pid == 0)
			fork_and_exec_child(current, shell);
		close_fds_and_record_pid(current, pid, &last_pid);
		current = current->next;
	}
	return (wait_for_last_pid(last_pid));
}
