/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:35:52 by myokono           #+#    #+#             */
/*   Updated: 2025/04/06 19:56:30 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

static int	add_token_safe(t_token **tokens, t_token_type type, \
	const char *literal)
{
	char	*value;
	t_token	*token;

	value = ft_strdup(literal);
	if (!value)
		return (ERROR);
	token = create_token(type, value);
	if (!token)
	{
		free(value);
		return (ERROR);
	}
	if (add_token(tokens, token) != SUCCESS)
	{
		free(value);
		free(token);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	handle_special_token(char *input, int *i, t_token **tokens)
{
	int	pos;

	pos = *i;
	if (input[*i] == '|' && add_token_safe(tokens, TOKEN_PIPE, "|") == SUCCESS)
		(*i)++;
	else if (input[*i] == '<' && input[*i + 1] == '<' && \
		add_token_safe(tokens, TOKEN_HEREDOC, "<<") == SUCCESS)
		*i += 2;
	else if (input[*i] == '>' && input[*i + 1] == '>' && \
		add_token_safe(tokens, TOKEN_APPEND, ">>") == SUCCESS)
		*i += 2;
	else if (input[*i] == '<' && add_token_safe(tokens, TOKEN_REDIRECT_IN, "<") \
		== SUCCESS)
		(*i)++;
	else if (input[*i] == '>' && add_token_safe(tokens, TOKEN_REDIRECT_OUT, \
		">") == SUCCESS)
		(*i)++;
	if (pos == *i)
		return (ERROR);
	return (SUCCESS);
}


t_token	*tokenize(char *input, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			if (handle_special_token(input, &i, &tokens) == ERROR)
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (handle_word_token(input, &i, &tokens, shell) == ERROR)
				return (free_tokens(tokens), NULL);
		}
	}
	return (tokens);
}
