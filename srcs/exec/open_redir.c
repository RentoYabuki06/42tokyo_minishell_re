/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:53:41 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:28:55 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	openfd(t_node *node, t_shell *shell)
{
	if (node->type == ND_REDIR_OUT)
		return (open(node->filename->value, O_CREAT | O_WRONLY | O_TRUNC, 0644));
	else if (node->type == ND_REDIR_IN)
		return (open(node->filename->value, O_RDONLY));
	else if (node->type == ND_REDIR_APPEND)
		return (open(node->filename->value,
				O_CREAT | O_WRONLY | O_APPEND, 0644));
	else if (node->type == ND_REDIR_HEREDOC)
		return (read_heredoc(node->delimiter->value, node->is_delim_unquoted, shell));
	else
		assert_error("open_redir_file", shell);
}

bool	open_redir_file(t_node *node, t_shell *shell)
{
	if (node == NULL)
		return (EXIT_SUCCESS);
	if (node->type == ND_PIPELINE)
	{
		if (open_redir_file(node->command, shell) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (open_redir_file(node->next, shell) < 0)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else if (node->type == ND_SIMPLE_CMD)
		return (open_redir_file(node->redirects, shell));
	node->filefd = openfd(node, shell);
	if (node->filefd < 0)
	{
		if (node->type == ND_REDIR_OUT || node->type == ND_REDIR_APPEND
			|| node->type == ND_REDIR_IN)
			ft_perror(node->filename->value);
		return (EXIT_FAILURE);
	}
	node->filefd = stashfd(node->filefd, shell);
	return (open_redir_file(node->next, shell));
}
