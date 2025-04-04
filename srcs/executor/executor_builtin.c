/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:08:02 by myokono           #+#    #+#             */
/*   Updated: 2025/04/04 18:11:08 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	char	*command;
	int		status;

	command = cmd->args[0];
	status = 0;
	if (ft_strcmp(command, "echo") == 0)
		status = builtin_echo(cmd, shell);
	else if (ft_strcmp(command, "cd") == 0)
		status = builtin_cd(cmd, shell);
	else if (ft_strcmp(command, "pwd") == 0)
		status = builtin_pwd(cmd, shell);
	else if (ft_strcmp(command, "export") == 0)
		status = builtin_export(cmd, shell);
	else if (ft_strcmp(command, "unset") == 0)
		status = builtin_unset(cmd, shell);
	else if (ft_strcmp(command, "env") == 0)
		status = builtin_env(cmd, shell);
	else if (ft_strcmp(command, "exit") == 0)
		status = builtin_exit(cmd, shell);
	return (status);
}

char	*find_executable(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*exec_path;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		exec_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
		if (access(exec_path, X_OK) == 0)
		{
			free_array(paths);
			return (exec_path);
		}
		free(exec_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}
