/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:55:42 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/03 17:15:30 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// get_env_value doesn't allocate, it only returns a pointer
// to the string inside of the matching env variable
static void	find_absolute_path(t_shell *shell, t_cmd *cmd)
{
	char	*tmp;
	size_t	i;

	if (shell->path_segments == NULL || (cmd->args != NULL && (cmd->args[0]
				&& (cmd->args[0][0] == '\0' || is_path(cmd->args[0])))))
	{
		if (cmd->cmd)
			alloc_tracker_remove(&shell->alloc_tracker, cmd->cmd);
		cmd->cmd = safe_strdup(shell, cmd->args[0]);
		return ;
	}
	i = 0;
	while (shell->path_segments[i] != NULL && cmd->args != NULL)
	{
		tmp = ft_strjoin_three(shell->path_segments[i], "/", cmd->args[0]);
		if (!tmp)
			error_exit(shell, NO_MEM, "ft_strjoin_three", EXIT_FAILURE);
		if (cmd->cmd)
			alloc_tracker_remove(&shell->alloc_tracker, cmd->cmd);
		cmd->cmd = safe_strdup(shell, tmp);
		free (tmp);
		if (access(cmd->cmd, F_OK) == 0)
			return ;
		i++;
	}
	if (cmd->cmd)
		alloc_tracker_remove(&shell->alloc_tracker, cmd->cmd);
	cmd->cmd = safe_strdup(shell, "");
}

static void	get_absolute_paths(t_shell *shell)
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
	alloc_tracker_add(&shell->alloc_tracker, shell->path_segments, 1, 0);
}

static void	set_env_as_array(t_shell *shell)
{
	char	**new_env_array;

	printf("🔵 set_env_as_array() called! Current env_as_array: %p\n",
		shell->env_as_array);
	new_env_array = get_env_array(shell);
	if (!new_env_array)
		error_exit(shell, NO_ALLOC, "set_env_as_array", EXIT_FAILURE);
	printf("🔍 Old env_as_array: %p\n", shell->env_as_array);
	if (shell->env_as_array)
	{
		printf("🔍 Attempting to remove old env_as_array: %p\n", shell->env_as_array);
		alloc_tracker_remove(&shell->alloc_tracker, shell->env_as_array);
		free(shell->env_as_array);
		shell->env_as_array = NULL;
	}
	printf("✅ Setting new env_as_array: %p\n", shell->env_as_array);
	shell->env_as_array = new_env_array;
	alloc_tracker_add(&shell->alloc_tracker, shell->env_as_array, 1, 0);
}

static void	set_cmd_count(t_shell *shell)
{
	t_cmd	*current_cmd;
	size_t	count;

	count = 0;
	current_cmd = shell->cmd;
	while (current_cmd)
	{
		count++;
		current_cmd = current_cmd->next;
		if (current_cmd == shell->cmd)
			break ;
	}
	shell->cmd_count = count;
}

// minimize allocations in the execution flow for easier error handling
// by front-loading anything we can before going too deep into exec
// if no PATH set, return and use cwd instead
// if anything goes wrong, most likely just a malloc fail,
// error_exit() is called.
void	prepare_execution(t_shell *shell)
{
	set_cmd_count(shell);
	set_path_segments(shell);
	get_absolute_paths(shell);
	set_env_as_array(shell);
}
