/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 11:57:37 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * envコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1
 */
int	builtin_env(t_command *cmd, t_shell *shell)
{
	t_env	*current;

	/* 引数がある場合はエラー（仕様による制約） */
	if (cmd->args[1])
	{
		error_message("env: too many arguments");
		return (1);
	}

	/* すべての環境変数を表示 */
	current = shell->env_list;
	while (current)
	{
		/* 値が存在する環境変数のみ表示 */
		if (current->value && *current->value)
		{
			ft_putstr_fd(current->key, cmd->output_fd);
			ft_putstr_fd("=", cmd->output_fd);
			ft_putstr_fd(current->value, cmd->output_fd);
			ft_putstr_fd("\n", cmd->output_fd);
		}
		current = current->next;
	}
	
	return (0);
}