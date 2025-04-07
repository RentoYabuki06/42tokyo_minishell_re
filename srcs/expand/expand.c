/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:34:27 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 17:49:30 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char(char **src, char c, t_shell *shell)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*src)
		size += ft_strlen(*src);
	new = safe_malloc(size, shell);
	if (*src)
		ft_strlcpy(new, *src, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*src)
		free(*src);
	*src = new;
}

void	expand(t_shell *shell)
{
	expand_parameter(shell->node, shell);
	expand_word_splitting(shell->node, shell);
	expand_quote_removal(shell->node, shell);
}