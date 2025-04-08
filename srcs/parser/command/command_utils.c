/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:00 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/08 14:38:13 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	add_arg(t_command *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		size;

	size = 0;
	if (cmd->args)
	{
		while (cmd->args[size])
			size++;
	}
	new_args = malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return (error_message("Memory allocation error"), ERROR);
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[size] = ft_strdup(arg);
	new_args[size + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	return (SUCCESS);
}

int	handle_pipe(t_token **tokens, t_command *cmd, t_command **commands)
{
	if (!cmd->args)
	{
		error_message("Syntax error near unexpected token `|'");
		return (ERROR);
	}
	add_command(commands, cmd);
	*tokens = (*tokens)->next;
	return (SUCCESS);
}
