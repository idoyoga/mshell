/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:55:42 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/27 13:23:09 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// get_env_value doesn't allocate, it only returns a pointer
// to the string inside of the matching env variable
static void	find_absolute_path(t_shell *shell, t_cmd *cmd)
{
	size_t	i;

	if (shell->path_segments == NULL || (cmd->args != NULL && (cmd->args[0]
				&& (cmd->args[0][0] == '\0' || is_path(cmd->args[0])))))
	{
		cmd->cmd = safe_strdup(shell, cmd->args[0]);
		return ;
	}
	i = 0;
	while (shell->path_segments[i] != NULL && cmd->args != NULL)
	{
		cmd->cmd = ft_strjoin_three(shell->path_segments[i], "/", cmd->args[0]);
		if (!cmd)
			error_exit(shell, NO_MEM, "ft_strjoin_three", EXIT_FAILURE);
		if (access(cmd->cmd, F_OK) == 0)
		{
			alloc_tracker_add(&shell->alloc_tracker, cmd->cmd, 0);
			return ;
		}
		free(cmd->cmd);
		i++;
	}
	cmd->cmd = safe_strdup(shell, "");
}

static void	get_absolute_paths(t_shell *shell, size_t cmd_count)
{
	size_t	i;
	t_cmd	*current_command;

	i = 0;
	current_command = shell->cmd;
	while (i < cmd_count)
	{
		find_absolute_path(shell, current_command);
		current_command = current_command->next;
		i++;
	}
}

static void	set_path_segments(t_shell *shell)
{
	char	*path_variable;

	path_variable = get_env_value(shell, "PATH");
	if (path_variable == NULL)
	{
		shell->path_segments = NULL;
		return ;
	}
	shell->path_segments = ft_split(path_variable, ':');
	if (!shell->path_segments)
		error_exit(shell, NO_ALLOC, "prepare_execution", EXIT_FAILURE);
	alloc_tracker_add(&shell->alloc_tracker, shell->path_segments, 1);
}

static void	set_env_as_array(t_shell *shell)
{
	shell->env_as_array = get_env_array(shell);
	if (!shell->env_as_array)
		error_exit(shell, NO_ALLOC, "get_env_array", EXIT_FAILURE);
}

// minimize allocations in the execution flow for easier error handling
// by front-loading anything we can before going too deep into exec
// if no PATH set, return and use cwd instead
// if anything goes wrong, most likely just a malloc fail,
// error_exit() is called.
void	prepare_execution(t_shell *shell, size_t cmd_count)
{
	set_path_segments(shell);
	get_absolute_paths(shell, cmd_count);
	set_env_as_array(shell);
}
