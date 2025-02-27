/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 11:57:56 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 環境変数の有効な識別子かチェックする関数
 * @param key 環境変数名
 * @return 有効であれば1、そうでなければ0
 */
static int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	
	/* 最初の文字は英字またはアンダースコアでなければならない */
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	
	/* 残りの文字は英数字またはアンダースコアでなければならない */
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	
	return (1);
}

/**
 * unsetコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1
 */
int	builtin_unset(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	
	/* 各引数を処理 */
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			command_error("unset", ft_strjoin(cmd->args[i], ": not a valid identifier"));
			status = 1;
		}
		else
		{
			/* 環境変数を削除 */
			remove_env_node(&shell->env_list, cmd->args[i]);
		}
		i++;
	}
	
	/* 環境変数配列を更新 */
	update_env_array(shell);
	
	return (status);
}