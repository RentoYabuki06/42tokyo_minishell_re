/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:57:53 by ryabuki           #+#    #+#             */
/*   Updated: 2025/04/12 13:23:20 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

bool	join_result(char **result, char *add)
{
	char	*tmp_result;

	tmp_result = ft_strdup(*result);
	if (tmp_result == NULL)
		return (false);
	free(*result);
	*result = ft_strjoin(tmp_result, add);
	free(tmp_result);
	if (*result == NULL)
		return (false);
	return (true);
}

bool	join_result_free(char **result, char *add)
{
	char	*tmp_result;

	tmp_result = ft_strdup(*result);
	if (tmp_result == NULL)
		return (free(add), false);
	free(*result);
	*result = ft_strjoin(tmp_result, add);
	free(tmp_result);
	if (*result == NULL)
		return (free(add), false);
	return (free(add), true);
}
