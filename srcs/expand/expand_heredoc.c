/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:50:17 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:31:58 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*new_word;
	char	*p;

	p = line;
	new_word = safe_calloc(1, sizeof(char), shell);
	while (*p)
	{
		if (is_variable(p))
			expand_variable_str(&new_word, &p, p, shell);
		else if (is_special_parameter(p))
			expand_special_parameter_str(&new_word, &p, p, shell);
		else
			append_char(&new_word, *p++, shell);
	}
	free(line);
	return (new_word);
}
