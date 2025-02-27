/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 11:57:08 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * pwdコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1
 */
int	builtin_pwd(t_command *cmd, t_shell *shell)
{
	char	current_dir[PATH_MAX];

	(void)shell;

	/* 現在のディレクトリを取得 */
	if (getcwd(current_dir, PATH_MAX) == NULL)
	{
		system_error("pwd");
		return (1);
	}

	/* 現在のディレクトリを出力 */
	ft_putstr_fd(current_dir, cmd->output_fd);
	ft_putstr_fd("\n", cmd->output_fd);
	
	return (0);
}