/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:20:21 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 13:22:52 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_env_nodes(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static t_env	**copy_env_to_array(t_env *env_list, int count)
{
	t_env	**array;
	t_env	*current;
	int		i;

	array = (t_env **)malloc(sizeof(t_env *) * (count + 1));
	if (!array)
		return (NULL);
	current = env_list;
	i = 0;
	while (current)
	{
		array[i++] = current;
		current = current->next;
	}
	return (array);
}

static void	sort_env_array(t_env **env_array, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_array[j]->key, env_array[j + 1]->key) > 0)
			{
				tmp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_env_array(t_env **env_array, int count, int fd)
{
	int	i;

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
}

int	print_sorted_env(t_env *env_list, int fd)
{
	t_env	**env_array;
	int		count;

	count = count_env_nodes(env_list);
	env_array = copy_env_to_array(env_list, count);
	if (!env_array)
		return (system_error("malloc"), ERROR);
	sort_env_array(env_array, count);
	print_env_array(env_array, count, fd);
	free(env_array);
	return (SUCCESS);
}
