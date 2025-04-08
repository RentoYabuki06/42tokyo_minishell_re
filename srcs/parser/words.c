/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:10:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/08 14:34:36 by myokono          ###   ########.fr       */
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
	temp_result = ft_strjoin(*result, segment);
	free(segment);
	free(*result);
	*result = temp_result;
	if (!*result)
	{
		error_message("Memory allocation error");
		return (ERROR);
	}
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
	int		has_content;
	int		start_pos;
	int		status;

	result = ft_strdup("");
	has_content = 0;
	start_pos = *i;
	status = SUCCESS;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		status = process_quote_or_dollar(input, i, &result, shell);
		if (status == ERROR)
		{
			free(result);
			return (ERROR);
		}
		has_content = 1;
	}
	if (has_content || ft_strlen(result) > 0 || (*i > start_pos))
		add_token(tokens, create_token(TOKEN_WORD, result));
	else
		free(result);
	return (SUCCESS);
}
