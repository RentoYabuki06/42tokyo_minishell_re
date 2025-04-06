/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:10:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/06 19:58:28 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_last_command_path(t_shell *shell)
{
	// 通常は $_ 環境変数が設定されているはず
	char	*path;

	path = get_env_value(shell->env_list, "_");
	if (path)
		return (path);
	return ("/bin/bash");
}

static int	is_delimiter(char c)
{
	return (is_space(c) || c == '|' || c == '<' || c == '>' || c == '\0');
}

/**
 * 環境変数を展開する関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param result 結果文字列
 * @param shell シェル構造体
 * @return 処理に成功すれば0、失敗すれば-1
 */
int	expand_env_var(char *input, int *i, char **result, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*var_value;

	(*i)++; // $ をスキップ
	// $? の処理
	if (input[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_status);
		*result = ft_strjoin_free(*result, var_value);
		(*i)++;
		return (0);
	}
	// $' または $" の処理
	if (input[*i] == '\'' && input[*i - 1] == '$')
	{
		// すでに $ をスキップしているので i-1 を渡す
		(*i)--; // $ に戻る
		return (handle_dollar_single_quote(input, i, result));
	}
	else if (input[*i] == '\"' && input[*i - 1] == '$')
	{
		// すでに $ をスキップしているので i-1 を渡す
		(*i)--; // $ に戻る
		return (handle_dollar_double_quote(input, i, result, shell));
	}
	// クォートで囲まれた $ の処理
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		// 空のクォートまたは別のトークンに囲まれた $ は展開しない
		*result = ft_strjoin_free(*result, ft_strdup("$"));
		return (0);
	}
	// 通常の環境変数の処理
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	// 変数名がない場合 ($ のみ)
	if (start == *i)
	{
		*result = ft_strjoin_free(*result, ft_strdup("$"));
		return (0);
	}
	// 変数名を取得して展開
	var_name = ft_substr(input, start, *i - start);
	var_value = get_env_value(shell->env_list, var_name);
	// シェル特殊変数の処理（例: $_）
	if (ft_strcmp(var_name, "_") == 0 && !var_value)
		var_value = get_last_command_path(shell);
	if (var_value)
		*result = ft_strjoin_free(*result, ft_strdup(var_value));
	free(var_name);
	return (SUCCESS);
}


static int	handle_word(char *input, int *i, char **result)
{
	int		start;
	char	*segment;

	start = *i;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"' || input[*i] == '$')
			break ;
		(*i)++;
	}
	segment = ft_substr(input, start, *i - start);
	*result = ft_strjoin(*result, segment);
	free(segment);
	if (!*result)
	{
		error_message("Memory allocation error");
		return (ERROR);
	}
	return (SUCCESS);
}


static int	handle_word_token(char *input, int *i, t_token **tokens, t_shell *shell)
{
	char	*result;
	int		has_content;
	int		start_pos;

	result = ft_strdup("");
	has_content = 0;
	start_pos = *i;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '$' && input[*i + 1] && \
			(input[*i + 1] == '\'' || input[*i + 1] == '\"'))
		{
			if (input[*i + 1] == '\'')
			{
				if (handle_dollar_single_quote(input, i, &result) == ERROR)
					return (free(result), ERROR);
			}
			else
			{
				if (handle_dollar_double_quote(input, i, &result, shell) \
					== ERROR)
					return (free(result), ERROR);
			}
			has_content = 1;
		}
		else if (input[*i] == '\'')
		{
			if (handle_single_quote(input, i, &result) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
		else if (input[*i] == '\"')
		{
			if (handle_double_quote(input, i, &result, shell) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
		else if (input[*i] == '$')
		{
			if (expand_env_var(input, i, &result, shell) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
		else
		{
			if (handle_word(input, i, &result) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
	}
	if (has_content || ft_strlen(result) > 0 || (*i > start_pos))
		add_token(tokens, create_token(TOKEN_WORD, result));
	else
		free(result);
	return (SUCCESS);
}
