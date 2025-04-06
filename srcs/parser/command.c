/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:29:11 by myokono           #+#    #+#             */
/*   Updated: 2025/04/06 21:30:14 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **commands, t_command *new_command)
{
	t_command	*current;

	if (!*commands)
	{
		*commands = new_command;
		return ;
	}
	current = *commands;
	while (current->next)
		current = current->next;
	current->next = new_command;
}
