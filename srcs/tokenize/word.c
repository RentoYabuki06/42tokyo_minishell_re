/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:22:20 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:01:51 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	consume_single_quote(char **rest, char *line, t_shell *shell)
{
	if (*line == SINGLE_QUOTE_CHAR)
	{
		line++;
		while (*line && *line != SINGLE_QUOTE_CHAR)
			line++;
		if (*line == '\0')
		{
			shell->flag_error = true;
			token_error("Unclosed single quote", &line, line);
		}
		else
			line++;
		*rest = line;
		return (true);
	}
	return (false);
}

static bool	consume_double_quote(char **rest, char *line, t_shell *shell)
{
	if (*line == DOUBLE_QUOTE_CHAR)
	{
		line++;
		while (*line && *line != DOUBLE_QUOTE_CHAR)
			line++;
		if (*line == '\0')
		{
			shell->flag_error = true;
			token_error("Unclosed double quote", &line, line);
		}
		else
			line++;
		*rest = line;
		return (true);
	}
	return (false);
}

t_token	*word(char **rest, char *line, t_shell *shell)
{
	const char	*start = line;
	char		*word;

	while (*line && !is_metacharacter(*line))
	{
		if (consume_single_quote(&line, line, shell))
			;
		else if (consume_double_quote(&line, line, shell))
			;
		else
			line++;
	}
	word = safe_strndup(start, line - start, shell);
	*rest = line;
	return (new_token(word, TOKEN_WORD, shell));
}
