/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/06 18:08:39 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
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


void	free_shell(t_shell *shell)
{
	t_env	*tmp;
	t_env	*current;

	if (!shell)
		return ;
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->env_list)
	{
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
