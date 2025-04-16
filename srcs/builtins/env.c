/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:35:26 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/16 17:51:01 by yabukirento      ###   ########.fr       */
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
		ft_fprintf2(STDOUT_FILENO, "%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (SUCCESS);
}
