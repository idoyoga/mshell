/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:55:42 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 19:03:13 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
