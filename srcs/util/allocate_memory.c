/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:45 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 16:25:32 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size, t_shell *shell)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
		fatal_error("malloc failed", shell);
	return (ret);
}

void	*safe_calloc(size_t count, size_t size, t_shell *shell)
{
	void	*ret;

	ret = ft_calloc(count, size);
	if (ret == NULL)
		fatal_error("calloc failed", shell);
	return (ret);
}


char	*safe_strdup(const char *src, t_shell *shell)
{
	char	*ret;

	ret = ft_strdup(src);
	if (ret == NULL)
		fatal_error("strdup failed", shell);
	return (ret);
}

char	*safe_strndup(const char *src, size_t size, t_shell *shell)
{
	char	*ret;

	ret = ft_strndup(src, size);
	if (ret == NULL)
		fatal_error("strndup failed", shell);
	return (ret);
}
