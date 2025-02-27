/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:25:47 by myokono           #+#    #+#             */
/*   Updated: 2025/02/27 12:55:30 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * PWDとOLDPWD環境変数を更新する関数
 * @param shell シェル構造体
 * @param old_pwd 以前のPWD
 */
static void	update_pwd_env(t_shell *shell, char *old_pwd)
{
	char	current_pwd[PATH_MAX];

	/* 現在のディレクトリを取得 */
	if (getcwd(current_pwd, PATH_MAX) == NULL)
	{
		system_error("getcwd");
		return ;
	}

	/* PWD環境変数を更新 */
	add_env_node(&shell->env_list, "PWD", current_pwd);
	
	/* OLDPWD環境変数を更新 */
	if (old_pwd)
		add_env_node(&shell->env_list, "OLDPWD", old_pwd);
	
	/* 環境変数配列を更新 */
	update_env_array(shell);
}

/**
 * cdコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1
 */
int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	/* 現在のディレクトリを取得 */
	if (getcwd(old_pwd, PATH_MAX) == NULL)
	{
		system_error("getcwd");
		return (1);
	}
	/* 引数がない場合はHOMEディレクトリに移動 */
	if (!cmd->args[1])
	{
		path = get_env_value(shell->env_list, "HOME");
		if (!path)
		{
			error_message("cd: HOME not set");
			return (1);
		}
	}
	else
	{
		path = cmd->args[1];
	}
	/* ディレクトリを変更 */
	if (chdir(path) != 0)
	{
		system_error(path);
		return (1);
	}

	/* PWDとOLDPWD環境変数を更新 */
	update_pwd_env(shell, old_pwd);
	
	return (0);
}
