/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:55 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 11:59:42 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (i == 0)
		{
			if (s[i] != c)
				count++;
		}
		else
		{
			if (s[i] != c && s[i - 1] == c)
				count++;
		}
		i++;
	}
	return (count);
}

static void	ft_allfree(char **ans, size_t j)
{
	size_t	i;

	i = 0;
	while (i < j)
	{
		free(ans[i]);
		ans[i] = NULL;
		i++;
	}
	if (ans != NULL)
		free(ans);
}

static void	fill_split(char **ans, char const *s, char c, size_t i)
{
	size_t	j;
	size_t	len;

	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break ;
		len = 0;
		while (s[i + len] && s[i + len] != c)
			len++;
		ans[j] = ft_strndup(s + i, len);
		if (!ans[j])
		{
			ft_allfree(ans, j);
			return ;
		}
		i += len;
		j++;
	}
	if (ans != NULL)
		ans[j] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**ans;

	if (!s)
		return (NULL);
	ans = (char **)malloc(sizeof(char *) * (ft_count(s, c) + 1));
	if (!ans)
		return (NULL);
	fill_split(ans, s, c, 0);
	return (ans);
}
