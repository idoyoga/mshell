/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:54:23 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/20 18:43:328 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <signal.h>

void	redirect_command(t_shell *shell, t_cmd *command)
{
	if (command->fd_in > 2)
	{
		if (dup2(command->fd_in, STDIN_FILENO) == -1)
		{
			// close all pipes here
			error_exit(shell, NO_DUP2, "redirect_command", EXIT_FAILURE);
		}
		close(command->fd_in);
		command->fd_in = -2;
	}
	if (command->fd_out > 2)
	{
		if (dup2(command->fd_out, STDOUT_FILENO) == -1)
		{
			// close all pipes here
			error_exit(shell, NO_DUP2, "redirect_command", EXIT_FAILURE);
		}
		close(command->fd_out);
		command->fd_out = -2;
	}
	// redirect from infile to outfile
	// when do I need to pay attention to what,
	// and do things in a different way?
}

void	execute_command(t_shell *shell, t_cmd *command)
{
	t_b_typ	type;

	// files should already be open, so we can just redirect
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirect_command(shell, command);
	type = identify_builtin(command->args[0]);
	if (type > _NOT_A_BUILTIN)
		execute_builtin(shell, type);
	shell->status = execve(command->cmd, command->args, shell->env_as_array);
	printf("execve failed in execute_command! errno is %d\n", errno);
	error_exit(shell, BAD_EXEC, "execute_command", shell->status);
}

int	wait_for_children(t_shell *shell, size_t cmd_count)
{
	int		status_code;
	t_cmd	*current_cmd;
	size_t	i;
	pid_t	wait_result;

	status_code = 0;
	i = 0;
	current_cmd = shell->cmd;
	wait_result = -1;
	while (i < cmd_count)
	{
		while (true)
		{
			wait_result = waitpid(current_cmd->child_pid, &status_code, 0);
			if (wait_result != -1)
				break ;
			if (errno != EINTR)
			{
				// close all files / pipes,
				// child processes should receive SIGPIPE
				error_exit(shell, BAD_WAIT, "waitpid", EXIT_FAILURE);
			}
		}
		if (WIFEXITED(status_code))
			status_code = WEXITSTATUS(status_code);
		else if (WIFSIGNALED(status_code))
			status_code = WTERMSIG(status_code) + 128;
		current_cmd = current_cmd->next;
		i++;
	}
	return (status_code);
}

static bool	fork_and_exec(t_shell *shell, t_cmd *cmd)
{
	cmd->child_pid = fork();
	if (cmd->child_pid == -1)
		return (false);
	if (cmd->child_pid == 0)
	{
		execute_command(shell, cmd);
	}
	printf("sent child off to execute %s\n", cmd->cmd);
	return (true);
}

static void	reset_fds(int *fds, size_t count)
{
	size_t	i;

	if (!fds)
		return ;
	i = 0;
	while (i < count)
	{
		if (fds[i] != -2)
			close(fds[i]);
		fds[i] = -2;
		i++;
	}
}

static void	plumb(int *target_fd, int *source_fd)
{
	if (*target_fd == -2)
		*target_fd = *source_fd;
	else
		reset_fds(source_fd, 1);
}

void	execute_with_pipeline(t_shell *shell, t_cmd *command, size_t cmd_count)
{
	int		pipe_fds[2];
	size_t	i;

	pipe_fds[0] = -2;
	pipe_fds[1] = -2;
	i = 0;
	while (i < cmd_count)
	{
		if (plumb(&command->fd_in, pipe_fds + 0), i < cmd_count)
		{
			if (pipe(pipe_fds) == -1)
			{
				// TODO: actually handle error
				reset_fds(pipe_fds, 2), reset_fds(&command->fd_in, 1);
				error_exit(shell, NO_PIPE, "pipeline", EXIT_FAILURE);
			}
			plumb(&command->fd_out, pipe_fds + 1);
		}
		if (!fork_and_exec(shell, command))
		{
			reset_fds(pipe_fds, 2), reset_fds(&command->fd_in, 1);
			error_exit(shell, NO_FORK, "fork_and_exec", EXIT_FAILURE);
		}
		reset_fds(pipe_fds + 1, 1), reset_fds(&command->fd_in, 1);
		command = command->next; // ?
		i++;
	}
	shell->status = wait_for_children(shell, cmd_count);
}
