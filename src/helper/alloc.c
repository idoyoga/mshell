/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:24:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/02 15:00:32 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Dynamically resize the allocation tracker if the capacity is reached,
**	growing the tracker by 50%.
*/

static bool	alloc_tracker_resize(t_alloc *tracker)
{
	void	**new_allocs;
	int		*new_flags;
	int		new_capacity;

	if (!tracker || tracker->count < tracker->capacity)
		return (false);
	new_capacity = tracker->capacity * 1.5;
	new_allocs = ft_calloc(new_capacity, sizeof(void *));
	new_flags = ft_calloc(new_capacity, sizeof(int));
	if (!new_allocs || !new_flags)
	{
		free(new_allocs);
		free(new_flags);
		error_exit(tracker->shell, NO_RESIZE, NULL, EXIT_FAILURE);
	}
	ft_memmove(new_allocs, tracker->allocs, tracker->count * sizeof(void *));
	ft_memmove(new_flags, tracker->is_array, tracker->count * sizeof(int));
	free(tracker->allocs);
	free(tracker->is_array);
	tracker->allocs = new_allocs;
	tracker->is_array = new_flags;
	tracker->capacity = new_capacity;
	return (true);
}

/*
**	Add a new allocation to the tracker. Resizes if necessary.
**	0 = single allocation, 1 = array allocation.
*/

void	*alloc_tracker_add(t_alloc *tracker, void *ptr, int is_array)
{
	if (!ptr || !tracker)
		return (NULL);
	if (tracker->count >= tracker->capacity)
	{
		if (!alloc_tracker_resize(tracker))
		{
			free(ptr);
			error_exit(tracker->shell, NO_TRACK, NULL, EXIT_FAILURE);
		}
	}
	tracker->allocs[tracker->count] = ptr;
	tracker->is_array[tracker->count] = is_array;
	tracker->count++;
	return (ptr);
}

/*
**	Remove an element from the allocation tracker efficiently.
**	Swaps with the last element to improve performance.
*/

void	alloc_tracker_remove(t_alloc *tracker, void *ptr)
{
	int	i;

	if (!tracker || !ptr || tracker->count == 0)
		return ;
	i = -1;
	while (++i < tracker->count)
	{
		if (tracker->allocs[i] == ptr)
		{
			tracker->allocs[i] = tracker->allocs[tracker->count - 1];
			tracker->is_array[i] = tracker->is_array[tracker->count - 1];
			tracker->allocs[tracker->count - 1] = NULL;
			tracker->is_array[tracker->count - 1] = 0;
			tracker->count--;
			return ;
		}
	}
	error(NO_REMOVE, false);
}

/*
**	Helper function to free memory tracked by the allocation tracker.
**	If 'is_array' is set, frees each element before freeing the main pointer.
*/

static void	free_tracker_allocs(void *alloc, int is_array)
{
	int		i;
	char	**array;

	if (!alloc)
		return ;
	if (is_array)
	{
		array = (char **)alloc;
		i = -1;
		while (array[++i])
			free(array[i]);
	}
	free(alloc);
}

/*
**	Free all tracked allocations and reset tracker fields.
*/

void	free_allocs(t_alloc *tracker)
{
	int	i;

	if (!tracker || !tracker->allocs)
		return ;
	i = -1;
	while (++i < tracker->count)
	{
		free_tracker_allocs(tracker->allocs[i], tracker->is_array[i]);
		tracker->allocs[i] = NULL;
	}
	free(tracker->allocs);
	free(tracker->is_array);
	*tracker = (t_alloc){0};
}
