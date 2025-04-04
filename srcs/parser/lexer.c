/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:35:52 by myokono           #+#    #+#             */
/*   Updated: 2025/03/09 23:21:40 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * 文字列連結を行い、古い文字列を解放する関数
 * @param s1 元の文字列（解放される）
 * @param s2 連結する文字列
 * @return 連結された新しい文字列
 */
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

/**
 * 空白文字かどうかを判定する関数
 * @param c 判定する文字
 * @return 空白文字であれば1、そうでなければ0
 */
static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

/**
 * トークンの区切り文字かどうかを判定する関数
 * @param c 判定する文字
 * @return 区切り文字であれば1、そうでなければ0
 */
static int	is_delimiter(char c)
{
	return (is_space(c) || c == '|' || c == '<' || c == '>' || c == '\0');
}

/**
 * 環境変数名の有効な文字かどうかを判定する関数
 * @param c 判定する文字
 * @return 有効な文字であれば1、そうでなければ0
 */
static int	is_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * 最後に実行されたコマンドのパスを取得する
 * @param shell シェル構造体
 * @return コマンドのパス文字列
 */
static char	*get_last_command_path(t_shell *shell)
{
	// 通常は $_ 環境変数が設定されているはず
	char	*path;

	path = get_env_value(shell->env_list, "_");
	if (path)
		return (path);
	return ("/bin/bash"); // デフォルト値
}

/**
 * C言語のエスケープシーケンスを解釈する
 * @param str 解釈するエスケープシーケンスを含む文字列
 * @return 解釈された新しい文字列
 */
static char	*interpret_escapes(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			if (str[i] == 'n')
				result[j++] = '\n';
			else if (str[i] == 't')
				result[j++] = '\t';
			else if (str[i] == 'r')
				result[j++] = '\r';
			else if (str[i] == 'a')
				result[j++] = '\a';
			else if (str[i] == 'b')
				result[j++] = '\b';
			else if (str[i] == 'f')
				result[j++] = '\f';
			else if (str[i] == 'v')
				result[j++] = '\v';
			else if (str[i] == '\\' || str[i] == '\'' || str[i] == '\"')
				result[j++] = str[i];
			else
				result[j++] = str[i];
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	free(str);
	return (result);
}

/**
 * スペシャルトークン（パイプ、リダイレクト）を処理する関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param tokens トークンリストへのポインタ
 * @return 処理に成功すれば0、失敗すれば-1
 */
static int	handle_special_token(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
	{
		add_token(tokens, create_token(TOKEN_PIPE, ft_strdup("|")));
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, create_token(TOKEN_HEREDOC, ft_strdup("<<")));
		(*i) += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, create_token(TOKEN_APPEND, ft_strdup(">>")));
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, create_token(TOKEN_REDIRECT_IN, ft_strdup("<")));
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, create_token(TOKEN_REDIRECT_OUT, ft_strdup(">")));
		(*i)++;
	}
	return (0);
}

/**
 * 特殊クォート形式 $'' の処理を行う関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param result 結果文字列
 * @return 処理に成功すれば0、失敗すれば-1
 */
static int	handle_dollar_single_quote(char *input, int *i, char **result)
{
	int		start;
	char	*content;

	(*i) += 2; // $' をスキップ
	start = *i;
	
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	
	if (!input[*i])
	{
		error_message("Syntax error: unclosed quote in $'...'");
		return (-1);
	}
	
	// エスケープシーケンスを解釈
	content = ft_substr(input, start, *i - start);
	content = interpret_escapes(content);
	*result = ft_strjoin_free(*result, content);
	(*i)++;
	return (0);
}

/**
 * 環境変数を展開する関数の前方宣言
 */
static int	expand_env_var(char *input, int *i, char **result, t_shell *shell);

/**
 * ダブルクォート内のテキストを処理する関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param result 結果文字列
 * @param shell シェル構造体
 * @return 処理に成功すれば0、失敗すれば-1
 */
static int	handle_double_quote(char *input, int *i, char **result, t_shell *shell)
{
	int		start;
	char	*segment;

	(*i)++; // " をスキップ
	
	// 空のダブルクォート "" の処理
	if (input[*i] == '\"')
	{
		(*i)++;
		return (0); // 空の文字列を追加しない
	}
	
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$')
		{
			// 特殊ケース: "$" だけの場合
			if (input[*i + 1] == '\"')
			{
				*result = ft_strjoin_free(*result, ft_strdup("$"));
				(*i)++;
			}
			else if (expand_env_var(input, i, result, shell) == -1)
				return (-1);
		}
		else
		{
			start = *i;
			while (input[*i] && input[*i] != '\"' && input[*i] != '$')
				(*i)++;
			segment = ft_substr(input, start, *i - start);
			*result = ft_strjoin_free(*result, segment);
		}
	}
	
	if (!input[*i])
	{
		error_message("Syntax error: unclosed double quote");
		return (-1);
	}
	
	(*i)++;
	return (0);
}

/**
 * 特殊クォート形式 $"" の処理を行う関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param result 結果文字列
 * @param shell シェル構造体
 * @return 処理に成功すれば0、失敗すれば-1
 */
static int	handle_dollar_double_quote(char *input, int *i, char **result, t_shell *shell)
{
	// $"" の中身は通常の "" と同様に処理するが、
	// 本来はローカライゼーションを適用する
	(*i)++; // $ をスキップして " から始める
	return (handle_double_quote(input, i, result, shell));
}

/**
 * 単一クォート内のテキストを処理する関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param result 結果文字列
 * @return 処理に成功すれば0、失敗すれば-1
 */
static int	handle_single_quote(char *input, int *i, char **result)
{
	int	start;

	(*i)++; // ' をスキップ
	
	// 空のシングルクォート '' の処理
	if (input[*i] == '\'')
	{
		(*i)++;
		return (0); // 空の文字列を追加しない
	}
	
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	
	if (!input[*i])
	{
		error_message("Syntax error: unclosed single quote");
		return (-1);
	}
	
	*result = ft_strjoin_free(*result, ft_substr(input, start, *i - start));
	(*i)++;
	return (0);
}

/**
 * 環境変数を展開する関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param result 結果文字列
 * @param shell シェル構造体
 * @return 処理に成功すれば0、失敗すれば-1
 */
static int	expand_env_var(char *input, int *i, char **result, t_shell *shell)
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
	while (input[*i] && is_env_char(input[*i]))
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
	{
		// $_ は最後に実行されたコマンドのパス
		var_value = get_last_command_path(shell);
	}
	
	if (var_value)
		*result = ft_strjoin_free(*result, ft_strdup(var_value));
	
	free(var_name);
	return (0);
}

/**
 * 通常のワードを処理する関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param result 結果文字列
 * @return 処理に成功すれば0、失敗すれば-1
 */
static int	handle_word(char *input, int *i, char **result)
{
	int		start;
	char	*segment;

	start = *i;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"' || input[*i] == '$')
			break;
		(*i)++;
	}
	
	segment = ft_substr(input, start, *i - start);
	*result = ft_strjoin_free(*result, segment);
	return (0);
}

/**
 * ワードトークンを処理する関数
 * @param input 入力文字列
 * @param i 現在の位置へのポインタ
 * @param tokens トークンリストへのポインタ
 * @param shell シェル構造体
 * @return 処理に成功すれば0、失敗すれば-1
 */
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
		if (input[*i] == '$' && input[*i + 1] && (input[*i + 1] == '\'' || input[*i + 1] == '\"'))
		{
			// 特殊クォート $'' や $"" の処理
			if (input[*i + 1] == '\'')
			{
				if (handle_dollar_single_quote(input, i, &result) == -1)
				{
					free(result);
					return (-1);
				}
			}
			else // input[*i + 1] == '\"'
			{
				if (handle_dollar_double_quote(input, i, &result, shell) == -1)
				{
					free(result);
					return (-1);
				}
			}
			has_content = 1;
		}
		else if (input[*i] == '\'')
		{
			if (handle_single_quote(input, i, &result) == -1)
			{
				free(result);
				return (-1);
			}
			has_content = 1;
		}
		else if (input[*i] == '\"')
		{
			if (handle_double_quote(input, i, &result, shell) == -1)
			{
				free(result);
				return (-1);
			}
			has_content = 1;
		}
		else if (input[*i] == '$')
		{
			if (expand_env_var(input, i, &result, shell) == -1)
			{
				free(result);
				return (-1);
			}
			has_content = 1;
		}
		else
		{
			if (handle_word(input, i, &result) == -1)
			{
				free(result);
				return (-1);
			}
			has_content = 1;
		}
	}
	
	// 内容があるか、または引用符が使われた場合のみトークンを追加
	if (has_content || ft_strlen(result) > 0 || (*i > start_pos))
		add_token(tokens, create_token(TOKEN_WORD, result));
	else
		free(result);
	
	return (0);
}

/**
 * 入力文字列をトークン化する関数
 * @param input 入力文字列
 * @param shell シェル構造体
 * @return トークンリスト
 */
t_token	*tokenize(char *input, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (handle_special_token(input, &i, &tokens) == -1)
			{
				free_tokens(tokens);
				return (NULL);
			}
		}
		else
		{
			if (handle_word_token(input, &i, &tokens, shell) == -1)
			{
				free_tokens(tokens);
				return (NULL);
			}
		}
	}
	return (tokens);
}
