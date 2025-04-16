/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:41:17 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 21:52:48 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dollar_doublequote(char *input, int *i, char **result, t_shell *shell)
{
	(*i)++;
	return (handle_double_quote(input, i, result, shell));
}

static int	append_segment_in_quotes(char *input, int *i, char **result)
{
	int		start;
	char	*segment;
	char	*tmp_result;

	start = *i;
	while (input[*i] && input[*i] != '\"' && input[*i] != '$')
		(*i)++;
	segment = ft_substr(input, start, *i - start);
	if (segment == NULL)
		return (ERROR);
	tmp_result = ft_strdup(*result);
	if (tmp_result == NULL)
	{
		free(segment);
		return (ERROR);
	}
	free(*result);
	*result = ft_strjoin(tmp_result, segment);
	free(segment);
	free(tmp_result);
	if (*result == NULL)
		return (ERROR);
	return (SUCCESS);
}

static int	dollar_in_quote(char *input, int *i, char **result, t_shell *shell)
{
	if (input[*i + 1] == '\"')
	{
		if (join_result(result, "$") == false)
			return (ERROR);
		(*i)++;
		return (SUCCESS);
	}
	return (expand_env_var(input, i, result, shell));
}

int	handle_double_quote(char *input, int *i, char **result, t_shell *shell)
{
	(*i)++;
	if (input[*i] == '\"')
	{
		(*i)++;
		return (SUCCESS);
	}
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$')
		{
			if (dollar_in_quote(input, i, result, shell) == ERROR)
				return (ERROR);
		}
		else
		{
			if (append_segment_in_quotes(input, i, result) == ERROR)
				return (ERROR);
		}
	}
	if (!input[*i])
		return (error_message("Syntax error: unclosed double quote"), ERROR);
	(*i)++;
	return (SUCCESS);
}
