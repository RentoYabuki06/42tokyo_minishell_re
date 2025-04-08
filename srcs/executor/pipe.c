/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:23:15 by myokono           #+#    #+#             */
/*   Updated: 2025/04/08 12:19:13 by yabukirento      ###   ########.fr       */
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
	t_command	*cmd_list;
	int			stdin_backup;

	current = commands;
	cmd_list = commands;
	ignore_signals();
	stdin_backup = dup(STDIN_FILENO); // 標準入力のバックアップを取る
	
	while (current)
	{
		if (!current->args || !current->args[0])
		{
			current = current->next;
			continue ;
		}
		
		// 最後のコマンドが標準入力を読む場合（catなど）は特別な処理
		if (current->next == NULL || !current->next->args || !current->next->args[0])
		{
			if (ft_strcmp(current->args[0], "cat") == 0 && current->args[1] == NULL && 
				current->input_fd == STDIN_FILENO)
			{
				// ヌルデバイスから入力を読むようにする
				int null_fd = open("/dev/null", O_RDONLY);
				if (null_fd != -1)
				{
					current->input_fd = null_fd;
				}
			}
		}
		
		pid = fork();
		if (pid == -1)
		{
			dup2(stdin_backup, STDIN_FILENO); // 元の標準入力に戻す
			close(stdin_backup);
			return (system_error("fork"), ERROR);
		}
		if (pid == 0)
		{
			close(stdin_backup); // 子プロセスでは不要
			fork_and_exec_child(current, shell);
		}
		last_pid = pid;
		current = current->next;
	}

	// すべての子プロセスを作成した後にFDをクローズ
	current = cmd_list;
	while (current)
	{
		if (current->input_fd != STDIN_FILENO)
			close(current->input_fd);
		if (current->output_fd != STDOUT_FILENO)
			close(current->output_fd);
		current = current->next;
	}

	// 元の標準入力に戻す
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	
	return (wait_for_last_pid(last_pid));
}
