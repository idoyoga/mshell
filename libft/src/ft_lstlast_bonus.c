/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:59:27 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/30 16:28:24 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	if (!lst->next)
		return (lst);
	return (ft_lstlast(lst->next));
}
/*
int main()
{
	t_list	*head;
	t_list	*last;

	head = ft_lstnew("First node");
	ft_lstadd_back(&head, ft_lstnew("Second node"));
	ft_lstadd_back(&head, ft_lstnew("Third node"));
	last = ft_lstlast(head);

	if (last)
		printf("Last element: %s\n", (char *)last->content);
	else
		printf("List is empty.\n");
	return (0);
}*/
