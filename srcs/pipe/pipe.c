/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:23:15 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 18:32:11 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_pipe(t_node *node, t_shell *shell)
{
	if (node->next == NULL)
		return ;
	safe_pipe(node->outpipe, shell);
	cpy_pipe(node->next->inpipe, node->outpipe);
}

void	prepare_pipe_child(t_node *node, t_shell *shell)
{
	safe_close(node->outpipe[0], shell);
	safe_dup2(node->inpipe[0], STDIN_FILENO, shell);
	if (node->inpipe[0] != STDIN_FILENO)
		safe_close(node->inpipe[0], shell);
	safe_dup2(node->outpipe[1], STDOUT_FILENO, shell);
	if (node->outpipe[1] != STDOUT_FILENO)
		safe_close(node->outpipe[1], shell);
}

void	prepare_pipe_parent(t_node *node, t_shell *shell)
{
	if (node->inpipe[0] != STDIN_FILENO)
		safe_close(node->inpipe[0], shell);
	if (node->next)
		safe_close(node->outpipe[1], shell);
}

void	cpy_pipe(int dst[2], int src[2])
{
	dst[0] = src[0];
	dst[1] = src[1];
}
