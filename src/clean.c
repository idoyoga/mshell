/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 11:11:54 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/14 14:50:24 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
**	Alloc tracker, used to keep track of all allocated memory
*/

bool alloc_tracker_add(t_alloc *tracker, void *ptr)
{
	if (!ptr || !tracker || tracker->count >= tracker->capacity)
		return (false);
	tracker->allocs[tracker->count++] = ptr;
	return (true);
}

/*
** Free all allocated memory in one function - that's at least the idea :)
*/

static void free_allocs(t_alloc *tracker)
{
	int	i;

	if (!tracker || !tracker->allocs)
		return ;
	i = -1;
	while (++i < tracker->count)
    {
        if (tracker->allocs[i])
            free(tracker->allocs[i]);
    }
    free(tracker->allocs);
    tracker->allocs = NULL;
    tracker->count = 0;
    tracker->capacity = 0;
}

/*
** Clean up the shell struct, more functions might be added here
*/

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_allocs(&shell->alloc_tracker);
}
