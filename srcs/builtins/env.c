/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:35:26 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/13 20:53:14 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_command *cmd, t_shell *shell)
{
	t_env	*current;

	if (cmd->args[1])
	{
		error_message("env: too many arguments");
		return (ERROR);
	}
	current = shell->env_list;
	while (current)
	{
		ft_putstr_fd(current->key, STDOUT_FILENO);
		ft_putstr_fd("=", STDOUT_FILENO);
		ft_putstr_fd(current->value, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		current = current->next;
	}
	return (SUCCESS);
}
