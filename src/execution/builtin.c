/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:51:59 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/12 21:53:06 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fds should definitely be valid here
// also close STDOUT_COPY when first dup2 fails?
// what could cause it to fail, what's the situation then
// and can we recover at all?
static void	reset_io(int *fd_copies)
{
	if (dup2(fd_copies[STDIN_FILENO], STDIN_FILENO) == -1)
	{
		close(fd_copies[STDIN_FILENO]);
		return (perror("reset_stdio: couldn't reset STDIN"));
	}
	close(fd_copies[STDIN_FILENO]);
	if (dup2(fd_copies[STDOUT_FILENO], STDOUT_FILENO) == -1)
	{
		close(fd_copies[STDOUT_FILENO]);
		return (perror("reset_stdio: couldn't reset STDOUT"));
	}
	close(fd_copies[STDOUT_FILENO]);
}

void	execute_single_builtin(t_shell *shell, t_b_typ type)
{
	int		fd_copies[2];
	void	(*builtin)(t_shell *);

	fd_copies[STDIN_FILENO] = dup(STDIN_FILENO);
	if (!fd_copies[STDIN_FILENO])
		return (perror("execute_single_builtin: could not duplicate STDIN"));
	fd_copies[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (!fd_copies[STDOUT_FILENO])
		return (perror("execute_single_builtin: could not duplicate STDOUT"));
	if (shell->cmd->fd_in >= 0)
	{
		// should probably close fd_copies if these fail
		if (dup2(shell->cmd->fd_in, STDIN_FILENO) == -1)
			return (close(shell->cmd->fd_in), perror("execute_single_builtin: dup2 to STDIN failed"));
		close(shell->cmd->fd_in);
	}
	if (shell->cmd->fd_out >= 0)
	{
		// should we close fd_in as well if it's set, and this dup2 fails?
		if (dup2(shell->cmd->fd_out, STDOUT_FILENO) == -1)
			return (close(shell->cmd->fd_out), perror("execute_single_builtin: dup2 to STDOUT failed"));
		close(shell->cmd->fd_out);
	}
	builtin = get_builtin(type);
	if (builtin != NULL)
		builtin(shell);
	reset_io(fd_copies);
}
