/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 12:09:49 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 安全なメモリ割り当てを行う関数
 * @param size 割り当てるバイト数
 * @return 割り当てられたメモリのポインタ
 */
void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		error_message("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

/**
 * 文字列配列を解放する関数
 * @param array 解放する文字列配列（NULL終端）
 */
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/**
 * トークンリストを解放する関数
 * @param tokens 解放するトークンリスト
 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

/**
 * コマンド構造体を解放する関数
 * @param commands 解放するコマンド構造体
 */
void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands)
	{
		tmp = commands->next;
		free_array(commands->args);
		/* リダイレクトトークンを解放 */
		if (commands->redirects)
			free_tokens(commands->redirects);
		/* 標準入出力以外のファイルディスクリプタを閉じる */
		if (commands->input_fd > 2)
			close(commands->input_fd);
		if (commands->output_fd > 2)
			close(commands->output_fd);
		free(commands);
		commands = tmp;
	}
}