/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:46:41 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/23 13:26:54 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Wrapper for malloc, automatically register allocations with the tracker
*/

void	*wrap_malloc(t_alloc *tracker, size_t size)
{
	void	*ptr;

	if (!tracker || !tracker->initialized || size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr || !alloc_tracker_add(tracker, ptr, 0))
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	return (ptr);
}

/*
**	Wrapper for calloc, automatically register allocations with the tracker
*/

void	*wrap_calloc(t_alloc *tracker, size_t count, size_t size)
{
	void	*ptr;

	if (!tracker || !tracker->initialized || count == 0 || size == 0)
		return (NULL);
	ptr = ft_calloc(count, size);
	if (!ptr || !alloc_tracker_add(tracker, ptr, 0))
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	return (ptr);
}
