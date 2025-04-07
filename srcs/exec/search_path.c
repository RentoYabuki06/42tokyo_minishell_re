/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:33:04 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 18:37:46 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_path(char *path, size_t pathsize,
	const char *filename, char **envpath)
{
	size_t	len;
	char	buf[PATH_MAX];
	char	*end;

	ft_bzero(path, pathsize);
	end = ft_strchr(*envpath, ':');
	if (*envpath == end)
		len = ft_strlcpy(path, ".", pathsize);
	else if (end)
	{
		len = ft_strlcpy(buf, *envpath, end - *envpath + 1);
		ft_strlcpy(path, buf, pathsize);
	}
	else
		len = ft_strlcpy(path, *envpath, pathsize);
	len += ft_strlcat(path, "/", pathsize);
	len += ft_strlcat(path, filename, pathsize);
	if (len > pathsize - 1)
		path[0] = '\0';
	if (end == NULL)
		*envpath = NULL;
	else
		*envpath = end + 1;
}
static char	*search_path_mode(const char *filename, int mode, t_shell *shell)
{
	char	path[PATH_MAX];
	char	*envpath;
	char	*dup;

	envpath = get_env_value(shell->env_list, "PATH");
	while (envpath && *envpath)
	{
		set_path(path, PATH_MAX, filename, &envpath);
		if (access(path, mode) == 0)
		{
			dup = safe_strdup(path, shell);
			return (dup);
		}
	}
	return (NULL);
}

char	*search_path(const char *filename, t_shell *shell)
{
	char	*path;

	path = search_path_mode(filename, X_OK, shell);
	if (path)
		return (path);
	path = search_path_mode(filename, F_OK, shell);
	return (path);
}


