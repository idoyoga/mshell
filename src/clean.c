/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 11:11:54 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/07 14:28:16 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* 
 * Free single env node
*/

void	free_env_node(t_env **node)
{
	if (!node || !*node)
		return ;
	free((*node)->key);
	free((*node)->value);
	free(*node);
	*node = NULL;
}

/*
 * Free env list in case create_env_node fails midway through init_env.
 * This function is also called in clean_shell.
*/

void	free_env_list(t_env **env_list)
{
	t_env	*current;
	t_env	*next;

	if (!env_list || !*env_list)
		return ;
	current = *env_list;
	while (current)
	{
		next = current->next;
		free_env_node(&current);
		current = next;
	}
	*env_list = NULL;
}
/*
 * Free all allocated memory in shell struct, including special case for env
*/

static void	free_allocs(t_shell *shell)
{
	int	i;

	i = -1;
	while (++i < shell->alloc_count)
	{
		if (shell->allocs[i])
		{
			if (shell->allocs[i] == (void *)shell->env)
				free_env_list((t_env **)&shell->allocs[i]);
			else
				ft_free(&shell->allocs[i]);
		}
	}
	free(shell->allocs);
	shell->allocs = NULL;
	shell->alloc_count = 0;
}

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_allocs(shell);
}
