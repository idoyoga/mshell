/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 09:36:20 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/15 10:40:08 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** Allocate and initialise new node
*/

static bool	init_list(t_shell *shell, t_lst **node, void *data)
{
	if (!data)
		return (false);
	*node = (t_lst *)wrap_malloc(&(shell->alloc_tracker), sizeof(t_lst));
	if (!(*node))
		return (false);
	(*node)->data = data;
	(*node)->next = NULL;
	(*node)->prev = NULL;
	return (true);
}

/*
** Append a new node to the end of the list. If the list is empty,
** the new node will be the first node and made circular.
*/

bool	append_node(t_shell *shell, t_lst **lst, void *data)
{
	t_lst	*node;

	if (!data || !init_list(shell, &node, data))
		return (false);
	if (!(*lst))
	{
		(*lst) = node;
		node->prev = node;
		node->next = node;
	}
	else
	{
		node->prev = (*lst)->prev;
		node->next = (*lst);
		(*lst)->prev->next = node;
		(*lst)->prev = node;
	}
	return (true);
}

size_t	lst_len(t_lst *lst)
{
	t_lst	*curr;
	size_t	count;

	if (!lst)
		return (0);
	curr = lst;
	count = 1;
	while (curr->next != lst)
	{
		curr = curr->next;
		count++;
	}
	return (count);
}
