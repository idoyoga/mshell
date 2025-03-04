/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:42:48 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 19:43:01 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Replace an old pointer with a new one in the allocation tracker.
**	If the old pointer is NULL, the new pointer is added to the tracker.
**	This function correctly frees the readline buffer when replacing it.
*/

void	*alloc_tracker_replace(t_alloc *tracker, void *old_ptr, void *new_ptr)
{
	int	i;

	if (!tracker || !new_ptr)
		return (NULL);
	if (!old_ptr)
		return (alloc_tracker_add(tracker, new_ptr, 0));
	i = -1;
	while (++i < tracker->count)
	{
		if (tracker->allocs[i] == old_ptr)
		{
			free(tracker->allocs[i]);
			tracker->allocs[i] = new_ptr;
			return (new_ptr);
		}
	}
	return (NULL);
}
