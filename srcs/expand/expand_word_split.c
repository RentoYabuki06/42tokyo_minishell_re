/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:38:55 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:36:28 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	insert_new_tok(char **new_word, t_token **rest, t_token *token, t_shell *shell)
{
	t_token	*new_tok;
	
	token->value = *new_word;
	*new_word = safe_calloc(1, sizeof(char), shell);
	new_tok = new_token(NULL, TOKEN_WORD, shell);
	new_tok->next = token->next;
	token->next = new_tok;
	*rest = token->next;
}

static void	word_split(t_token *token, t_shell *shell)
{
	char	*new_word;
	char	*to_free;
	char	*p;
	
	to_free = token->value;
	trim_ifs(&p, token->value, shell);
	new_word = safe_calloc(1, sizeof(char), shell);
	while (*p)
	{
		if (*p == SINGLE_QUOTE_CHAR)
		append_single_quote(&new_word, &p, p, shell);
		else if (*p == DOUBLE_QUOTE_CHAR)
		append_double_quote(&new_word, &p, p, shell);
		else if (consume_ifs(&p, p, shell))
		{
			if (*p)
			insert_new_tok(&new_word, &token, token, shell);
		}
		else
		append_char(&new_word, *p++, shell);
	}
	token->value = new_word;
	free(to_free);
}

void	expand_word_splitting_token(t_token *token, t_shell *shell)
{
	if (token == NULL || token->type != TOKEN_WORD || token->value == NULL)
	return ;
	if (!token->is_expanded)
	return (expand_word_splitting_token(token->next, shell));
	word_split(token, shell);
	expand_word_splitting_token(token->next, shell);
}

void	expand_word_splitting(t_node *node, t_shell *shell)
{
	if (node == NULL)
		return ;
	expand_word_splitting_token(node->args, shell);
	expand_word_splitting_token(node->filename, shell);
	expand_word_splitting(node->redirects, shell);
	expand_word_splitting(node->command, shell);
	expand_word_splitting(node->next, shell);
}
