/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ut.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:09:57 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/08 13:11:25 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_skip_command(t_command *current)
{
	if (!current->args || !current->args[0])
		return (1);
	return (0);
}

static void	fork_and_exec_child(t_command *cmd, t_shell *shell)
{
	default_signals();
	setup_child_io(cmd);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	exit(execute_external_forked(cmd, shell));
}

static void	handle_special_cat(t_command *current)
{
	int	null_fd;

	if (current->next == NULL || !current->next->args || !current->next->args[0])
	{
		if (ft_strcmp(current->args[0], "cat") == 0 && current->args[1] == NULL && 
			current->input_fd == STDIN_FILENO)
		{
			null_fd = open("/dev/null", O_RDONLY);
			if (null_fd != -1)
				current->input_fd = null_fd;
		}
	}
}

static pid_t	create_process(t_command *current, t_shell *shell, int stdin_backup)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		return (system_error("fork"), ERROR);
	}
	if (pid == 0)
	{
		close(stdin_backup);
		fork_and_exec_child(current, shell);
	}
	return (pid);
}

pid_t	static_execute_commands(t_command *current, t_shell *shell, int stdin_backup)
{
	pid_t	pid;
	pid_t	last_pid;

	last_pid = 0;
	while (current)
	{
		if (should_skip_command(current))
		{
			current = current->next;
			continue;
		}
		handle_special_cat(current);
		pid = create_process(current, shell, stdin_backup);
		if (pid == ERROR)
			return (ERROR);
		last_pid = pid;
		current = current->next;
	}
	return (last_pid);
}
