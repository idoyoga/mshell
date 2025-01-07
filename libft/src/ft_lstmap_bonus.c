/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 13:38:49 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/24 22:08:19 by dp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*node_new;
	t_list	*lst_new;
	void	*set;

	if (!lst || !f || !del)
		return (NULL);
	lst_new = NULL;
	while (lst)
	{
		set = f(lst->content);
		node_new = ft_lstnew(set);
		if (!node_new)
		{
			del(set);
			ft_lstclear(&lst_new, (*del));
			return (lst_new);
		}
		ft_lstadd_back(&lst_new, node_new);
		lst = lst->next;
	}
	return (lst_new);
}
