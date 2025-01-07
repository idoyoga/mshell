/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:13:30 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/30 12:55:44 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free (lst);
	lst = NULL;
}
/*
void	delete_content(void *content)
{
	free(content);
}

int	main()
{
	t_list	*head;
	head = malloc(sizeof(t_list));
	head->content = ft_strdup("Node 1");
	head->next = NULL;

	ft_lstdelone(head, delete_content);
	if (head)
		printf("Node successfully deleted.\n");
	else
		printf("Node deletion failed.\n");
	return (0);
}*/
