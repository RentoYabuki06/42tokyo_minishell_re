/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:09:14 by myokono           #+#    #+#             */
/*   Updated: 2025/04/21 13:34:49 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_export_without_equal(char *arg, t_shell *shell)
{
	char	*key;
	char	*msg;

	key = ft_strdup(arg);
	if (!is_valid_identifier(key))
	{
		msg = ft_strjoin(key, ": not a valid identifier");
		if (msg == NULL)
			return (ERROR);
		print_error("export", msg);
		free(msg);
		free(key);
		return (ERROR);
	}
	if (!get_env_node(shell->env_list, key))
		add_env_node(&shell->env_list, key, "");
	free(key);
	return (SUCCESS);
}

static int	export_with_equal(char *arg, char *equals_pos, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*msg;

	key = ft_substr(arg, 0, equals_pos - arg);
	value = ft_strdup(equals_pos + 1);
	if (!is_valid_identifier(key))
	{
		msg = ft_strjoin(arg, ": not a valid identifier");
		if (msg == NULL)
			return (ERROR);
		print_error("export", msg);
		free(msg);
		free(key);
		free(value);
		return (ERROR);
	}
	add_env_node(&shell->env_list, key, value);
	free(key);
	free(value);
	return (SUCCESS);
}

static int	add_export(char *arg, t_shell *shell)
{
	char	*equals_pos;
	int		status;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
		status = handle_export_without_equal(arg, shell);
	else
	{
		if (equals_pos > arg && *(equals_pos - 1) == '+')
			status = export_with_plus_equal(arg, equals_pos, shell);
		else
			status = export_with_equal(arg, equals_pos, shell);
	}
	update_env_array(shell);
	if (!shell->env_array)
		return (system_error("update_env_array"), ERROR);
	return (status);
}

int	builtin_export(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	if (!cmd->args[1])
	{
		print_sorted_env(shell->env_list, STDOUT_FILENO);
		return (0);
	}
	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		if (add_export(cmd->args[i], shell) != 0)
			status = 1;
		i++;
	}
	return (status);
}
