/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:24:50 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 14:56:18 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_last_command_path(t_shell *shell)
{
	char	*path;

	path = get_env_value(shell->env_list, "_");
	if (path)
		return (path);
	return ("/bin/bash");
}

static int	handle_special(char *input, int *i, char **result, t_shell *shell)
{
	if (input[*i] == '?')
	{
		input = ft_itoa(g_signal_status);
		if (input == NULL)
			return (ERROR);
		join_result_free(result, input);
		(*i)++;
		return (ERROR);
	}
	if (input[*i] == '\'' && input[*i - 1] == '$')
	{
		(*i)--;
		return (handle_dollar_single_quote(input, i, result));
	}
	else if (input[*i] == '\"' && input[*i - 1] == '$')
	{
		(*i)--;
		return (dollar_doublequote(input, i, result, shell));
	}
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		join_result(result, "$");
		return (ERROR);
	}
	return (SUCCESS);
}

static bool	handle_nokey(char **result)
{
	char	*tmp_result;
	char	*tmp_varvalue;

	tmp_result = ft_strdup(*result);
	if (tmp_result == NULL)
		return (ERROR);
	tmp_varvalue = ft_strdup("$");
	if (tmp_varvalue == NULL)
	{
		free(tmp_result);
		return (ERROR);
	}
	free(*result);
	*result = ft_strjoin(tmp_result, tmp_varvalue);
	free(tmp_result);
	free(tmp_varvalue);
	if (*result == NULL)
		return (ERROR);
	return (SUCCESS);
}

int	expand_env_var(char *input, int *i, char **result, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (handle_special(input, i, result, shell))
		return (0);
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	if (start == *i)
		return (handle_nokey(result));
	var_name = ft_substr(input, start, *i - start);
	if (var_name == NULL)
		return (ERROR);
	var_value = get_env_value(shell->env_list, var_name);
	if (ft_strcmp(var_name, "_") == 0 && !var_value)
		var_value = get_last_command_path(shell);
	free(var_name);
	if (var_value == NULL)
		return (ERROR);
	if (join_result(result, var_value) == false)
		return (ERROR);
	return (SUCCESS);
}
