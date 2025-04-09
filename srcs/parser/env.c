/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:24:50 by myokono           #+#    #+#             */
/*   Updated: 2025/04/09 17:16:04 by yabukirento      ###   ########.fr       */
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
	char	*tmp;
	char	*tmp_result;

	if (input[*i] == '?')
	{
		input = ft_itoa(shell->exit_status);
		if (input == NULL)
			return (ERROR);
		tmp_result = ft_strdup(*result);
		if (tmp_result == NULL)
		{
			free(input);
			return (ERROR);
		}
		free(*result);
		*result = ft_strjoin(tmp_result, input);
		free(input);
		free(tmp_result);
		if (*result == NULL)
			return (ERROR);
		(*i)++;
		return (1);
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
		tmp_result = ft_strdup(*result);
		if (tmp_result == NULL)
			return (ERROR);
		tmp = ft_strdup("$");
		if (tmp == NULL)
		{
			free(tmp_result);
			return (ERROR);
		}
		free(*result);
		*result = ft_strjoin(tmp_result, tmp);
		free(tmp_result);
		free(tmp);
		if (*result == NULL)
			return (ERROR);
		return (1);
	}
	return (0);
}

int	expand_env_var(char *input, int *i, char **result, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*tmp_result;
	char	*tmp_varvalue;

	(*i)++;
	if (handle_special(input, i, result, shell))
		return (0);
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	if (start == *i)
	{
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
	var_name = ft_substr(input, start, *i - start);
	if (var_name == NULL)
		return (ERROR);
	var_value = get_env_value(shell->env_list, var_name);
	if (ft_strcmp(var_name, "_") == 0 && !var_value)
		var_value = get_last_command_path(shell);
	free(var_name);
	if (var_value == NULL)
		return (ERROR);
	tmp_varvalue = ft_strdup(var_value);
	if (tmp_varvalue == NULL)
		return (ERROR);
	tmp_result = ft_strdup(*result);
	if (tmp_result == NULL)
	{
		free(tmp_varvalue);
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
