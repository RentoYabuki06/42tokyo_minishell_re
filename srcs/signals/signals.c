/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 12:22:53 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * SIGINT (Ctrl+C) シグナルハンドラ
 * @param sig シグナル番号
 */
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_status = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * SIGQUIT (Ctrl+\) シグナルハンドラ
 * @param sig シグナル番号
 */
void	handle_sigquit(int sig)
{
	(void)sig;
	/* 要件に従って何もしない */
}

/**
 * 子プロセスのシグナルハンドラ設定
 */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * シグナルハンドラの設定
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	/* SIGINT (Ctrl+C) ハンドラの設定 */
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	/* SIGQUIT (Ctrl+\) ハンドラの設定 */
	sa_quit.sa_handler = handle_sigquit;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
