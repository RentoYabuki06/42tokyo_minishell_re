/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/06 21:46:00 by myokono          ###   ########.fr       */
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
 * 環境変数をアルファベット順にソートして表示する関数
 * @param env_list 環境変数リスト
 * @param fd 出力先ファイルディスクリプタ
 */
static void	print_sorted_env(t_env *env_list, int fd)
{
	t_env	**env_array;
	t_env	*current;
	int		count;
	int		i;
	int		j;

	/* 環境変数の数を数える */
	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}

	/* 環境変数の配列を作成 */
	env_array = (t_env **)malloc(sizeof(t_env *) * count);
	current = env_list;
	i = 0;
	while (current)
	{
		env_array[i++] = current;
		current = current->next;
	}

	/* バブルソートで環境変数をソート */
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_array[j]->key, env_array[j + 1]->key) > 0)
			{
				current = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = current;
			}
			j++;
		}
		i++;
	}

	/* ソートされた環境変数を表示 */
	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(env_array[i]->key, fd);
		ft_putstr_fd("=\"", fd);
		ft_putstr_fd(env_array[i]->value, fd);
		ft_putstr_fd("\"\n", fd);
		i++;
	}

	free(env_array);
}

/**
 * 環境変数を追加する関数
 * @param arg 引数（形式: KEY=VALUE または KEY）
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1
 */
static int	add_export(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
	{
		/* KEY のみの場合 */
		key = ft_strdup(arg);
		if (!is_valid_identifier(key))
		{
			command_error("export", ft_strjoin(key, ": not a valid identifier"));
			free(key);
			return (1);
		}
		
		/* 既に存在するなら何もしない、存在しなければ空の値で追加 */
		if (!get_env_node(shell->env_list, key))
			add_env_node(&shell->env_list, key, "");
		
		free(key);
	}
	else
	{
		/* KEY=VALUE の場合 */
		key = ft_substr(arg, 0, equals_pos - arg);
		value = ft_strdup(equals_pos + 1);
		
		if (!is_valid_identifier(key))
		{
			command_error("export", ft_strjoin(key, ": not a valid identifier"));
			free(key);
			free(value);
			return (1);
		}
		
		add_env_node(&shell->env_list, key, value);
		
		free(key);
		free(value);
	}
	
	/* 環境変数配列を更新 */
	update_env_array(shell);
	if (!shell->env_array)
	{
		free_shell(shell);
		system_error("update_env_array");
		return (1);
	}
	return (0);
}

/**
 * exportコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1
 */
int	builtin_export(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	/* 引数がない場合はすべての環境変数を表示 */
	if (!cmd->args[1])
	{
		print_sorted_env(shell->env_list, cmd->output_fd);
		return (0);
	}

	/* 各引数を処理 */
	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		if (add_export(cmd->args[i], shell) != 0)
			status = 1;
		i++;
	}
	
	return (status);
}