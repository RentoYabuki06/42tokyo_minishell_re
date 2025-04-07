/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:40:32 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:42:06 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_single_quote(char **dst, char **rest, char *p, t_shell *shell)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		p++;
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote", shell);
			append_char(dst, *p++, shell);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected single quote", shell);
}

static void	remove_double_quote(char **dst, char **rest, char *p, t_shell *shell)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		p++;
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote", shell);
			append_char(dst, *p++, shell);
		}
		p++;
		*rest = p;
	}
	else
		assert_error("Expected double quote", shell);
}

static void	remove_quote(t_token *tok, t_shell *shell)
{
	char	*new_value;
	char	*p;

	if (tok == NULL || tok->type != TOKEN_WORD || tok->value == NULL)
		return ;
	p = tok->value;
	new_value = safe_calloc(1, sizeof(char), shell);
	while (*p)
	{
		if (*p == SINGLE_QUOTE_CHAR)
			remove_single_quote(&new_value, &p, p, shell);
		else if (*p == DOUBLE_QUOTE_CHAR)
			remove_double_quote(&new_value, &p, p, shell);
		else
			append_char(&new_value, *p++, shell);
	}
	free(tok->value);
	tok->value = new_value;
	remove_quote(tok->next, shell);
}

void	expand_quote_removal(t_node *node, t_shell *shell)
{
	if (node == NULL)
		return ;
	remove_quote(node->args, shell);
	remove_quote(node->filename, shell);
	remove_quote(node->delimiter, shell);
	expand_quote_removal(node->redirects, shell);
	expand_quote_removal(node->command, shell);
	expand_quote_removal(node->next, shell);
}
