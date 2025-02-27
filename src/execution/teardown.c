/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teardown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:55:34 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/27 20:48:36 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_path_segments(t_shell *shell)
{
	size_t	i;

	i = 0;
	if (shell->path_segments != NULL)
	{
		alloc_tracker_remove(&shell->alloc_tracker, shell->path_segments);
		while (shell->path_segments[i] != NULL)
		{
			free(shell->path_segments[i++]);
		}
		free(shell->path_segments);
		shell->path_segments = NULL;
	}
}

static void	clean_env_array(t_shell *shell)
{
	size_t	i;

	i = 0;
	if (shell->env_as_array != NULL)
	{
		alloc_tracker_remove(&shell->alloc_tracker, shell->env_as_array);
		while (shell->env_as_array[i] != NULL)
		{
			alloc_tracker_remove(&shell->alloc_tracker, shell->env_as_array[i]);
			free(shell->env_as_array[i++]);
		}
		free(shell->env_as_array);
		shell->env_as_array = NULL;
	}
}

static void	clean_absolute_paths(t_shell *shell)
{
	size_t	i;
	t_cmd	*current_command;

	i = 0;
	current_command = shell->cmd;
	while (i < shell->cmd_count)
	{
		alloc_tracker_remove(&shell->alloc_tracker, current_command->cmd);
		free(current_command->cmd);
		current_command->cmd = NULL;
		current_command = current_command->next;
		i++;
	}
}

void	postpare_execution(t_shell *shell)
{
	clean_path_segments(shell);
	clean_env_array(shell);
	clean_absolute_paths(shell);
	cleanup_fds(shell->cmd);
}
