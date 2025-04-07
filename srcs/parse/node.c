/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:31:23 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:07:34 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*new_node(t_node_type type, t_shell *shell)
{
	t_node	*node;

	node = safe_malloc(sizeof(t_node), shell);
	node->type = type;
	return (node);
}

void	append_node(t_node **node, t_node *elm)
{
	if (*node == NULL)
	{
		*node = elm;
		return ;
	}
	append_node(&(*node)->next, elm);
}
