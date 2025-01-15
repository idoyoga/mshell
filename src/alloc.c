/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:24:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/15 17:36:32 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** Dynamically resize the allocation tracker if the capacity is reached.
*/

static bool	alloc_tracker_resize(t_alloc *tracker, int new_capacity)
{
	void	**new_allocs;

	if (!tracker || new_capacity <= tracker->capacity)
		return (false);
	new_allocs = ft_calloc(new_capacity, sizeof(void *));
	if (!new_allocs)
		return (error(NULL, NO_RESIZE, false));
	ft_memmove(new_allocs, tracker->allocs, tracker->count * sizeof(void *));
	free(tracker->allocs);
	tracker->allocs = new_allocs;
	tracker->capacity = new_capacity;
	return (true);
}

/*
**	Alloc tracker, used to keep track of all allocated memory.
**	Resizes the allocation tracker if initial capacity is reached, 
**	doubling its capacity.
*/

bool	alloc_tracker_add(t_alloc *tracker, void *ptr)
{
	if (!ptr || !tracker || !tracker->initialized)
		return (false);
	if (tracker->count >= tracker->capacity)
	{
		if (!alloc_tracker_resize(tracker, tracker->capacity * 2))
			return (error(NULL, NO_RESIZE, false));
	}
	tracker->allocs[tracker->count++] = ptr;
	return (true);
}

/*
** Wrapper for malloc, automatically register allocations with the tracker
*/

void	*wrap_malloc(t_alloc *tracker, size_t size)
{
	void	*ptr;

	if (!tracker || size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr || !alloc_tracker_add(tracker, ptr))
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	return (ptr);
}

/*
** Wrapper for calloc, automatically register allocations with the tracker
*/

void	*wrap_calloc(t_alloc *tracker, size_t count, size_t size)
{
	void	*ptr;

	if (!tracker || count == 0 || size == 0)
		return (NULL);
	ptr = ft_calloc(count, size);
	if (!ptr || !alloc_tracker_add(tracker, ptr))
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	return (ptr);
}

/*
** Free all allocated memory in one function call
*/

void	free_allocs(t_alloc *tracker)
{
	int	i;

	if (!tracker || !tracker->allocs || !tracker->initialized)
		return ;
	i = -1;
	while (++i < tracker->count)
	{
		if (tracker->allocs[i])
		{
			free(tracker->allocs[i]);
			tracker->allocs[i] = NULL;
		}
	}
	free(tracker->allocs);
	tracker->allocs = NULL;
	tracker->count = 0;
	tracker->capacity = 0;
	tracker->initialized = false;
}
