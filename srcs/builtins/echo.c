/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 11:56:54 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * echoコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 常に0（成功）
 */
int	builtin_echo(t_command *cmd, t_shell *shell)
{
	int	i;
	int	n_flag;

	(void)shell;
	
	/* 引数がない場合は改行のみ */
	if (!cmd->args[1])
	{
		ft_putstr_fd("\n", cmd->output_fd);
		return (0);
	}

	/* -nオプションのチェック */
	i = 1;
	n_flag = 0;
	while (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}

	/* 引数を出力 */
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], cmd->output_fd);
		
		/* 最後の引数でなければスペースを出力 */
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", cmd->output_fd);
		
		i++;
	}

	/* -nオプションがない場合は改行を出力 */
	if (!n_flag)
		ft_putstr_fd("\n", cmd->output_fd);

	return (0);
}