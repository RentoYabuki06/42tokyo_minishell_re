/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:32:46 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 18:54:01 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*operator(char **rest, char *line, t_shell *shell)
{
	static char *const	operators[] = {">>", "<<", "<", ">", "|", "\n"};
	size_t				i;				
	char				*op;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (is_prefix(line, operators[i]))
		{
			op = safe_strdup(operators[i], shell);
				*rest = line + ft_strlen(op);
			return (new_token(op, TOKEN_OPERATOR, shell));
		}
		i++;
	}
	shell->flag_error = true;
	token_error("Unexpected Token", &line, line);
	return (NULL);
}


void	tokenize(char	*line, t_shell *shell)
{
	t_token	head;
	t_token	*token;

	head.next = NULL;
	token = &head;
	while (*line)
	{
		if (is_space(*line))
			line++;
		else if (is_metacharacter(*line))
		{
			token->next = operator(&line, line, shell);
			token = token->next;
		}
		else if (is_word(line))
		{
			token->next = word(&line, line, shell);
			token = token->next;
		}
		else
		{
			shell->flag_error = true;
			token_error("Unexpected Token", &line, line);
		}
	}
	token->next = new_token(NULL, TOKEN_EOF, shell);
	shell->token = head.next;
}
