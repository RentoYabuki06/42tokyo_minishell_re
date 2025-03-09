// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   exit.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
// /*   Updated: 2025/03/09 23:30:40 by myokono          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/minishell.h"

// /**
//  * 文字列が数値かどうかをチェックする関数
//  * @param str チェックする文字列
//  * @return 数値であれば1、そうでなければ0
//  */
// static int	is_numeric(char *str)
// {
// 	int	i;

// 	i = 0;
	
// 	/* 最初の文字がプラスまたはマイナスの場合はスキップ */
// 	if (str[i] == '+' || str[i] == '-')
// 		i++;
	
// 	/* 少なくとも1つの数字が必要 */
// 	if (!str[i])
// 		return (0);
	
// 	/* 残りの文字はすべて数字でなければならない */
// 	while (str[i])
// 	{
// 		if (!ft_isdigit(str[i]))
// 			return (0);
// 		i++;
// 	}
	
// 	return (1);
// }

// /**
//  * 文字列を数値(long)に変換する関数
//  * @param str 変換する文字列
//  * @param endptr 変換終了位置を格納するポインタ
//  * @param base 基数
//  * @return 変換結果
//  */
// static long	ft_atol(const char *str)
// {
// 	long	result;
// 	int		sign;
// 	int		i;

// 	result = 0;
// 	sign = 1;
// 	i = 0;

// 	/* 空白をスキップ */
// 	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
// 			str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
// 		i++;

// 	/* 符号の処理 */
// 	if (str[i] == '-' || str[i] == '+')
// 	{
// 		if (str[i] == '-')
// 			sign = -1;
// 		i++;
// 	}

// 	/* 数値の変換 */
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		/* オーバーフローチェック */
// 		if (result > LONG_MAX / 10 ||
// 			(result == LONG_MAX / 10 && (str[i] - '0') > LONG_MAX % 10))
// 		{
// 			if (sign == 1)
// 				return (LONG_MAX);
// 			else
// 				return (LONG_MIN);
// 		}
// 		result = result * 10 + (str[i] - '0');
// 		i++;
// 	}

// 	return (result * sign);
// }

// /**
//  * 数値を一つだけ持つexitコマンドを処理する関数
//  * @param arg 引数
//  * @param shell シェル構造体
//  */
// static void	handle_numeric_exit(char *arg, t_shell *shell)
// {
// 	long	exit_code;
// 	int		i;
// 	int		is_valid;

// 	i = 0;
// 	is_valid = 1;

// 	/* 符号の処理 */
// 	if (arg[i] == '+' || arg[i] == '-')
// 		i++;

// 	/* 数字のみで構成されているかチェック */
// 	while (arg[i])
// 	{
// 		if (!ft_isdigit(arg[i]))
// 		{
// 			is_valid = 0;
// 			break;
// 		}
// 		i++;
// 	}

// 	if (!is_valid)
// 	{
// 		ft_putstr_fd("exit\n", STDERR_FILENO);
// 		command_error("exit", ft_strjoin(arg, ": numeric argument required"));
// 		shell->running = 0;
// 		shell->exit_status = 255;
// 		return;
// 	}

// 	/* 文字列を数値に変換 */
// 	exit_code = ft_atol(arg);
	
// 	/* 正常に数値に変換できた場合 */
// 	ft_putstr_fd("exit\n", STDERR_FILENO);
// 	shell->running = 0;
// 	shell->exit_status = exit_code & 0xFF; /* 下位8ビットを使用 */
// }

// /**
//  * exitコマンドを実装する関数
//  * @param cmd コマンド構造体
//  * @param shell シェル構造体
//  * @return 成功すれば0、失敗すれば1（実際には呼び出し元に戻らない）
//  */
// int	builtin_exit(t_command *cmd, t_shell *shell)
// {
// 	/* 引数がない場合は直前のコマンドの終了ステータスで終了 */
// 	if (!cmd->args[1])
// 	{
// 		ft_putstr_fd("exit\n", STDERR_FILENO);
// 		shell->running = 0;
// 		return (shell->exit_status);
// 	}

// 	/* 最初の引数が数値でない場合 */
// 	if (!is_numeric(cmd->args[1]))
// 	{
// 		ft_putstr_fd("exit\n", STDERR_FILENO);
// 		command_error("exit", ft_strjoin(cmd->args[1], ": numeric argument required"));
// 		shell->running = 0;
// 		shell->exit_status = 255;
// 		return (255);
// 	}

// 	/* 引数が2つ以上ある場合 */
// 	if (cmd->args[2])
// 	{
// 		ft_putstr_fd("exit\n", STDERR_FILENO);
// 		error_message("exit: too many arguments");
// 		return (1);
// 	}

// 	/* 数値を一つだけ持つexitコマンドを処理 */
// 	handle_numeric_exit(cmd->args[1], shell);
	
// 	return (shell->exit_status);
// }



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/03/09 23:30:40 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 文字列が有効な数値かどうかをチェックする関数
 * Bashの動作に合わせて厳密にチェックする
 * @param str チェックする文字列
 * @return 有効な数値であれば1、そうでなければ0
 */
static int	is_valid_numeric(char *str)
{
	int	i;

	i = 0;
	
	/* 先頭の符号は一つだけ許可 */
	if (str[i] == '+' || str[i] == '-')
		i++;
	
	/* 複数の+/-や数字以外の文字があると無効 */
	if (!str[i])
		return (0);
		
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	
	return (1);
}

/**
 * 文字列を long long に変換する関数
 * オーバーフローも処理する
 * @param str 変換する文字列
 * @return 変換結果
 */
static long long	ft_atoll(const char *str)
{
	long long	result;
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
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;

	/* 数値の変換 */
	while (str[i] >= '0' && str[i] <= '9')
	{
		/* オーバーフローチェック */
		if (result > LLONG_MAX / 10 || 
			(result == LLONG_MAX / 10 && (str[i] - '0') > LLONG_MAX % 10))
		{
			/* オーバーフローの場合 */
			return (sign == 1 ? LLONG_MAX : LLONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}

	return (result * sign);
}

/**
 * exitコマンドを実装する関数
 * @param cmd コマンド構造体
 * @param shell シェル構造体
 * @return 成功すれば0、失敗すれば1（実際には呼び出し元に戻らない）
 */
int	builtin_exit(t_command *cmd, t_shell *shell)
{
	long long	exit_code;

	/* 常に "exit" を表示 */
	ft_putstr_fd("exit\n", STDERR_FILENO);
	
	/* 引数がない場合は直前のコマンドの終了ステータスで終了 */
	if (cmd->args[1] == NULL)
	{
		shell->running = 0;
		return (shell->exit_status);
	}

	/* 最初の引数が有効な数値でない場合 - 255で終了 */
	if (!is_valid_numeric(cmd->args[1]))
	{
		command_error("exit", ft_strjoin(cmd->args[1], ": numeric argument required"));
		shell->running = 0;
		shell->exit_status = 255;
		return (255);
	}

	/* 有効な数値の場合 */
	exit_code = ft_atoll(cmd->args[1]);
	
	/* 引数が2つ以上ある場合はエラーを表示してシェルを継続 */
	if (cmd->args[2] != NULL)
	{
		error_message("exit: too many arguments");
		return (1);
	}

	/* 正常に終了する場合 - 下位8ビットを使用 */
	shell->running = 0;
	shell->exit_status = (unsigned char)exit_code;
	return (shell->exit_status);
}
