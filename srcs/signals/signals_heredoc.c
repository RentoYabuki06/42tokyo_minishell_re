/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:02:35 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/12 16:51:52 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_signal_status = -1;
	write(STDOUT_FILENO, "\n", 1);
}

static void	sigquit_heredoc_handler(int sig)
{
	(void)sig;
	g_signal_status = -1;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_signal_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_heredoc_handler;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_heredoc_handler;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
