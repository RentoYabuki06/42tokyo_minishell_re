/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 12:37:02 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* シグナル処理用グローバル変数 */
int	g_signal_status = 0;

/**
 * シェルを初期化する関数
 * @param envp 環境変数配列
 * @return 初期化されたシェル構造体
 */
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = safe_malloc(sizeof(t_shell));
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->env_list = init_env_list(envp);
	shell->env_array = env_list_to_array(shell->env_list);
	shell->exit_status = 0;
	shell->running = 1;
	return (shell);
}

/**
 * シェルのメモリを解放する関数
 * @param shell シェル構造体
 */
void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->env_list)
	{
		t_env	*tmp;
		t_env	*current;

		current = shell->env_list;
		while (current)
		{
			tmp = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = tmp;
		}
	}
	if (shell->env_array)
		free_array(shell->env_array);
	free(shell);
}

/**
 * 入力行を処理する関数
 * @param input 入力文字列
 * @param shell シェル構造体
 * @return 処理結果
 */
static int	process_input(char *input, t_shell *shell)
{
	if (!input || ft_strlen(input) == 0)
		return (SUCCESS);

	/* 入力を履歴に追加 */
	add_history(input);

	/* 入力の字句解析（トークン化） */
	shell->tokens = tokenize(input, shell);
	if (!shell->tokens)
	{
		free(input);
		return (ERROR);
	}

	/* 構文解析（コマンド構造の構築） */
	if (parse(shell) != SUCCESS)
	{
		free(input);
		return (ERROR);
	}

	/* コマンドの実行 */
	shell->exit_status = execute_commands(shell);

	/* トークンとコマンドのメモリ解放 */
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_commands(shell->commands);
	shell->commands = NULL;

	free(input);
	return (SUCCESS);
}

/**
 * メイン関数
 * @param argc 引数の数
 * @param argv 引数配列
 * @param envp 環境変数配列
 * @return 終了ステータス
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*input;

	(void)argc;
	(void)argv;

	/* シェルの初期化 */
	shell = init_shell(envp);
	/* シグナルハンドラの設定 */
	setup_signals();

	/* メインループ */
	while (shell->running)
	{
		/* プロンプトを表示して入力を受け取る */
		input = readline("minishell$ ");
		
		/* EOF（Ctrl+D）を検出した場合は終了 */
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		
		/* 入力を処理 */
		process_input(input, shell);
	}

	/* シェルのメモリを解放 */
	free_shell(shell);
	
	/* readline関連のメモリを解放 */
	// rl_clear_history();
	clear_history();
	
	return (shell->exit_status);
}
