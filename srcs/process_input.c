/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:23:22 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/21 13:35:12 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_last_argument(t_command *commands)
{
	t_command	*cmd;
	char		**args;
	int			i;

	cmd = commands;
	if (!cmd)
		return (NULL);
	while (cmd && cmd->next)
		cmd = cmd->next;
	args = cmd->args;
	if (!args || !args[0])
		return (NULL);
	i = 0;
	while (args[i + 1])
		i++;
	return (ft_strdup(args[i]));
}

static void	set_last_arggument(t_shell *shell)
{
	char	*last_arg;

	last_arg = get_last_argument(shell->commands);
	if (last_arg)
	{
		add_env_node(&shell->env_list, "_", last_arg);
		update_env_array(shell);
		free(last_arg);
	}
}

int	process_input(char *input, t_shell *shell)
{
	if (!input || ft_strlen(input) == 0)
		return (SUCCESS);
	add_history(input);
	shell->tokens = tokenize(input, shell);
	if (shell->exit_status == NO_TOKEN)
	{
		shell->exit_status = SUCCESS;
		return (shell->exit_status);
	}
	if (shell->tokens == NULL)
	{
		shell->exit_status = ERROR;
		return (shell->exit_status);
	}
	if (parse(shell) == ERROR)
	{
		shell->exit_status = SYNTAX_ERROR;
		return (shell->exit_status);
	}
	shell->exit_status = execute_commands(shell);
	set_last_arggument(shell);
	return (shell->exit_status);
}
