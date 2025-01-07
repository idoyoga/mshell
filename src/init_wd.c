/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_wd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 15:51:52 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/06 18:54:35 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
 * Get the value of an environment variable
*/

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
 * Initialize work_dir (PWD) and old_work_dir (OLDPWD), should also work with
 * incomplete environment (PWD unset). Used also for cd builtin later.
*/

bool	init_wd(t_shell *shell)
{
	char	*cwd;
	char	*oldpwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (error(shell, GETCWD, 1));
	shell->work_dir = ft_strdup(cwd);
	shell->allocs[shell->alloc_count++] = shell->work_dir;
	free(cwd);
	if (!shell->work_dir)
		return (error(shell, NO_MEM, 1));
	oldpwd = get_env_value(shell->env, "OLDPWD");
	if (oldpwd)
		shell->old_work_dir = ft_strdup(oldpwd);
	else
		shell->old_work_dir = ft_strdup(shell->work_dir);
	shell->allocs[shell->alloc_count++] = shell->old_work_dir;
	if (!shell->old_work_dir)
		return (error(shell, NO_MEM, 1));
	printf("work_dir: %s\n", shell->work_dir); // debug
	return (false);
}
