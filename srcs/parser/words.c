/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:10:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 18:09:04 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_delimiter(char c)
{
	return (is_space(c) || c == '|' || c == '<' || c == '>' || c == '\0');
}

static int	handle_word(char *input, int *i, char **result)
{
	int		start;
	char	*segment;
	char	*temp_result;

	start = *i;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"' || input[*i] == '$')
			break ;
		(*i)++;
	}
	segment = ft_substr(input, start, *i - start);
	if (segment == NULL)
		return (ERROR);
	temp_result = ft_strjoin(*result, segment);
	free(segment);
	if (temp_result == NULL)
		return (ERROR);
	free(*result);
	*result = temp_result;
	return (SUCCESS);
}

int	process_dollar_quote(char *input, int *i, char **result, t_shell *shell)
{
	if (input[*i + 1] == '\'')
	{
		if (handle_dollar_single_quote(input, i, result) == ERROR)
			return (ERROR);
	}
	else
	{
		if (dollar_doublequote(input, i, result, shell) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

int	process_quote_or_dollar(char *input, int *i, char **result, t_shell *shell)
{
	int	status;

	status = SUCCESS;
	if (input[*i] == '$' && input[*i + 1] && \
		(input[*i + 1] == '\'' || input[*i + 1] == '\"'))
		status = process_dollar_quote(input, i, result, shell);
	else if (input[*i] == '\'')
		status = handle_single_quote(input, i, result);
	else if (input[*i] == '\"')
		status = handle_double_quote(input, i, result, shell);
	else if (input[*i] == '$')
		status = expand_env_var(input, i, result, shell);
	else
		status = handle_word(input, i, result);
	return (status);
}

int	handle_word_token(char *input, int *i, t_token **tokens, t_shell *shell)
{
	char	*result;
	int		start_pos;

	result = ft_strdup("");
	if (result == NULL)
		return (ERROR);
	start_pos = *i;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (process_quote_or_dollar(input, i, &result, shell) == ERROR)
		{
			free(result);
			return (ERROR);
		}
	}
	if (*i > start_pos && result[0] != '\0')
	{
		t_token *new_token = create_token(TOKEN_WORD, result);
		if (!new_token)
		{
			free(result);
			return (ERROR);
		}
		add_token(tokens, new_token);
	}
	else
		free(result);
	return (SUCCESS);
}
