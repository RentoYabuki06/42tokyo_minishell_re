/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:36:55 by myokono           #+#    #+#             */
/*   Updated: 2025/04/06 19:37:44 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*interpret_escapes(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			if (str[i] == 'n')
				result[j++] = '\n';
			else if (str[i] == 't')
				result[j++] = '\t';
			else if (str[i] == 'r')
				result[j++] = '\r';
			else if (str[i] == 'a')
				result[j++] = '\a';
			else if (str[i] == 'b')
				result[j++] = '\b';
			else if (str[i] == 'f')
				result[j++] = '\f';
			else if (str[i] == 'v')
				result[j++] = '\v';
			else if (str[i] == '\\' || str[i] == '\'' || str[i] == '\"')
				result[j++] = str[i];
			else
				result[j++] = str[i];
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	free(str);
	return (result);
}

int	handle_dollar_single_quote(char *input, int *i, char **result)
{
	int		start;
	char	*content;

	(*i) += 2;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
	{
		error_message("Syntax error: unclosed quote in $'...'");
		return (ERROR);
	}
	content = ft_substr(input, start, *i - start);
	content = interpret_escapes(content);
	if (!content)
	{
		error_message("Memory allocation error");
		return (ERROR);
	}
	*result = ft_strjoin(*result, content);
	(*i)++;
	return (SUCCESS);
}

int	handle_single_quote(char *input, int *i, char **result)
{
	int	start;

	(*i)++;
	if (input[*i] == '\'')
	{
		(*i)++;
		return (SUCCESS);
	}
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
	{
		error_message("Syntax error: unclosed single quote");
		return (ERROR);
	}
	*result = ft_strjoin(*result, ft_substr(input, start, *i - start));
	(*i)++;
	return (SUCCESS);
}
