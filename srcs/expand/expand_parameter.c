/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_parameter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:35:34 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:33:07 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_parameter_token(t_token *token, t_shell *shell)
{
	char	*new_value;
	char	*p;

	if (token == NULL || token->type != TOKEN_WORD || token->value == NULL)
		return ;
	p = token->value;
	new_value = safe_calloc(1, sizeof(char), shell);
	while (*p)
	{
		if (*p == SINGLE_QUOTE_CHAR)
			append_single_quote(&new_value, &p, p, shell);
		else if (*p == DOUBLE_QUOTE_CHAR)
			append_double_quote(&new_value, &p, p, shell);
		else if (is_variable(p))
		{
			expand_variable_str(&new_value, &p, p, shell);
			token->is_expanded = true;
		}
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_value, &p, p, shell);
		else
			append_char(&new_value, *p++, shell);
	}
	free(token->value);
	token->value = new_value;
	expand_parameter_token(token->next, shell);
}

// heredoc delimiter will not be expanded
// 
void	expand_parameter(t_node *node, t_shell *shell)
{
	if (node == NULL)
		return ;
	expand_parameter_token(node->args, shell);
	expand_parameter_token(node->filename, shell);
	expand_parameter(node->redirects, shell);
	expand_parameter(node->command, shell);
	expand_parameter(node->next, shell);
}
