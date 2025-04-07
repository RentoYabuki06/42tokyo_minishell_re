/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:14:31 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:42:11 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirect(t_node *node)
{
	if (node->type == ND_REDIR_OUT)
		return (true);
	else if (node->type == ND_REDIR_IN)
		return (true);
	else if (node->type == ND_REDIR_APPEND)
		return (true);
	else if (node->type == ND_REDIR_HEREDOC)
		return (true);
	return (false);
}

void	do_redirect(t_node *redir, t_shell *shell)
{
	if (redir == NULL)
		return ;
	if (is_redirect(redir))
	{
		redir->stashed_targetfd = stashfd(redir->targetfd, shell);
		safe_dup2(redir->filefd, redir->targetfd, shell);
	}
	else
		assert_error("do_redirect", shell);
	do_redirect(redir->next, shell);
}

// Reset must be done from tail to head
void	reset_redirect(t_node *redir, t_shell *shell)
{
	if (redir == NULL)
		return ;
	reset_redirect(redir->next, shell);
	if (is_redirect(redir))
	{
		safe_close(redir->filefd, shell);
		safe_close(redir->targetfd, shell);
		safe_dup2(redir->stashed_targetfd, redir->targetfd, shell);
	}
	else
		assert_error("reset_redirect", shell);
}
