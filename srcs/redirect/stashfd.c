/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stashfd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:15:29 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:16:51 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_fd(int fd)
{
	struct stat	st;

	if (fd < 0)
		return (false);
	errno = 0;
	if (fstat(fd, &st) < 0 && errno == EBADF)
		return (false);
	return (true);
}

int	stashfd(int fd, t_shell *shell)
{
	int	stashfd;

	if (!is_valid_fd(fd))
	{
		errno = EBADF;
		return (-1);
	}
	stashfd = 10;
	while (is_valid_fd(stashfd))
		stashfd++;
	stashfd = safe_dup2(fd, stashfd, shell);
	safe_close(fd, shell);
	return (stashfd);
}
