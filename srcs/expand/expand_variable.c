/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:28 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:26:38 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_variable_str(char **dst, char **rest, char *p, t_shell *shell)
{
	char	*name;
	char	*value;

	name = safe_calloc(1, sizeof(char), shell);
	if (*p != '$')
		assert_error("Expected dollar sign", shell);
	p++;
	if (!is_alpha_under(*p))
		assert_error("Variable must starts with alphabets or underscore.", shell);
	append_char(&name, *p++, shell);
	while (is_alpha_num_under(*p))
		append_char(&name, *p++, shell);
	value = get_env_value(shell->env_list, name);
	free(name);
	if (value)
		while (*value)
			append_char(dst, *value++, shell);
	*rest = p;
}
