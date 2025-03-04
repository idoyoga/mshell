/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:26:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 19:30:17 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redirect_command(t_shell *shell, t_cmd *command)
{
	if (command->fd_in > 2)
	{
		if (dup2(command->fd_in, STDIN_FILENO) == -1)
			error_exit(shell, NO_DUP2, "redirect_command", EXIT_FAILURE);
		close(command->fd_in);
		command->fd_in = -2;
	}
	if (command->fd_out > 2)
	{
		if (dup2(command->fd_out, STDOUT_FILENO) == -1)
			error_exit(shell, NO_DUP2, "redirect_command", EXIT_FAILURE);
		close(command->fd_out);
		command->fd_out = -2;
	}
}

static void	handle_access_status_error(t_shell *shell, t_cmd *cmd,
		t_acc_t status)
{
	if (status == A_NOOP)
	{
		clean_shell(shell);
		exit(shell->status);
	}
	if (status == A_NOT_FOUND)
	{
		shell->status = 127;
		if (is_path(cmd->args[0]))
			error_exit_s(shell, EXEC_NO_FILE, cmd->cmd, shell->status);
		error_exit_s(shell, EXEC_NOT_FOUND, cmd->args[0], shell->status);
	}
	if (status == A_IS_DIRECTORY)
	{
		shell->status = 126;
		error_exit_s(shell, EXEC_IS_DIRECTORY, cmd->cmd, shell->status);
	}
	if (status == A_PERMISSION_DENIED)
	{
		shell->status = 126;
		error_exit_s(shell, EXEC_PERMISSION_DENIED, cmd->cmd, shell->status);
	}
}

void	execute_command(t_shell *shell, t_cmd *command)
{
	t_b_typ	type;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirect_command(shell, command);
	handle_access_status_error(shell, command, command->access_status);
	type = identify_builtin(command->args[0]);
	if (type > _NOT_A_BUILTIN)
		execute_builtin(shell, command, type);
	shell->status = execve(command->cmd, command->args, shell->env_as_array);
	error_exit_s(shell, BAD_EXEC, command->cmd, shell->status);
}

static void	execute_without_pipeline(t_shell *shell)
{
	if (shell->cmd->skip)
	{
		shell->status = 1;
		return ;
	}
	shell->cmd->child_pid = fork();
	if (shell->cmd->child_pid == -1)
		error_exit(shell, NO_FORK, "execute_without_pipeline", EXIT_FAILURE);
	if (shell->cmd->child_pid == 0)
	{
		execute_command(shell, shell->cmd);
	}
	shell->status = wait_for_children(shell, shell->cmd);
}

// entrypoint for any execution
void	dispatch(t_shell *shell)
{
	int		pipe_fd[2];
	t_b_typ	type;

	pipe_fd[0] = -2;
	pipe_fd[1] = -2;
	prepare_execution(shell);
	if (shell->cmd_count != 1)
	{
		execute_with_pipeline(shell, shell->cmd, pipe_fd);
		return ;
	}
	if (!shell->cmd->args)
		return ;
	type = identify_builtin(shell->cmd->args[0]);
	if (type == _NOT_A_BUILTIN)
	{
		execute_without_pipeline(shell);
		return ;
	}
	execute_single_builtin(shell, type);
}
