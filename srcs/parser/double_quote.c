/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:41:17 by myokono           #+#    #+#             */
/*   Updated: 2025/04/06 20:32:07 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_dollar_double_quote(char *input, int *i, \
	char **result, t_shell *shell)
{
	(*i)++;
	return (handle_double_quote(input, i, result, shell));
}

// int	handle_double_quote(char *input, int *i, char **result, t_shell *shell)
// {
// 	int		start;
// 	char	*segment;

// 	(*i)++;
// 	if (input[*i] == '\"')
// 	{
// 		(*i)++;
// 		return (SUCCESS);
// 	}
// 	while (input[*i] && input[*i] != '\"')
// 	{
// 		if (input[*i] == '$')
// 		{
// 			if (input[*i + 1] == '\"')
// 			{
// 				*result = ft_strjoin(*result, ft_strdup("$"));
// 				(*i)++;
// 			}
// 			else if (expand_env_var(input, i, result, shell) == -1)
// 				return (ERROR);
// 		}
// 		else
// 		{
// 			start = *i;
// 			while (input[*i] && input[*i] != '\"' && input[*i] != '$')
// 				(*i)++;
// 			segment = ft_substr(input, start, *i - start);
// 			*result = ft_strjoin(*result, segment);
// 		}
// 	}
// 	if (!input[*i])
// 	{
// 		error_message("Syntax error: unclosed double quote");
// 		return (ERROR);
// 	}
// 	(*i)++;
// 	return (SUCCESS);
// }












static int	append_segment_in_quotes(char *input, int *i, char **result)
{
	int		start;
	char	*segment;
	char	*tmp;

	start = *i;
	while (input[*i] && input[*i] != '\"' && input[*i] != '$')
		(*i)++;
	segment = ft_substr(input, start, *i - start);
	if (!segment)
		return (ERROR);
	tmp = ft_strjoin(*result, segment);
	free(segment);
	if (!tmp)
		return (ERROR);
	free(*result);
	*result = tmp;
	return (SUCCESS);
}

static int	handle_dollar_in_quotes\
	(char *input, int *i, char **result, t_shell *shell)
{
	char	*tmp;

	if (input[*i + 1] == '\"')
	{
		tmp = ft_strjoin(*result, ft_strdup("$"));
		if (!tmp)
			return (ERROR);
		free(*result);
		*result = tmp;
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
			if (handle_dollar_in_quotes(input, i, result, shell) == ERROR)
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
