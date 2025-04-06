/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/07 02:00:58 by myokono          ###   ########.fr       */
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

int	builtin_unset(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			command_error("unset", ft_strjoin(cmd->args[i], ": not a valid identifier"));
			status = 1;
		}
		else
			remove_env_node(&shell->env_list, cmd->args[i]);
		i++;
	}
	update_env_array(shell);
	if (!shell->env_array)
	{
		free_shell(shell);
		system_error("update_env_array");
		return (1);
	}
	return (status);
}
