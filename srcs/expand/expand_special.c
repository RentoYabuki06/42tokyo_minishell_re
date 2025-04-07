/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:30:21 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:31:27 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_special_parameter(char *s)
{
	return (s[0] == '$' && s[1] == '?');
}

static void	append_num(char **dst, unsigned int num, t_shell *shell)
{
	if (num == 0)
	{
		append_char(dst, '0', shell);
		return ;
	}
	if (num / 10 != 0)
		append_num(dst, num / 10, shell);
	append_char(dst, '0' + (num % 10), shell);
}

void	expand_special_parameter_str(char **dst, char **rest, char *p, t_shell *shell)
{
	if (!is_special_parameter(p))
		assert_error("Expected special parameter", shell);
	p += 2;
	append_num(dst, shell->exit_status, shell);
	*rest = p;
}
