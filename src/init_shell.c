/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:12:50 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/06 18:54:35 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
 * Initialize shell struct with environment variables and working directory,
 * also allocates memory for shell->allocs (list of pointers for all allocs,
 * easy to free them all later).
*/

bool	init_shell(t_shell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->allocs = ft_calloc(MAX_ALLOCS, sizeof(void *));
	if (!shell->allocs)
		return (error(shell, NO_MEM, 1));
	shell->env = init_env(shell, env);
	if (!shell->env)
		return (error(shell, NO_ENV, 1));
	if (init_wd(shell))
		return (error(shell, NO_WD, 1));
	return (false);
}
