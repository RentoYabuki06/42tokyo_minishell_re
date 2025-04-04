/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/04 17:54:41 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 指定されたコマンドがビルトインコマンドかチェックする関数
 * @param cmd コマンド名
 * @return ビルトインコマンドならば1、そうでなければ0
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

/**
 * ビルトインコマンドを実行する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return コマンドの終了ステータス
 */
int	execute_builtin(t_command *cmd, t_shell *shell)
{
	char	*command;
	int		status;

	command = cmd->args[0];
	status = 0;

	/* 各ビルトインコマンドに分岐 */
	if (ft_strcmp(command, "echo") == 0)
		status = builtin_echo(cmd, shell);
	else if (ft_strcmp(command, "cd") == 0)
		status = builtin_cd(cmd, shell);
	else if (ft_strcmp(command, "pwd") == 0)
		status = builtin_pwd(cmd, shell);
	else if (ft_strcmp(command, "export") == 0)
		status = builtin_export(cmd, shell);
	else if (ft_strcmp(command, "unset") == 0)
		status = builtin_unset(cmd, shell);
	else if (ft_strcmp(command, "env") == 0)
		status = builtin_env(cmd, shell);
	else if (ft_strcmp(command, "exit") == 0)
		status = builtin_exit(cmd, shell);

	return (status);
}

/**
 * パスからコマンド実行ファイルを探す関数
 * @param cmd コマンド名
 * @param env_list 環境変数リスト
 * @return 実行ファイルのパス（見つからない場合はNULL）
 */
char	*find_executable(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*exec_path;
	int		i;

	/* 絶対パスまたは相対パスの場合は直接チェック */
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}

	/* PATH環境変数を取得 */
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);

	/* PATHを分割 */
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

	/* 各パスでコマンドを検索 */
	i = 0;
	while (paths[i])
	{
		exec_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
		if (access(exec_path, X_OK) == 0)
		{
			free_array(paths);
			return (exec_path);
		}
		free(exec_path);
		i++;
	}

	free_array(paths);
	return (NULL);
}

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
		return (128 + WTERMSIG(status));

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
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (!cmd->args || !cmd->args[0])
		return (0);

	/* 標準入出力を保存 */
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);

	/* リダイレクトを設定 */
	if (cmd->input_fd != STDIN_FILENO)
		dup2(cmd->input_fd, STDIN_FILENO);
	if (cmd->output_fd != STDOUT_FILENO)
		dup2(cmd->output_fd, STDOUT_FILENO);

	/* ビルトインコマンドか外部コマンドを実行 */
	if (is_builtin(cmd->args[0]))
		status = execute_builtin(cmd, shell);
	else
		status = execute_external_standalone(cmd, shell);

	/* 標準入出力を復元 */
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);

	return (status);
}

/**
 * すべてのコマンドを実行する関数
 * @param shell シェル構造体
 * @return 最後のコマンドの終了ステータス
 */
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




int	do_one_command(char *input, t_shell *shell)
{
	process_input(ft_strdup(input), shell);
	free_shell(shell);
	clear_history();
	exit (shell->exit_status);
}
