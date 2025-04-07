/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:05:30 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 18:05:46 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(t_node *node)
{
	const char			*cmd_name;
	static const char	*builtin_commands[] = {"exit", "export", "unset", "env",
		"cd", "echo", "pwd"};
	unsigned int		i;

	if (node == NULL || node->command == NULL | node->command->args == NULL
		|| node->command->args->value == NULL)
		return (false);
	cmd_name = node->command->args->value;
	i = 0;
	while (i < sizeof(builtin_commands) / sizeof(*builtin_commands))
	{
		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}
