/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:04:51 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:57:14 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	safe_close(int fd, t_shell *shell)
{
	if (close(fd) < 0)
		fatal_error("close failed", shell);
	return (EXIT_SUCCESS);
}

int	safe_dup(int fd, t_shell *shell)
{
	int	new_fd;

	new_fd = dup(fd);
	if (new_fd < 0)
		fatal_error("dup failed", shell);
	return (new_fd);
}

int	safe_dup2(int from_fd, int to_fd, t_shell *shell)
{
	int	fd;

	fd = dup2(from_fd, to_fd);
	if (fd < 0)
		fatal_error("dup2 failed", shell);
	return (fd);
}

int	safe_pipe(int fd[2], t_shell *shell)
{
	int	ret;

	ret = pipe(fd);
	if (ret < 0)
		fatal_error("pipe failed", shell);
	return (ret);
}
