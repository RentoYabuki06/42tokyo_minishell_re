/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:03:37 by myokono           #+#    #+#             */
/*   Updated: 2025/04/21 13:38:14 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		system_error("malloc");
		return (NULL);
	}
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

int	add_env_node(t_env **env_list, char *key, char *value)
{
	t_env	*existing_node;
	t_env	*new_node;
	t_env	*current;

	existing_node = get_env_node(*env_list, key);
	if (existing_node)
	{
		free(existing_node->value);
		existing_node->value = ft_strdup(value);
		if (!existing_node->value)
			return (ERROR);
		return (SUCCESS);
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return (ERROR);
	if (!*env_list)
		return ((*env_list = new_node), SUCCESS);
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (SUCCESS);
}

void	remove_head_env_node(t_env **env_list, char *key)
{
	t_env	*current;

	if (!*env_list)
		return ;
	if (ft_strcmp((*env_list)->key, key) == 0)
	{
		current = *env_list;
		*env_list = (*env_list)->next;
		free(current->key);
		free(current->value);
		free(current);
	}
}

void	remove_env_node(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!*env_list)
		return ;
	if (ft_strcmp((*env_list)->key, key) == 0)
	{
		remove_head_env_node(env_list, key);
		return ;
	}
	prev = *env_list;
	current = (*env_list)->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
