/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/07 17:38:19 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 外部コマンドを実行する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return コマンドの終了ステータス
 */
int	execute_external_forked(t_command *cmd, t_shell *shell)
{
	char	*exec_path;

	exec_path = find_executable(cmd->args[0], shell->env_list);
	if (!exec_path)
	{
		command_error(cmd->args[0], "command not found");
		return (127);
	}

	execve(exec_path, cmd->args, shell->env_array);

	system_error(exec_path);
	free(exec_path);
	exit(126); // execve失敗時は子プロセスから exit
}
int	execute_external_standalone(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*exec_path;

	exec_path = find_executable(cmd->args[0], shell->env_list);
	if (!exec_path)
	{
		command_error(cmd->args[0], "command not found");
		return (127);
	}

	ignore_signals();
	pid = fork();
	if (pid == -1)
	{
		free(exec_path);
		system_error("fork");
		return (1);
	}

	if (pid == 0)
	{
		// 子プロセス内でリダイレクト
		if (cmd->input_fd != STDIN_FILENO)
		{
			dup2(cmd->input_fd, STDIN_FILENO);
			close(cmd->input_fd);
		}
		if (cmd->output_fd != STDOUT_FILENO)
		{
			dup2(cmd->output_fd, STDOUT_FILENO);
			close(cmd->output_fd);
		}
		defalut_signals();
		setup_child_signals();
		execve(exec_path, cmd->args, shell->env_array);

		// execve 失敗時
		system_error(exec_path);
		free(exec_path);
		exit(126);
	}

	free(exec_path);
	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + WTERMSIG(status));
	}
	return (1);
}


/**
 * 単一コマンドを実行する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return コマンドの終了ステータス
 */
static int	execute_command(t_command *cmd, t_shell *shell)
{
	int	status;

	// Call setup_redirects to apply redirection settings
	if (setup_redirects(cmd) == ERROR)
	{
		return (1);
	}

	if (!cmd->args || !cmd->args[0])
	{
		return (0);
	}

	if (is_builtin(cmd->args[0]))
	{
		// built-in でリダイレクトが必要な場合は子プロセスで実行
		if (cmd->input_fd != STDIN_FILENO || cmd->output_fd != STDOUT_FILENO)
		{
			pid_t	pid = fork();
			if (pid == 0)
			{
				if (cmd->input_fd != STDIN_FILENO)
				{
					dup2(cmd->input_fd, STDIN_FILENO);
					close(cmd->input_fd);
				}
				if (cmd->output_fd != STDOUT_FILENO)
				{
					dup2(cmd->output_fd, STDOUT_FILENO);
					close(cmd->output_fd);
				}
				status = execute_builtin(cmd, shell);
				exit(status);
			}
			waitpid(pid, &status, 0);
		}
		else
			status = execute_builtin(cmd, shell);
	}
	else
		status = execute_external_standalone(cmd, shell);
	return (status);
}

int	execute_commands(t_shell *shell)
{
	if (!shell->commands)
		return (0);
	if (!shell->commands->next)
		return (execute_command(shell->commands, shell));
	if (setup_pipes(shell->commands) != SUCCESS)
		return (1);
	return (execute_pipeline(shell->commands, shell));
}
