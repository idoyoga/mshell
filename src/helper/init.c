/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:12:50 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/19 22:54:04 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Initialize allocation tracker and adapt initial capacity based on
** the environment size.
*/

static bool	init_alloc_tracker(t_shell *shell, int initial_capacity)
{
	if (!shell || initial_capacity <= 0)
		return (false);
	shell->alloc_tracker.allocs = ft_calloc(initial_capacity, sizeof(void *));
	shell->alloc_tracker.is_array = ft_calloc(initial_capacity, sizeof(int));
	if (!shell->alloc_tracker.allocs || !shell->alloc_tracker.is_array)
	{
		free(shell->alloc_tracker.allocs);
		free(shell->alloc_tracker.is_array);
		shell->alloc_tracker.allocs = NULL;
		shell->alloc_tracker.is_array = NULL;
		error_exit(shell, NO_ALLOC, EXIT_FAILURE);
	}
	shell->alloc_tracker.count = 0;
	shell->alloc_tracker.capacity = initial_capacity;
	shell->alloc_tracker.initialized = true;
	return (true);
}

/*
** Initialize work_dir (PWD) and old_work_dir (OLDPWD), should also work with
** incomplete environment (PWD unset). Used also for cd builtin later.
*/

static bool	init_work_dirs(t_shell *shell)
{
	char	*cwd;
	char	*tmp;

	tmp = safe_strdup(shell, "OLDPWD");
	if (!tmp)
		return (error(NO_MEM, false));
	if (!add_env_variable(shell, &shell->env, tmp))
		return (error(NO_MEM, false));
	cwd = getcwd(NULL, 0);
	if (!cwd || !alloc_tracker_add(&shell->alloc_tracker, cwd, 0))
		return (error(GETCWD, false));
	tmp = safe_strjoin(shell, "PWD=", cwd);
	if (!tmp)
		return (error(NO_MEM, false));
	if (!add_env_variable(shell, &shell->env, tmp))
		return (error(NO_MEM, false));
	shell->work_dir = safe_strdup(shell, cwd);
	if (!shell->work_dir)
		return (error(NO_MEM, false));
	shell->old_work_dir = safe_strdup(shell, "");
	if (!shell->old_work_dir)
		return (error(NO_MEM, false));
	return (true);
}

/*
** Initialize the environment variables, if no environment is passed,
** return an error and exit with status false. Otherwise, create a new list 
** and add all environment variables to it. 
*/

static bool	init_env(t_shell *shell, char **env)
{
	t_env	*lst;
	char	*tmp;
	int		i;

	if (!env || !(*env))
		return (error(NO_ENV, false));
	lst = NULL;
	i = -1;
	while (env[++i])
	{
		tmp = safe_strdup(shell, env[i]);
		if (!tmp)
			error_exit(shell, NO_MEM, EXIT_FAILURE);
		if (!add_env_variable(shell, &lst, tmp))
			return (error(NO_MEM, false));
	}
	shell->env = lst;
	shell->env_count = i;
	return (true);
}

/*
** Set the user name and create the prompt string. If the user name is not set,
** return an error and exit with status false.
*/

static bool	init_prompt(t_shell *shell)
{
	shell->user = getenv("USER");
	if (!shell->user)
		return (error(NO_USER, false));
	if (!shell->work_dir)
		return (error(NO_WD, false));
	shell->prompt = create_prompt(shell);
	if (!shell->prompt)
		return (error(NO_PROMPT, false));
	return (true);
}

/*
** Initialize shell struct with allocation tracker, environment variables 
** and working directory
*/

bool	init_shell(t_shell *shell, char **env)
{
	int	initial_capacity;
	int	i;

	ft_memset(shell, 0, sizeof(t_shell));
	i = 0;
	if (env)
	{
		while (env[i])
			i++;
		initial_capacity = (i * 2);
	}
	else
		initial_capacity = DEFAULT_ALLOC_CAPACITY;
	if (!init_alloc_tracker(shell, initial_capacity))
		return (error(NO_ALLOC, false));
	if (!init_work_dirs(shell))
		return (error(NO_WD, false));
	if (!init_env(shell, env))
		return (error(NO_ENV, false));
	if (!init_prompt(shell))
		return (error(NO_PROMPT, false));
	return (true);
}
