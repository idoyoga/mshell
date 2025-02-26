/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:26:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/26 02:41:22 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// effectively the same as fork_and_exec()
// TODO: unify into one function
static void	execute_without_pipeline(t_shell *shell)
{
	shell->cmd->child_pid = fork();
	if (shell->cmd->child_pid == -1)
		error_exit(shell, NO_FORK, "execute_without_pipeline", EXIT_FAILURE);
	if (shell->cmd->child_pid == 0)
	{
		execute_command(shell, shell->cmd);
	}
	shell->status = wait_for_children(shell, 1);
}

// entrypoint for any execution
void	dispatch(t_shell *shell, size_t cmd_count)
{
	t_b_typ	type;

	print_parsed_data(shell);
	if (cmd_count != 1)
	{
		execute_with_pipeline(shell, shell->cmd, cmd_count);
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
