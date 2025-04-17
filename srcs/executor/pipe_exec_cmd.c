/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:18:06 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/17 18:50:32 by myokono          ###   ########.fr       */
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
	if (setup_redirects(cmd) == ERROR)
		exit(ERROR);
	setup_child_io(cmd);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	exit(execute_external_forked(cmd, shell));
}

static void	handle_special_cat(t_command *current)
{
	int			null_fd;
	t_command	*next;

	next = current->next;
	if (next == NULL || !next->args || !next->args[0])
	{
		if (ft_strcmp(current->args[0], "cat") == 0
			&& current->args[1] == NULL && current->input_fd == STDIN_FILENO)
		{
			null_fd = open("/dev/null", O_RDONLY);
			if (null_fd != -1)
				current->input_fd = null_fd;
		}
	}
}

static pid_t	new_process(t_command *current, t_shell *shell, int stdin)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		dup2(stdin, STDIN_FILENO);
		close(stdin);
		return (system_error("fork"), ERROR);
	}
	if (pid == 0)
	{
		close(stdin);
		fork_and_exec_child(current, shell);
	}
	return (pid);
}

pid_t	static_execute_commands(t_command *current, t_shell *shell, int stdin)
{
	pid_t	pid;
	pid_t	last_pid;

	last_pid = 0;
	while (current)
	{
		if (should_skip_command(current))
		{
			current = current->next;
			continue ;
		}
		handle_special_cat(current);
		pid = new_process(current, shell, stdin);
		if (pid == ERROR)
			return (ERROR);
		last_pid = pid;
		current = current->next;
	}
	return (last_pid);
}
