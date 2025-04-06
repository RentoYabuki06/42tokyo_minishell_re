/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:10:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/06 21:27:23 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static int	handle_word(char *input, int *i, char **result)
{
	int		start;
	char	*segment;

	start = *i;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"' || input[*i] == '$')
			break ;
		(*i)++;
	}
	segment = ft_substr(input, start, *i - start);
	*result = ft_strjoin(*result, segment);
	free(segment);
	if (!*result)
	{
		error_message("Memory allocation error");
		return (ERROR);
	}
	return (SUCCESS);
}

int	handle_word_token(char *input, int *i, t_token **tokens, t_shell *shell)
{
	char	*result;
	int		has_content;
	int		start_pos;

	result = ft_strdup("");
	has_content = 0;
	start_pos = *i;
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '$' && input[*i + 1] && \
			(input[*i + 1] == '\'' || input[*i + 1] == '\"'))
		{
			if (input[*i + 1] == '\'')
			{
				if (handle_dollar_single_quote(input, i, &result) == ERROR)
					return (free(result), ERROR);
			}
			else
			{
				if (handle_dollar_double_quote(input, i, &result, shell) \
					== ERROR)
					return (free(result), ERROR);
			}
			has_content = 1;
		}
		else if (input[*i] == '\'')
		{
			if (handle_single_quote(input, i, &result) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
		else if (input[*i] == '\"')
		{
			if (handle_double_quote(input, i, &result, shell) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
		else if (input[*i] == '$')
		{
			if (expand_env_var(input, i, &result, shell) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
		else
		{
			if (handle_word(input, i, &result) == ERROR)
				return (free(result), ERROR);
			has_content = 1;
		}
	}
	if (has_content || ft_strlen(result) > 0 || (*i > start_pos))
		add_token(tokens, create_token(TOKEN_WORD, result));
	else
		free(result);
	return (SUCCESS);
}
