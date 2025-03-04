/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:02:58 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 19:12:10 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_acc_t	get_file_access_status(const char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == -1)
		return (A_NOT_FOUND);
	if (S_ISDIR(file_stat.st_mode))
		return (A_IS_DIRECTORY);
	if (access(path, X_OK) == -1)
		return (A_PERMISSION_DENIED);
	if (!(file_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
		return (A_PERMISSION_DENIED);
	return (A_CAN_EXECUTE);
}

static bool	handle_path_builtin_access_status(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0] || cmd->skip)
	{
		cmd->cmd = NULL;
		cmd->access_status = A_NOOP;
		return (true);
	}
	if (identify_builtin(cmd->args[0]) != _NOT_A_BUILTIN)
	{
		cmd->access_status = A_CAN_EXECUTE;
		cmd->cmd = safe_strdup(shell, cmd->args[0]);
		return (true);
	}
	if (shell->path_segments == NULL || cmd->args[0][0] == '\0'
		|| is_path(cmd->args[0]))
	{
		cmd->access_status = get_file_access_status(cmd->args[0]);
		cmd->cmd = safe_strdup(shell, cmd->args[0]);
		return (true);
	}
	return (false);
}

// get_env_value doesn't allocate, it only returns a pointer
// to the string inside of the matching env variable
static void	find_absolute_path(t_shell *shell, t_cmd *cmd)
{
	size_t	i;

	if (handle_path_builtin_access_status(shell, cmd))
		return ;
	i = 0;
	while (shell->path_segments[i] != NULL && cmd->args != NULL)
	{
		cmd->cmd = ft_strjoin_three(shell->path_segments[i], "/", cmd->args[0]);
		if (!cmd->cmd)
			error_exit(shell, NO_MEM, "ft_strjoin_three", EXIT_FAILURE);
		if (access(cmd->cmd, F_OK) == 0)
		{
			alloc_tracker_add(&shell->alloc_tracker, cmd->cmd, 0);
			cmd->access_status = get_file_access_status(cmd->cmd);
			return ;
		}
		free(cmd->cmd);
		i++;
	}
	cmd->access_status = A_NOT_FOUND;
	cmd->cmd = safe_strdup(shell, "");
}

void	get_absolute_paths(t_shell *shell)
{
	size_t	i;
	t_cmd	*current_command;

	i = 0;
	current_command = shell->cmd;
	while (i < shell->cmd_count)
	{
		find_absolute_path(shell, current_command);
		current_command = current_command->next;
		i++;
	}
}
