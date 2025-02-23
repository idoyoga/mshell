/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:12 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/23 09:23:11 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Close file descriptors and reset them to -2 (default value).
**	Prevents fd leaks by resetting 'fd_in' and 'fd_out' to '-2'
**	(a convention for "unused" in our Minishell project).
*/


void	cleanup_fds(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->fd_in > 2)
	{
		close(cmd->fd_in);
		cmd->fd_in = -2;
	}
	if (cmd->fd_out > 2)
	{
		close(cmd->fd_out);
		cmd->fd_out = -2;
	}
}

/*
**	Clean up the shell struct and free all allocated memory.
**	- I guess we can add more cleanup functions here if needed.
*/

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_allocs(&shell->alloc_tracker);
}
