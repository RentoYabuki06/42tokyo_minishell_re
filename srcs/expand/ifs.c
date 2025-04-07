/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ifs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:31:52 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:34:42 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_default_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static bool	is_ifs(char c, t_shell *shell)
{
	char	*ifs;

	ifs = get_env_value(shell->env_list, "IFS");
	if (ifs == NULL)
		return (is_default_ifs(c));
	if (c == '\0')
		return (false);
	return (ft_strchr(ifs, c) != NULL);
}

static bool	consume_default_ifs(char **rest, char *p, t_shell *shell)
{
	bool	consumed;

	consumed = false;
	while (*p && is_ifs(*p, shell) && is_default_ifs(*p))
	{
		consumed = true;
		p++;
	}
	*rest = p;
	return (consumed);
}

bool	consume_ifs(char **rest, char *line, t_shell *shell)
{
	if (is_ifs(*line, shell))
	{
		consume_default_ifs(&line, line, shell);
		if (is_ifs(*line, shell))
			line++;
		consume_default_ifs(rest, line, shell);
		return (true);
	}
	*rest = line;
	return (false);
}

void	trim_ifs(char **rest, char *p, t_shell *shell)
{
	char	*last;

	consume_default_ifs(&p, p, shell);
	*rest = p;
	last = NULL;
	while (*p)
	{
		last = p;
		if (consume_default_ifs(&p, p, shell))
			;
		else
			p++;
	}
	if (last && is_ifs(*last, shell) && is_default_ifs(*last))
		*last = '\0';
}
