/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/05 11:07:58 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_env(t_command *cmd, t_shell *shell)
{
	t_env	*current;

	if (cmd->args[1])
	{
		error_message("env: too many arguments");
		return (1);
	}
	current = shell->env_list;
	while (current)
	{
		if (current->value && *current->value)
		{
			ft_putstr_fd(current->key, cmd->output_fd);
			ft_putstr_fd("=", cmd->output_fd);
			ft_putstr_fd(current->value, cmd->output_fd);
			ft_putstr_fd("\n", cmd->output_fd);
		}
		current = current->next;
	}
	return (0);
}
