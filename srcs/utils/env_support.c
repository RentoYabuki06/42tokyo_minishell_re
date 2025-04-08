/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_support.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:26:06 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/08 13:26:14 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env_array(t_shell *shell)
{
	if (shell->env_array)
		free_array(shell->env_array);
	shell->env_array = env_list_to_array(shell->env_list);
}

t_env	*get_env_node(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*node;

	node = get_env_node(env_list, key);
	if (!node)
		return (NULL);
	return (node->value);
}
