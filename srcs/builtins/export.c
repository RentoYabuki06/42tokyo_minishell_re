/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/07 02:07:55 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	print_sorted_env(t_env *env_list, int fd)
{
	t_env	**env_array;
	t_env	*current;
	int		count;
	int		i;
	int		j;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = (t_env **)malloc(sizeof(t_env *) * count);
	if (!env_array)
	{
		system_error("malloc");
		return (ERROR);
	}
	current = env_list;
	i = 0;
	while (current)
	{
		env_array[i++] = current;
		current = current->next;
	}
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_array[j]->key, env_array[j + 1]->key) > 0)
			{
				current = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = current;
			}
			j++;
		}
		i++;
	}

	/* ソートされた環境変数を表示 */
	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(env_array[i]->key, fd);
		ft_putstr_fd("=\"", fd);
		ft_putstr_fd(env_array[i]->value, fd);
		ft_putstr_fd("\"\n", fd);
		i++;
	}

	free(env_array);
	return (SUCCESS);
}

static int	add_export(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
	{
		key = ft_strdup(arg);
		if (!is_valid_identifier(key))
		{
			command_error("export", ft_strjoin(key, ": not a valid identifier"));
			free(key);
			return (ERROR);
		}
		if (!get_env_node(shell->env_list, key))
			add_env_node(&shell->env_list, key, "");
		
		free(key);
	}
	else
	{
		key = ft_substr(arg, 0, equals_pos - arg);
		value = ft_strdup(equals_pos + 1);
		if (!is_valid_identifier(key))
		{
			command_error("export", ft_strjoin(key, ": not a valid identifier"));
			free(key);
			free(value);
			return (ERROR);
		}
		add_env_node(&shell->env_list, key, value);
		free(key);
		free(value);
	}
	update_env_array(shell);
	if (!shell->env_array)
	{
		free_shell(shell);
		system_error("update_env_array");
		return (ERROR);
	}
	return (SUCCESS);
}

int	builtin_export(t_command *cmd, t_shell *shell)
{
	int	i;
	int	status;

	if (!cmd->args[1])
	{
		print_sorted_env(shell->env_list, cmd->output_fd);
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
