/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 12:02:25 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "../../includes/minishell.h"

/**
 * 文字列が数値かどうかをチェックする関数
 * @param str チェックする文字列
 * @return 数値であれば1、そうでなければ0
 */
static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	
	/* 最初の文字がプラスまたはマイナスの場合はスキップ */
	if (str[i] == '+' || str[i] == '-')
		i++;
	
	/* 少なくとも1つの数字が必要 */
	if (!str[i])
		return (0);
	
	/* 残りの文字はすべて数字でなければならない */
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	
	return (1);
}

/**
 * 文字列を数値(long)に変換する関数
 * @param str 変換する文字列
 * @param endptr 変換終了位置を格納するポインタ
 * @param base 基数
 * @return 変換結果
 */
static long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;

	/* 空白をスキップ */
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
			str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;

	/* 符号の処理 */
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}

	/* 数値の変換 */
	while (str[i] >= '0' && str[i] <= '9')
	{
		/* オーバーフローチェック */
		if (result > LONG_MAX / 10 ||
			(result == LONG_MAX / 10 && (str[i] - '0') > LONG_MAX % 10))
		{
			if (sign == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}

	return (result * sign);
}

/**
 * 数値を一つだけ持つexitコマンドを処理する関数
 * @param arg 引数
 * @param shell シェル構造体
 */
static void	handle_numeric_exit(char *arg, t_shell *shell)
{
	long	exit_code;
	int		i;
	int		is_valid;

	i = 0;
	is_valid = 1;

	/* 符号の処理 */
	if (arg[i] == '+' || arg[i] == '-')
		i++;

	/* 数字のみで構成されているかチェック */
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			is_valid = 0;
			break;
		}
		i++;
	}

	if (!is_valid)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		command_error("exit", ft_strjoin(arg, ": numeric argument required"));
		shell->running = 0;
		shell->exit_status = 255;
		return;
	}

	/* 文字列を数値に変換 */
	exit_code = ft_atol(arg);
	
	/* 正常に数値に変換できた場合 */
	ft_putstr_fd("exit\n", STDERR_FILENO);
	shell->running = 0;
	shell->exit_status = exit_code & 0xFF; /* 下位8ビットを使用 */
}

/**
 * exitコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1（実際には呼び出し元に戻らない）
 */
int	builtin_exit(t_command *cmd, t_shell *shell)
{
	/* 引数がない場合は直前のコマンドの終了ステータスで終了 */
	if (!cmd->args[1])
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		shell->running = 0;
		return (shell->exit_status);
	}

	/* 最初の引数が数値でない場合 */
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		command_error("exit", ft_strjoin(cmd->args[1], ": numeric argument required"));
		shell->running = 0;
		shell->exit_status = 255;
		return (255);
	}

	/* 引数が2つ以上ある場合 */
	if (cmd->args[2])
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		error_message("exit: too many arguments");
		return (1);
	}

	/* 数値を一つだけ持つexitコマンドを処理 */
	handle_numeric_exit(cmd->args[1], shell);
	
	return (shell->exit_status);
}