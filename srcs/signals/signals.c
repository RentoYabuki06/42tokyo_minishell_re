/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:52:21 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/18 14:33:33 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_status = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	int	point_pos;

	(void)sig;
	point_pos = rl_point;
	rl_on_new_line();
	rl_point = point_pos;
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ignore_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_IGN;
	sa_int.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	default_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = SIG_DFL;
	sa_int.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_DFL;
	sa_quit.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
