/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:54:23 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 18:56:19 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <signal.h>

static void	reset_fd(int *fd)
{
	if (*fd != -2)
	{
		close(*fd);
		*fd = -2;
	}
}

static void	link_the_child(int *prev_fd, int *pipe_fds, bool is_not_last,
		t_shell *shell)
{
	bool	success;

	success = true;
	if (*prev_fd != -2 && dup2(*prev_fd, STDIN_FILENO) == -1)
		success = false;
	if (success && is_not_last && dup2(pipe_fds[1], STDOUT_FILENO) == -1)
		success = false;
	(reset_fd(pipe_fds), reset_fd(pipe_fds + 1), reset_fd(prev_fd));
	if (!success)
		error_exit(shell, NO_DUP2, "(child) dup2", EXIT_FAILURE);
}

static void	fork_process(t_shell *shell, t_cmd *command, int *prev_fd,
		int *pipe_fd)
{
	command->child_pid = fork();
	if (command->child_pid == -1)
	{
		reset_fd(prev_fd);
		reset_fd(pipe_fd);
		reset_fd(pipe_fd + 1);
		error_exit(shell, NO_FORK, "fork", EXIT_FAILURE);
	}
}

void	execute_with_pipeline(t_shell *shell, t_cmd *command, int *pipe_fd)
{
	int		prev_fd;
	size_t	i;

	prev_fd = -2;
	i = 0;
	while (i < shell->cmd_count)
	{
		if (i < shell->cmd_count - 1 && pipe(pipe_fd) == -1)
			(reset_fd(&prev_fd), error_exit(shell, NO_PIPE, NULL,
					EXIT_FAILURE));
		fork_process(shell, command, &prev_fd, pipe_fd);
		if (command->child_pid == 0)
		{
			link_the_child(&prev_fd, pipe_fd, i < shell->cmd_count - 1, shell);
			execute_command(shell, command);
		}
		(reset_fd(&command->fd_in), reset_fd(&command->fd_out));
		(reset_fd(&prev_fd), reset_fd(pipe_fd + 1));
		prev_fd = pipe_fd[0];
		pipe_fd[0] = -2;
		command = command->next;
		i++;
	}
	shell->status = wait_for_children(shell, shell->cmd);
}
