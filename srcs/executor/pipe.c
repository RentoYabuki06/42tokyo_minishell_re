/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:23:15 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 21:57:13 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			else if (WTERMSIG(status))
			{
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				else if (WTERMSIG(status) == SIGQUIT)
					write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
				else if (WTERMSIG(status) == SIGPIPE)
					write(STDERR_FILENO, "Broken pipe\n", 13);				
				last_status = 128 + WTERMSIG(status);
			}
		}
		pid = wait(&status);
	}
	return (last_status);
}

static void	prepare_pipeline(t_command **current, t_command **cmd_list,
	t_command *commands, int *stdin_backup)
{
	*current = commands;
	*cmd_list = commands;
	ignore_signals();
	*stdin_backup = dup(STDIN_FILENO);
}

static void	close_command_fds(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->input_fd != STDIN_FILENO)
			close(current->input_fd);
		if (current->output_fd != STDOUT_FILENO)
			close(current->output_fd);
		current = current->next;
	}
}

static void	restore_stdin(int stdin_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}

int	execute_pipeline(t_command *commands, t_shell *shell)
{
	t_command	*current;
	t_command	*cmd_list;
	int			stdin_backup;
	pid_t		last_pid;

	prepare_pipeline(&current, &cmd_list, commands, &stdin_backup);
	last_pid = static_execute_commands(current, shell, stdin_backup);
	if (last_pid == ERROR)
		return (ERROR);
	close_command_fds(cmd_list);
	restore_stdin(stdin_backup);
	return (wait_for_last_pid(last_pid));
}
