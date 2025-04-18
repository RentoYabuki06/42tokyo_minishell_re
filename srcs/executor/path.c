/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:30:51 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 21:56:50 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_paths_from_env(t_env *env_list)
{
	char	*path_env;
	char	**paths;

	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}

static char	*is_direct_path(char *cmd)
{
	if (ft_strchr(cmd, '/') != NULL)
	{
		return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*exec_path;

	i = 0;
	while (paths[i])
	{
		if (ft_strlen(paths[i]) != 0)
			tmp = ft_strjoin(paths[i], "/");
		else
			tmp = ft_strdup(paths[i]);
		if (tmp == NULL)
			return (NULL);
		exec_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (exec_path == NULL)
			return (NULL);
		if (access(exec_path, X_OK) == 0)
			return (exec_path);
		free(exec_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*result;

	result = is_direct_path(cmd);
	if (result)
		return (result);
	paths = get_paths_from_env(env_list);
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, cmd);
	free_array(paths);
	return (result);
}
