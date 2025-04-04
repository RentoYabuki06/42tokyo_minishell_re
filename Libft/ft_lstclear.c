/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:54:03 by ryabuki           #+#    #+#             */
/*   Updated: 2024/04/21 20:25:52 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*plst;
	t_list	*tmp;

	if (!lst || !del || !*lst)
		return ;
	plst = *lst;
	while (plst)
	{
		tmp = plst->next;
		del(plst->content);
		free(plst);
		plst = tmp;
	}
	*lst = NULL;
}
