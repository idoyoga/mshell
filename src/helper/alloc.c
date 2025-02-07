/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:24:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/07 14:05:59 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Dynamically resize the allocation tracker if the capacity is reached.
*/

static bool	alloc_tracker_resize(t_alloc *tracker, int new_capacity)
{
	void	**new_allocs;
	int		*new_flags;

	if (!tracker || new_capacity <= tracker->capacity)
		return (false);
	new_allocs = ft_calloc(new_capacity, sizeof(void *));
	new_flags = ft_calloc(new_capacity, sizeof(int));
	if (!new_allocs || !new_flags)
	{
		free(new_allocs);
		free(new_flags);
		new_allocs = NULL;
		new_flags = NULL;
		error_exit(NULL, NO_RESIZE, EXIT_FAILURE);
	}
	ft_memmove(new_allocs, tracker->allocs, tracker->count * sizeof(void *));
	ft_memmove(new_flags, tracker->flags, tracker->count * sizeof(int));
	free(tracker->allocs);
	free(tracker->flags);
	tracker->allocs = new_allocs;
	tracker->flags = new_flags;
	tracker->capacity = new_capacity;
	return (true);
}

/*
**	Alloc tracker, used to keep track of all allocated memory.
**	Resizes the allocation tracker if initial capacity is reached, 
**	doubling its capacity.
**	0 = single allocation, 1 = array allocation
*/

void	*alloc_tracker_add(t_alloc *tracker, void *ptr, int is_array)
{
	if (!ptr || !tracker || !tracker->initialized)
		return (NULL);
	if (tracker->count >= tracker->capacity)
	{
		if (!alloc_tracker_resize(tracker, tracker->capacity * 2))
			return (NULL);
	}
	tracker->allocs[tracker->count] = ptr;
	tracker->flags[tracker->count] = is_array;
	tracker->count++;
	return (ptr);
}

/*
**	Free all elements of a char ** array
*/

static void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while (array[++i])
	{
		free(array[i]);
		array[i] = NULL;
	}
}

/*
**	Helper function for free_allocs
*/

static void	free_tracker_allocs(t_alloc *tracker, int index)
{
	char	**array;

	if (tracker->flags[index] == 1)
	{
		array = (char **)tracker->allocs[index];
		free_array(array);
	}
	free(tracker->allocs[index]);
	tracker->allocs[index] = NULL;
}

/*
**	Free all allocated memory in one function call
*/

void	free_allocs(t_alloc *tracker)
{
	int		i;

	if (!tracker || !tracker->allocs || !tracker->initialized)
		return ;
	i = -1;
	while (++i < tracker->count)
	{
		if (tracker->allocs[i])
			free_tracker_allocs(tracker, i);
	}
	free(tracker->allocs);
	free(tracker->flags);
	tracker->allocs = NULL;
	tracker->flags = NULL;
	tracker->count = 0;
	tracker->capacity = 0;
	tracker->initialized = false;
}
