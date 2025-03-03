/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:26:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/03 00:49:19 by xgossing         ###   ########.fr       */
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

void	execute_command(t_shell *shell, t_cmd *command)
{
	t_b_typ	type;

	shell->status = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirect_command(shell, command);
	if (command->access_status == A_NOOP)
	{
		ft_putendl_fd("nooping the heck out",2);
		clean_shell(shell);
		exit(0);
	}
	if (command->access_status == A_NOT_FOUND)
	{
		shell->status = 127;
		error_exit(shell, EXEC_NOT_FOUND, "execute_command", shell->status);
	}
	if (command->access_status == A_IS_DIRECTORY)
	{
		shell->status = 126;
		error_exit(shell, EXEC_IS_DIRECTORY, "execute_command", shell->status);
	}
	if (command->access_status == A_PERMISSION_DENIED)
	{
		shell->status = 126;
		error_exit(shell, EXEC_PERMISSION_DENIED, "execute_command",
			shell->status);
	}
	type = identify_builtin(command->args[0]);
	if (type > _NOT_A_BUILTIN)
		execute_builtin(shell, command, type);
	shell->status = execve(command->cmd, command->args, shell->env_as_array);
	printf("execve failed in execute_command! errno is %d\n", errno);
	error_exit(shell, BAD_EXEC, "execute_command", shell->status);
}

// effectively the same as fork_and_exec()
// TODO: unify into one function
static void	execute_without_pipeline(t_shell *shell)
{
	if (shell->cmd->skip)
		return ;
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

	shell->status = 0;
	printf("cmd count: %lu\n", shell->cmd_count);
	if (shell->cmd_count != 1)
	{
		pipe_fd[0] = -2;
		pipe_fd[1] = -2;
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
