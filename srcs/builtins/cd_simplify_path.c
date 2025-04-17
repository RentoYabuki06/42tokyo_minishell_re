/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_simplify_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:39:58 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/17 10:44:12 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_2d(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
		free(array[i++]);
	free(array);
}

static void	cut_current_dir(char *result)
{
	char	*last_slash;

	last_slash = strrchr(result, '/');
	if (last_slash && last_slash != result)
		*last_slash = '\0';
	else
		ft_strlcpy(result, "/", sizeof(result));
}

static void	ft_loop(char **components, char *result)
{
	int	i;

	i = -1;
	while (components[++i])
	{
		if (ft_strcmp(components[i], "") == 0)
			continue ;
		if (ft_strcmp(components[i], ".") == 0)
			continue ;
		if (ft_strcmp(components[i], "..") == 0)
			cut_current_dir(result);
		else
		{
			if (ft_strcmp(result, "/") != 0)
				ft_strlcat(result, "/", sizeof(result));
			ft_strlcat(result, components[i], sizeof(result));
		}
	}
}

char	*simplify_path(const char *path)
{
	char	**components;
	char	result[PATH_MAX];

	components = ft_split(path, '/');
	if (!components)
		return (NULL);
	result[0] = '/';
	result[1] = '\0';
	ft_loop(components, result);
	free_2d(components);
	return (ft_strdup(result));
}
