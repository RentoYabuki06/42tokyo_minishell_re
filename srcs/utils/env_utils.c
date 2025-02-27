/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:03:37 by myokono           #+#    #+#             */
/*   Updated: 2025/02/27 13:03:56 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 環境変数の文字列からキーと値を分離する関数
 * @param env_str 環境変数文字列（形式: KEY=VALUE）
 * @param key 分離されたキーの保存先
 * @param value 分離された値の保存先
 */
static void	split_env_str(char *env_str, char **key, char **value)
{
	char	*equals_pos;

	equals_pos = ft_strchr(env_str, '=');
	if (!equals_pos)
	{
		*key = ft_strdup(env_str);
		*value = ft_strdup("");
		return ;
	}
	*key = ft_substr(env_str, 0, equals_pos - env_str);
	*value = ft_strdup(equals_pos + 1);
}

/**
 * 新しい環境変数ノードを作成する関数
 * @param env_str 環境変数文字列
 * @return 作成された環境変数ノード
 */
static t_env	*create_env_node(char *env_str)
{
	t_env	*new_node;
	char	*key;
	char	*value;

	split_env_str(env_str, &key, &value);
	new_node = safe_malloc(sizeof(t_env));
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

/**
 * 環境変数リストを初期化する関数
 * @param envp 環境変数配列
 * @return 初期化された環境変数リスト
 */
t_env	*init_env_list(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	t_env	*last_node;
	int		i;

	env_list = NULL;
	last_node = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!env_list)
			env_list = new_node;
		else
			last_node->next = new_node;
		last_node = new_node;
		i++;
	}
	return (env_list);
}

/**
 * 環境変数リストから配列を生成する関数
 * @param env_list 環境変数リスト
 * @return 環境変数配列（NULL終端）
 */
char	**env_list_to_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;
	char	*tmp;

	/* リストの長さを数える */
	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}

	/* 配列を割り当て */
	env_array = safe_malloc(sizeof(char *) * (count + 1));
	
	/* 各ノードを配列に変換 */
	i = 0;
	current = env_list;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	
	return (env_array);
}

/**
 * 環境変数リストを更新した後に環境変数配列も更新する関数
 * @param shell シェル構造体
 */
void	update_env_array(t_shell *shell)
{
	if (shell->env_array)
		free_array(shell->env_array);
	shell->env_array = env_list_to_array(shell->env_list);
}

/**
 * 指定されたキーの環境変数ノードを取得する関数
 * @param env_list 環境変数リスト
 * @param key 検索するキー
 * @return 見つかったノード（見つからない場合はNULL）
 */
t_env	*get_env_node(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * 環境変数の値を取得する関数
 * @param env_list 環境変数リスト
 * @param key 検索するキー
 * @return 環境変数の値（見つからない場合はNULL）
 */
char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*node;

	node = get_env_node(env_list, key);
	if (!node)
		return (NULL);
	return (node->value);
}

/**
 * 環境変数を追加または更新する関数
 * @param env_list 環境変数リストへのポインタ
 * @param key 追加/更新するキー
 * @param value 設定する値
 */
void	add_env_node(t_env **env_list, char *key, char *value)
{
	t_env	*existing_node;
	t_env	*new_node;
	t_env	*current;

	/* 既存のノードを検索 */
	existing_node = get_env_node(*env_list, key);
	if (existing_node)
	{
		/* 既存のノードを更新 */
		free(existing_node->value);
		existing_node->value = ft_strdup(value);
		return ;
	}

	/* 新しいノードを作成 */
	new_node = safe_malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;

	/* リストの末尾に追加 */
	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

/**
 * 環境変数を削除する関数
 * @param env_list 環境変数リストへのポインタ
 * @param key 削除するキー
 */
void	remove_env_node(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!*env_list)
		return ;

	/* 先頭ノードの場合 */
	if (ft_strcmp((*env_list)->key, key) == 0)
	{
		current = *env_list;
		*env_list = (*env_list)->next;
		free(current->key);
		free(current->value);
		free(current);
		return ;
	}

	/* 他のノードを検索 */
	prev = *env_list;
	current = (*env_list)->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}