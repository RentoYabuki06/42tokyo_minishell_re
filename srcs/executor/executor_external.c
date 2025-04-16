/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:48:16 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 14:25:56 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_execve_error(char *exec_path)
{
	int code;
	int err = errno;
	struct stat st;

	code = 126;
	if (stat(exec_path, &st) == 0 && S_ISDIR(st.st_mode))
		print_error(exec_path, "Is a directory");
	else if (err == EACCES)
		print_error(exec_path, "Permission denied");
	else if (err == ENOENT)
	{
		print_error(exec_path, "No such file or directory");
		code = 127;
	}
	else
	{
		system_error(exec_path);
		code = 1;
	}
	exit (code);
}


static void	child_process(t_command *cmd, t_shell *shell, char *exec_path)
{
	if (cmd->input_fd != STDIN_FILENO)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
			system_error("dup2");
		close(cmd->input_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
			system_error("dup2");
		close(cmd->output_fd);
	}
	default_signals();
	execve(exec_path, cmd->args, shell->env_array);
	handle_execve_error(exec_path);
}

static int	wait_child_status(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (system_error("waitpid"), ERROR);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (ERROR);
}

int	execute_external_standalone(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*exec_path;

	exec_path = find_executable(cmd->args[0], shell->env_list);
	if (!exec_path)
	{
		print_error(cmd->args[0], "command not found");
		return (127);
	}
	ignore_signals();
	pid = fork();
	if (pid == -1)
	{
		free(exec_path);
		system_error("fork");
		return (ERROR);
	}
	if (pid == 0)
		child_process(cmd, shell, exec_path);
	free(exec_path);
	return (wait_child_status(pid));
}
