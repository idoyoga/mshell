/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:57:17 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/30 11:19:14 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}
/*
int	main()
{
	t_list	*node1;
	t_list	*node2;
	t_list	*node3;
	t_list	*lst;
	t_list	*current;

	node1 = ft_lstnew("Node 1");
	node2 = ft_lstnew("Node 2");
	node3 = ft_lstnew("Node 3");
	lst = NULL;
	ft_lstadd_back(&lst, node1);
	ft_lstadd_back(&lst, node2);
	ft_lstadd_back(&lst, node3);
	printf("Contents of the list:\n");
	current = lst;
	while (current)
	{
		printf("%s\n", (char *)(current->content));
		current = current->next;
	}
	return (0);
}*/
