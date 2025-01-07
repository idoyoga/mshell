/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:39:54 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/23 10:29:27 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
/*
int	main()
{
	int	content = 42;

	t_list *new_elem = ft_lstnew(&content);

	if (new_elem)
	{
		printf("Content of the new element: %d\n", *((int*)new_elem->content));
		printf("Next pointer of the new element: %p\n", (void*)new_elem->next);
		free (new_elem);
	}
	else
		printf("Error: Failed to create a new list element.\n");
	return (0);
}*/
