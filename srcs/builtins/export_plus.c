/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_plus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:23:53 by myokono           #+#    #+#             */
/*   Updated: 2025/04/21 13:25:14 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup_key_value(char *arg, char *eq, char **key, char **val)
{
	*key = ft_substr(arg, 0, eq - arg - 1);
	if (!*key)
		return (system_error("ft_substr"), ERROR);
	*val = ft_strdup(eq + 1);
	if (!*val)
	{
		free(*key);
		return (system_error("ft_strdup"), ERROR);
	}
	return (SUCCESS);
}

static int	validate_identifier(char *arg, char *key)
{
	char	*msg;

	if (is_valid_identifier(key))
		return (SUCCESS);
	msg = ft_strjoin(arg, ": not a valid identifier");
	if (!msg)
		return (system_error("ft_strjoin"), ERROR);
	print_error("export", msg);
	free(msg);
	return (ERROR);
}

static int	concat_if_exists(t_shell *sh, char *key, char **val)
{
	char	*old;
	char	*tmp;

	old = get_env_value(sh->env_list, key);
	if (!old)
		return (SUCCESS);
	tmp = ft_strjoin(old, *val);
	if (!tmp)
		return (system_error("ft_strjoin"), ERROR);
	free(*val);
	*val = tmp;
	return (SUCCESS);
}

int	export_with_plus_equal(char *arg, char *eq, t_shell *sh)
{
	char	*key;
	char	*val;
	int		st;

	st = dup_key_value(arg, eq, &key, &val);
	if (st == SUCCESS && validate_identifier(arg, key) == SUCCESS)
		st = concat_if_exists(sh, key, &val);
	if (st == SUCCESS && add_env_node(&sh->env_list, key, val) == ERROR)
		st = ERROR;
	if (st == ERROR)
		free(val);
	free(key);
	free(val);
	return (st);
}
