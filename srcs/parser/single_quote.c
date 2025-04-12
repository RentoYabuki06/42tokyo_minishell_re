/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:36:55 by myokono           #+#    #+#             */
/*   Updated: 2025/04/09 16:58:52 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	append_escape(char *result, int *j, char c)
{
	if (c == 'n')
		result[(*j)++] = '\n';
	else if (c == 't')
		result[(*j)++] = '\t';
	else if (c == 'r')
		result[(*j)++] = '\r';
	else if (c == 'a')
		result[(*j)++] = '\a';
	else if (c == 'b')
		result[(*j)++] = '\b';
	else if (c == 'f')
		result[(*j)++] = '\f';
	else if (c == 'v')
		result[(*j)++] = '\v';
	else if (c == '\\' || c == '\'' || c == '\"')
		result[(*j)++] = c;
	else
	{
		result[(*j)++] = '\\';
		result[(*j)++] = c;
	}
}

static void	process_escape_sequence(char *str, char *result, int *i, int *j)
{
	if (str[*i] == '\\' && str[*i + 1])
	{
		(*i)++;
		append_escape(result, j, str[*i]);
		(*i)++;
	}
	else
	{
		result[(*j)++] = str[*i];
		(*i)++;
	}
}

char	*interpret_escapes(char *str)
{
	char	*result;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
	{
		free(str);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (str[i])
		process_escape_sequence(str, result, &i, &j);
	result[j] = '\0';
	free(str);
	return (result);
}

int	handle_dollar_single_quote(char *input, int *i, char **result)
{
	int		start;
	char	*content;
	char	*tmp_result;

	(*i) += 2;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
		return (error_message("Syntax error: unclosed quote in $'...'"), ERROR);
	content = ft_substr(input, start, *i - start);
	if (!content)
		return (ERROR);
	content = interpret_escapes(content);
	if (!content)
		return (error_message("Memory allocation error"), ERROR);
	tmp_result = ft_strdup(*result);
	if (tmp_result == NULL)
	{
		free(content);
		return (ERROR);
	}
	free(*result);
	*result = ft_strjoin(tmp_result, content);
	free(tmp_result);
	free(content);
	if (!*result)
		return (error_message("Memory allocation error"), ERROR);
	(*i)++;
	return (SUCCESS);
}

int	handle_single_quote(char *input, int *i, char **result)
{
	int		start;
	char	*str_add;
	char	*tmp_result;

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
	str_add = ft_substr(input, start, *i - start);
	if (str_add == NULL)
		return (ERROR);
	tmp_result = ft_strdup(*result);
	if (tmp_result)
	{
		free(str_add);
		return (ERROR);
	}
	free(*result);
	*result = ft_strjoin(tmp_result, str_add);
	free(tmp_result);
	free(str_add);
	if (*result == NULL)
		return (ERROR);
	(*i)++;
	return (SUCCESS);
}
