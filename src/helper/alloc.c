/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:24:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 17:39:25 by dplotzl          ###   ########.fr       */
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
	int		*new_array_flags;
	int		*new_cycle_flags;
	int		new_capacity;

	if (!tracker || tracker->count < tracker->capacity)
		return (false);
	new_capacity = tracker->capacity * 1.5;
	new_allocs = ft_calloc(new_capacity, sizeof(void *));
	new_array_flags = ft_calloc(new_capacity, sizeof(int));
	new_cycle_flags = ft_calloc(new_capacity, sizeof(int));
	if (!new_allocs || !new_array_flags || !new_cycle_flags)
	{
		free(new_allocs);
		free(new_array_flags);
		free(new_cycle_flags);
		error_exit(tracker->shell, NO_RESIZE, NULL, EXIT_FAILURE);
	}
	ft_memmove(new_allocs, tracker->allocs, tracker->count * sizeof(void *));
	ft_memmove(new_array_flags, tracker->is_array, tracker->count * sizeof(int));
	ft_memmove(new_cycle_flags, tracker->cycle_only, tracker->count * sizeof(int));
	free(tracker->allocs);
	free(tracker->is_array);
	free(tracker->cycle_only);
	tracker->allocs = new_allocs;
	tracker->is_array = new_array_flags;
	tracker->cycle_only = new_cycle_flags;
	tracker->capacity = new_capacity;
	return (true);
}

/*
**	Add a new allocation to the tracker. Resizes if necessary.
**	- is_array: 0 = single allocation, 1 = array allocation.
**	- cycle_flag: 0 = persistent allocation, 1 = cycle-only allocation.
*/

void	*alloc_tracker_add(t_alloc *tracker, void *ptr, int is_array, int cycle_flag)
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
	printf("✅ Adding allocation to tracker: %p (is_array=%d, cycle=%d)\n",
			ptr, is_array, cycle_flag);
	tracker->allocs[tracker->count] = ptr;
	tracker->is_array[tracker->count] = is_array;
	tracker->cycle_only[tracker->count] = cycle_flag;
	tracker->count++;
	return (ptr);
}

/*
**	Remove an element from the allocation tracker efficiently.
**	Swaps with the last element to improve performance.
*/

void	alloc_tracker_remove(t_alloc *tracker, void *ptr)
{
	int		i;

	if (!tracker || !ptr || tracker->count == 0)
		return ;
	i = -1;
	while (++i < tracker->count)
	{
		if (tracker->allocs[i] == ptr)
		{
			if (tracker->cycle_only[i])
				return ;
			printf("✅ Removing and freeing tracked allocation: %p\n", ptr);
			free_tracker_allocs(&tracker->allocs[i], tracker->is_array[i]);
			tracker->allocs[i] = tracker->allocs[tracker->count - 1];
			tracker->is_array[i] = tracker->is_array[tracker->count - 1];
			tracker->allocs[tracker->count - 1] = NULL;
			tracker->is_array[tracker->count - 1] = 0;
			tracker->count--;
			return ;
		}
	}
	printf("❌ ERROR: Pointer %p NOT FOUND in alloc_tracker_remove!\n", ptr);
	error(NO_REMOVE, false);
}

/*
**	Helper function to free memory tracked by the allocation tracker.
**	If 'is_array' is set, frees each element before freeing the main pointer.
*/

void	free_tracker_allocs(void **alloc, int is_array)
{
	int		i;
	char	**array;

	if (!alloc) 
		return ;
	if (is_array)
	{
		array = (char **)*alloc;
		i = -1;
		while (array[++i])
		{
			printf("🟠 Freeing array element in free_tracker_allocs: %p\n", array[i]);
			free(array[i]);
			array[i] = NULL;
		}
	}
	printf("🟢 Freeing allocation in free_tracker_allocs: %p\n", alloc);
	free(*alloc);
	*alloc = NULL;
}

/*
**	Free all tracked allocations and reset tracker fields.
*/

void	free_allocs(t_alloc *tracker)
{
	int	i;

	if (!tracker || !tracker->allocs)
		return ;
	printf("🧹 Calling free_allocs()...\n");
	i = 0;
	while (i < tracker->count)
	{
		if (tracker->allocs[i] && !tracker->cycle_only[i])
		{
			printf("🟢 Freeing allocation in free_allocs: %p\n", tracker->allocs[i]);
			alloc_tracker_remove(tracker, tracker->allocs[i]);
			free_tracker_allocs(&tracker->allocs[i], tracker->is_array[i]);
			tracker->allocs[i] = NULL;
			tracker->is_array[i] = 0;
			tracker->cycle_only[i] = 0;
		}
		i++;
	}
	free(tracker->allocs);
	free(tracker->is_array);
	free(tracker->cycle_only);
	*tracker = (t_alloc){0};
}
