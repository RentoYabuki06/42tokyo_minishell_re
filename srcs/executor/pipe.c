/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:23:15 by myokono           #+#    #+#             */
/*   Updated: 2025/02/27 12:23:18 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * パイプを設定する関数
 * @param commands コマンドリスト
 * @return 成功すればSUCCESS、失敗すればERROR
 */
int	setup_pipes(t_command *commands)
{
	t_command	*current;
	t_command	*next;
	int			pipe_fd[2];

	current = commands;
	while (current && current->next)
	{
		next = current->next;
		
		/* パイプを作成 */
		if (pipe(pipe_fd) == -1)
		{
			system_error("pipe");
			return (ERROR);
		}

		/* 現在のコマンドの出力を設定 */
		if (current->output_fd == STDOUT_FILENO)
			current->output_fd = pipe_fd[1];
		else
			close(pipe_fd[1]);

		/* 次のコマンドの入力を設定 */
		if (next->input_fd == STDIN_FILENO)
			next->input_fd = pipe_fd[0];
		else
			close(pipe_fd[0]);

		current = next;
	}

	return (SUCCESS);
}

/**
 * 子プロセスでのパイプとリダイレクトの設定
 * @param cmd 現在のコマンド
 */
void	setup_child_io(t_command *cmd)
{
	/* 入力リダイレクトを設定 */
	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}

	/* 出力リダイレクトを設定 */
	if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
}

/**
 * パイプラインでの複数コマンド実行
 * @param commands コマンドリスト
 * @param shell シェル構造体
 * @return 最後のコマンドの終了ステータス
 */
int	execute_pipeline(t_command *commands, t_shell *shell)
{
	t_command	*current;
	pid_t		pid;
	int			status;
	int			last_status;

	current = commands;
	last_status = 0;

	/* 各コマンドを子プロセスで実行 */
	while (current)
	{
		if (!current->args || !current->args[0])
		{
			current = current->next;
			continue;
		}

		pid = fork();
		if (pid == -1)
		{
			system_error("fork");
			return (1);
		}

		if (pid == 0)
		{
			/* 子プロセス */
			setup_child_io(current);
			
			if (is_builtin(current->args[0]))
				exit(execute_builtin(current, shell));
			else
				exit(execute_external(current, shell));
		}

		/* 親プロセス: 次のコマンドへ */
		if (current->input_fd != STDIN_FILENO)
			close(current->input_fd);
		if (current->output_fd != STDOUT_FILENO)
			close(current->output_fd);
		
		current = current->next;
	}

	/* すべての子プロセスの終了を待機 */
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
	}

	return (last_status);
}