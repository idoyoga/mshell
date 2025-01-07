/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:57:45 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/30 14:24:32 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst | !f)
		return ;
	ft_lstiter(lst->next, f);
	(f)(lst->content);
}
/*
void	print_content(void *content)
{
	printf("%s\n", (char*)content);
}

int	main()
{
	t_list *node1 = malloc(sizeof(t_list));
	t_list *node2 = malloc(sizeof(t_list));
	t_list *node3 = malloc(sizeof(t_list));

	node1->content = "Let's test";
	node1->next = node2;
	node2->content = "this stuff";
	node2->next = node3;
	node3->content = "!";
	node3->next = NULL;

	printf("Printing contents of the linked list:\n");
//	ft_lstiter(node1, print_content);
	t_list *tmp = node1;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	free(node1);
	free(node2);
	free(node3);
	return (0);
}*/
