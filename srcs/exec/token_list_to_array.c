/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_to_array.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:19:04 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:19:47 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	argv_len(t_token *tok)
{
	size_t	len;

	len = 0;
	while (tok && !is_eof(tok))
	{
		len++;
		tok = tok->next;
	}
	return (len);
}

char	**token_list_to_argv(t_token *tok, t_shell *shell)
{
	char	**argv;
	size_t	i;

	argv = safe_calloc(argv_len(tok) + 1, sizeof(char *), shell);
	i = 0;
	while (tok && !is_eof(tok))
	{
		argv[i] = safe_strdup(tok->value, shell);
		i++;
		tok = tok->next;
	}
	return (argv);
}