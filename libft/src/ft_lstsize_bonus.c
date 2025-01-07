/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:25:55 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/29 16:57:30 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
/*
int	main()
{
	t_list	*head;
	t_list	*node1;
	t_list	*node2;
	int		size;

	head = malloc(sizeof(t_list));
	head->content = "Head";
	head->next = NULL;

	node1 = malloc(sizeof(t_list));
	node1->content = "Node 1";
	node1->next = NULL;

	node2 = malloc(sizeof(t_list));
	node2->content = "Node 2";
	node2->next = NULL;

	head->next = node1;
	node1->next = node2;

	size = ft_lstsize(head);
	printf("Size of the list: %d\n", size);
	free (head);
	free (node1);
	free (node2);
}*/
