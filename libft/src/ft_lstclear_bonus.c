/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:03:31 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/30 12:41:40 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst || !del || !(*lst))
		return ;
	tmp = (*lst)->next;
	if (tmp)
		ft_lstclear(&tmp, del);
	(del)((*lst)->content);
	free(*lst);
	*lst = NULL;
}
/*
void	delete_int(void *data)
{
	int	*value;

	value = (int *)data;
	printf("Deleting integer: %d\n", *value);
}

int	main()
{
	t_list	*node1;
	t_list	*node2;
	t_list	*node3;
	t_list	*lst;
	int		data1 = 10;
	int		data2 = 20;
	int		data3 = 30;

	node1 = malloc(sizeof(t_list));
	node2 = malloc(sizeof(t_list));
	node3 = malloc(sizeof(t_list));

	node1->content = &data1;
	node2->content = &data2;
	node3->content = &data3;
	
	node1->next = node2;
	node2->next = node3;
	node3->next = NULL;

	lst = node1;
	ft_lstclear(&lst, delete_int);
	if (!lst)
		printf("After clearing, list pointer is NULL\n");
	else
		printf("After clearing, list pointer is not NULL\n");
	return (0);
}*/
