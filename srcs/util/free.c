/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:49:38 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:07:59 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_node(t_node *node)
{
	if (node == NULL)
		return ;
	free_token(node->args);
	free_token(node->filename);
	free_token(node->delimiter);
	free_node(node->redirects);
	free_node(node->next);
	free_node(node->command);
	free(node);
}

void	free_token(t_token *tok)
{
	if (tok == NULL)
		return ;
	if (tok->value)
		free(tok->value);
	free_token(tok->next);
	free(tok);
}

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_shell(t_shell *shell)
{
	free_node(shell->node);
	free_token(shell->token);
	free_env_list(shell->env_list);
	free_argv(shell->env_array);
}
