/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:51:59 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 21:10:53 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fds should definitely be valid here
// also close STDOUT_COPY when first dup2 fails?
// what could cause it to fail, what's the situation then
// and can we recover at all?
// TODO: inspect
static void	reset_io(t_shell *shell, int *fd_copies)
{
	if (dup2(fd_copies[STDIN_FILENO], STDIN_FILENO) == -1)
	{
		close(fd_copies[STDIN_FILENO]);
		close(fd_copies[STDOUT_FILENO]);
		error_exit(shell, NO_DUP2, "reset_io", EXIT_FAILURE);
	}
	close(fd_copies[STDIN_FILENO]);
	fd_copies[STDIN_FILENO] = -2;
	if (dup2(fd_copies[STDOUT_FILENO], STDOUT_FILENO) == -1)
	{
		close(fd_copies[STDOUT_FILENO]);
		error_exit(shell, NO_DUP2, "reset_io", EXIT_FAILURE);
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

static void	link_fds_for_builtin(t_shell *shell)
{
	if (shell->cmd->fd_in > 2)
	{
		if (dup2(shell->cmd->fd_in, STDIN_FILENO) == -1)
		{
			close(shell->fd_copies[STDIN_FILENO]);
			close(shell->fd_copies[STDOUT_FILENO]);
			error_exit(shell, NO_DUP2, "execute_single_builtin", EXIT_FAILURE);
		}
		close(shell->cmd->fd_in);
	}
	if (shell->cmd->fd_out > 2)
	{
		if (dup2(shell->cmd->fd_out, STDOUT_FILENO) == -1)
		{
			close(shell->fd_copies[STDIN_FILENO]);
			close(shell->fd_copies[STDOUT_FILENO]);
			error_exit(shell, NO_DUP2, "execute_single_builtin", EXIT_FAILURE);
		}
		close(shell->cmd->fd_out);
	}
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
		error_exit(shell, NO_DUP, "execute_single_builtin", EXIT_FAILURE);
	shell->fd_copies[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (shell->fd_copies[STDOUT_FILENO] == -1)
	{
		close(shell->fd_copies[STDIN_FILENO]);
		error_exit(shell, NO_DUP, "execute_single_builtin", EXIT_FAILURE);
	}
	link_fds_for_builtin(shell);
	builtin = get_builtin(type);
	if (builtin != NULL)
		builtin(shell, shell->cmd);
	reset_io(shell, shell->fd_copies);
}
