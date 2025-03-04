/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:51:59 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 03:19:27 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fds should definitely be valid here
// also close STDOUT_COPY when first dup2 fails?
// what could cause it to fail, what's the situation then
// and can we recover at all?
// TODO: inspect
static void	reset_io(int *fd_copies)
{
	if (dup2(fd_copies[STDIN_FILENO], STDIN_FILENO) == -1)
	{
		// close all unnecessary garbage
		close(fd_copies[STDIN_FILENO]);
		fd_copies[STDIN_FILENO] = -2;
		return (perror("reset_stdio: couldn't reset STDIN"));
	}
	close(fd_copies[STDIN_FILENO]);
	fd_copies[STDIN_FILENO] = -2;
	if (dup2(fd_copies[STDOUT_FILENO], STDOUT_FILENO) == -1)
	{
		// also, reset io
		close(fd_copies[STDOUT_FILENO]);
		fd_copies[STDOUT_FILENO] = -2;
		return (perror("reset_stdio: couldn't reset STDOUT"));
	}
	close(fd_copies[STDOUT_FILENO]);
	fd_copies[STDOUT_FILENO] = -2;
}

void	execute_builtin(t_shell *shell, t_cmd *command, t_b_typ type)
{
	void	(*builtin)(t_shell *, t_cmd *);

	builtin = get_builtin(type);
	if (builtin != NULL)
		builtin(shell, command);
	clean_shell(shell);
	exit(shell->status);
}

void	execute_single_builtin(t_shell *shell, t_b_typ type)
{
	void	(*builtin)(t_shell *, t_cmd *);

	if (shell->cmd->skip)
	{
		shell->status = 1;
		return ;
	}
	shell->fd_copies[STDIN_FILENO] = dup(STDIN_FILENO);
	if (shell->fd_copies[STDIN_FILENO] == -1)
	{
		// ensure that fd_in and fd_out for cmd are closed here
		// also, reset io
		return (perror("execute_single_builtin: could not duplicate STDIN"));
	}
	shell->fd_copies[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (shell->fd_copies[STDOUT_FILENO] == -1)
	{
		// ensure that fd_in and fd_out for cmd are closed here
		// also, reset io
		close(shell->fd_copies[STDIN_FILENO]);
		return (perror("execute_single_builtin: could not duplicate STDOUT"));
	}
	if (shell->cmd->fd_in > 2)
	{
		// should probably close fd_copies if these fail
		if (dup2(shell->cmd->fd_in, STDIN_FILENO) == -1)
		{
			// close(shell->cmd->fd_in);
			close(shell->fd_copies[STDIN_FILENO]);
			close(shell->fd_copies[STDOUT_FILENO]);
			return (perror("execute_single_builtin: dup2 to STDIN failed"));
		}
		close(shell->cmd->fd_in);
	}
	if (shell->cmd->fd_out > 2)
	{
		// should we close fd_in as well if it's set, and this dup2 fails?
		if (dup2(shell->cmd->fd_out, STDOUT_FILENO) == -1)
		{
			close(shell->fd_copies[STDIN_FILENO]);
			close(shell->fd_copies[STDOUT_FILENO]);
			// close(shell->cmd->fd_out);
			return (perror("execute_single_builtin: dup2 to STDOUT failed"));
		}
		close(shell->cmd->fd_out);
	}
	builtin = get_builtin(type);
	if (builtin != NULL)
		builtin(shell, shell->cmd);
	reset_io(shell->fd_copies);
}
