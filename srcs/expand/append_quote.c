/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:35:11 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:35:40 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_single_quote(char **dst, char **rest, char *p, t_shell *shell)
{
	if (*p == SINGLE_QUOTE_CHAR)
	{
		append_char(dst, *p++, shell);
		while (*p != SINGLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed single quote", shell);
			append_char(dst, *p++, shell);
		}
		append_char(dst, *p++, shell);
		*rest = p;
	}
	else
		assert_error("Expected single quote", shell);
}

void	append_double_quote(char **dst, char **rest, char *p, t_shell *shell)
{
	if (*p == DOUBLE_QUOTE_CHAR)
	{
		append_char(dst, *p++, shell);
		while (*p != DOUBLE_QUOTE_CHAR)
		{
			if (*p == '\0')
				assert_error("Unclosed double quote", shell);
			else if (is_variable(p))
				expand_variable_str(dst, &p, p, shell);
			else if (is_special_parameter(p))
				expand_special_parameter_str(dst, &p, p, shell);
			else
				append_char(dst, *p++, shell);
		}
		append_char(dst, *p++, shell);
		*rest = p;
	}
	else
		assert_error("Expected double quote", shell);
}
