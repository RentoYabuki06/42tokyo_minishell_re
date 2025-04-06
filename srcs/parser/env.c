/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 21:24:50 by myokono           #+#    #+#             */
/*   Updated: 2025/04/06 21:26:27 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static char	*get_last_command_path(t_shell *shell)
{
	char	*path;

	path = get_env_value(shell->env_list, "_");
	if (path)
		return (path);
	return ("/bin/bash");
}

static int	is_delimiter(char c)
{
	return (is_space(c) || c == '|' || c == '<' || c == '>' || c == '\0');
}

int	expand_env_var(char *input, int *i, char **result, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (input[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_status);
		*result = ft_strjoin(*result, var_value);
		free(var_value);
		(*i)++;
		return (0);
	}
	if (input[*i] == '\'' && input[*i - 1] == '$')
	{
		(*i)--;
		return (handle_dollar_single_quote(input, i, result));
	}
	else if (input[*i] == '\"' && input[*i - 1] == '$')
	{
		(*i)--;
		return (handle_dollar_double_quote(input, i, result, shell));
	}
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		*result = ft_strjoin(*result, ft_strdup("$"));
		return (0);
	}
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	// 変数名がない場合 ($ のみ)
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
	{
		*result = ft_strjoin(*result, ft_strdup(var_value));
		free(var_value);
	}
	free(var_name);
	return (SUCCESS);
}
