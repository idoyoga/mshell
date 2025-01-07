/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:28:29 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/30 11:25:13 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
/*
int	main()
{
	t_list	*head;
	t_list	*node1;
	t_list	*node2;
	t_list	*current;

	node1 = malloc(sizeof(t_list));
	node1->content = "Node 1";
	node1->next = NULL;

	node2 = malloc(sizeof(t_list));
	node2->content = "Node 2";
	node2->next = NULL;

	head = NULL;
	ft_lstadd_front(&head, node1);
	ft_lstadd_front(&head, node2);

	current = head;
	while (current)
	{
		printf("%s ", (char *)current->content);
		current = current->next;
	}
	printf("\n");
	while (head)
	{
		t_list *temp = head;
		head = head->next;
		free(temp);
	}
}*/
