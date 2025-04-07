/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:03:37 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 19:07:40 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env_array(t_shell *shell)
{
	if (shell->env_array)
		free_argv(shell->env_array);
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

void	add_env_node(t_env **env_list, char *key, char *value)
{
	t_env	*existing_node;
	t_env	*new_node;
	t_env	*current;

	existing_node = get_env_node(*env_list, key);
	if (existing_node)
	{
		free(existing_node->value);
		existing_node->value = ft_strdup(value);
		return ;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		system_error("malloc");
		return ;
	}
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void	remove_env_node(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!*env_list)
		return ;

	if (ft_strcmp((*env_list)->key, key) == 0)
	{
		current = *env_list;
		*env_list = (*env_list)->next;
		free(current->key);
		free(current->value);
		free(current);
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
