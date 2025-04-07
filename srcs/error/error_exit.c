/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:33:47 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:12:12 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fatal_error(char *msg, t_shell *shell)
{
	free_shell(shell);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("Fatal Error: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	perror(NULL);
	exit(EXIT_FAILURE);
}

void	assert_error(char *msg, t_shell *shell)
{
	free_shell(shell);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("Assert Error: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(ERROR_ASSERT);
}

void	error_exit(char *location, char *msg, int status, t_shell *shell)
{
	status = shell->exit_status;
	free_shell(shell);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(location, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(status);
}
