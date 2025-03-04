/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:12 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 19:09:37 by dplotzl          ###   ########.fr       */
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
	t_cmd	*current;

	if (!cmd)
		return ;
	current = cmd;
	while (current)
	{
		if (current->fd_in > 2)
		{
			close(current->fd_in);
			current->fd_in = -2;
		}
		if (current->fd_out > 2)
		{
			close(current->fd_out);
			current->fd_out = -2;
		}
		current = current->next;
		if (current == cmd)
			break ;
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
	cleanup_fds(shell->cmd);
	if (shell->fd_copies[STDIN_FILENO] != -2)
		close(shell->fd_copies[STDIN_FILENO]);
	if (shell->fd_copies[STDOUT_FILENO] != -2)
		close(shell->fd_copies[STDOUT_FILENO]);
	if (shell->alloc_tracker.allocs)
		free_allocs(&shell->alloc_tracker);
}
