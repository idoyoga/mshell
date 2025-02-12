/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:26:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/12 21:38:45 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	cmd_lstlen(t_cmd *cmd)
{
	size_t	i;
	t_cmd	*current;

	if (cmd == NULL)
		return (0);
	i = 1;
	current = cmd->next;
	while (current != cmd)
	{
		i++;
		current = current->next;
	}
	return (i);
}

static void	execute_without_pipeline(t_shell *shell)
{
	(void)shell;
}

static void	execute_with_pipeline(t_shell *shell, size_t cmd_count)
{
	size_t	i;

	(void)shell;
	i = 0;
	while (i < cmd_count)
	{
		i++;
	}
}

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

// after copying fds, try opening any files (in or out)
// manage file redirections and return an error if anything breaks
// then run the builtin
static void	execute_single_builtin(t_shell *shell, t_b_typ type)
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

// run single builtins without forking
// if receiving multiple commands,
// fork and execute for each command
void	execute(t_shell *shell)
{
	size_t	cmd_count;
	t_b_typ	type;

	cmd_count = cmd_lstlen(shell->cmd);
	if (cmd_count != 1)
		return (execute_with_pipeline(shell, cmd_count));
	type = identify_builtin(shell->cmd->args[0]);
	if (type == _NOT_A_BUILTIN)
		return (execute_without_pipeline(shell));
	return (execute_single_builtin(shell, type));
}
