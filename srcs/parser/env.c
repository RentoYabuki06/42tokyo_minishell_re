/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:24:50 by myokono           #+#    #+#             */
/*   Updated: 2025/04/08 14:10:47 by yabukirento      ###   ########.fr       */
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
		input = ft_itoa(shell->exit_status);
		*result = ft_strjoin(*result, input);
		free(input);
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
		*result = ft_strjoin(*result, ft_strdup("$"));
		return (1);
	}
	return (0);
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
	{
		*result = ft_strjoin(*result, ft_strdup("$"));
		return (0);
	}
	var_name = ft_substr(input, start, *i - start);
	var_value = get_env_value(shell->env_list, var_name);
	if (ft_strcmp(var_name, "_") == 0 && !var_value)
		var_value = get_last_command_path(shell);
	if (var_value)
		*result = ft_strjoin(*result, ft_strdup(var_value));
	free(var_name);
	return (SUCCESS);
}
