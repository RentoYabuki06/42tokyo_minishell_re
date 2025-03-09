/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:25:47 by myokono           #+#    #+#             */
/*   Updated: 2025/03/10 08:05:58 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * チルダ(~)をホームディレクトリに展開する関数
 * @param path 元のパス文字列
 * @param shell シェル構造体
 * @return 展開後のパス（メモリ割り当てが必要）
 */
static char	*expand_tilde(char *path, t_shell *shell)
{
	char	*home;
	char	*expanded_path;

	if (path[0] != '~')
		return (ft_strdup(path));
	
	home = get_env_value(shell->env_list, "HOME");
	if (!home)
	{
		error_message("cd: HOME not set");
		return (NULL);
	}

	if (path[1] == '\0') // 単に "~" の場合
		return (ft_strdup(home));
	else if (path[1] == '/') // "~/something" の場合
	{
		expanded_path = ft_strjoin(home, path + 1);
		return (expanded_path);
	}
	else // "~user" などの場合は現在サポートしない
		return (ft_strdup(path));
}

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
	char	*expanded_path;
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
		expanded_path = ft_strdup(path);
		if (!expanded_path)
		{
			system_error("malloc");
			return (1);
		}
	}
	else
	{
		/* チルダ展開を行う */
		expanded_path = expand_tilde(cmd->args[1], shell);
		if (!expanded_path)
			return (1);
	}

	/* ディレクトリを変更 */
	if (chdir(expanded_path) != 0)
	{
		system_error(expanded_path);
		free(expanded_path);
		return (1);
	}
	free(expanded_path);
	/* PWDとOLDPWD環境変数を更新 */
	update_pwd_env(shell, old_pwd);
	return (0);
}
