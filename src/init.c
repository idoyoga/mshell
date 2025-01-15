/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:12:50 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/15 17:27:30 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** Initialize allocation tracker and adapt initial capacity based on
** the environment size.
*/

static bool	init_alloc_tracker(t_alloc *tracker, char **env)
{
	int	i;
	int	initial_capacity;

	i = 0;
	if (env)
	{
		while (env[i])
			i++;
		initial_capacity = i * 2;
	}
	else
		initial_capacity = DEFAULT_ALLOC_CAPACITY;
	if (!tracker || initial_capacity <= 0)
		return (false);
	tracker->allocs = ft_calloc(initial_capacity, sizeof(void *));
	if (!tracker->allocs)
		return (error(NULL, NO_ALLOC, false));
	tracker->count = 0;
	tracker->capacity = initial_capacity;
	tracker->initialized = true;
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

	tmp = ft_strdup("OLDPWD");
	alloc_tracker_add(&(shell->alloc_tracker), tmp);
	if (!tmp || !append_node(shell, &(shell->env), tmp))
		return (error(NULL, NO_MEM, 1));
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (error(NULL, GETCWD, 1));
	alloc_tracker_add(&(shell->alloc_tracker), cwd);
	tmp = ft_strjoin("PWD=", cwd);
	alloc_tracker_add(&(shell->alloc_tracker), tmp);
	if (!tmp || !append_node(shell, &(shell->env), tmp))
		return (error(NULL, NO_MEM, 1));
	shell->work_dir = ft_strdup(tmp + 4);
	alloc_tracker_add(&(shell->alloc_tracker), shell->work_dir);
	if (!shell->work_dir)
		return (error(NULL, NO_MEM, 1));
	shell->old_work_dir = ft_strdup("");
	if (!shell->old_work_dir)
		return (error(NULL, NO_MEM, 1));
	alloc_tracker_add(&(shell->alloc_tracker), shell->old_work_dir);
	return (false);
}

/*
** Initialize the environment variables, if no environment is passed,
** return an error and exit with status 1. Otherwise, create a new list 
** and add all environment variables to it. 
*/

static bool	init_env(t_shell *shell, char **env)
{
	t_lst	*list;
	char	*tmp;
	int		i;

	if (!env || !(*env))
		return (error(NULL, NO_ENV, 1));
	list = NULL;
	i = -1;
	while (env[++i])
	{
		tmp = ft_strdup(env[i]);
		if (!tmp || !alloc_tracker_add(&(shell->alloc_tracker), tmp))
			return (error(NULL, NO_MEM, 1));
		if (!append_node(shell, &list, tmp))
			return (error(NULL, NO_MEM, 1));
	}
	shell->env = list;
	shell->env_count = i;
	return (false);
}

/*
** Set the user name and create the prompt string. If the user name is not set,
** return an error and exit with status 1.
*/

static int	prompt(t_shell *shell)
{
	shell->user = getenv("USER");
	if (!shell->user)
		return (error(NULL, NO_USER, 1));
	if (!shell->work_dir)
		return (error(NULL, NO_MEM, 1));
	shell->prompt = create_prompt(shell);
	if (!shell->prompt)
		return (error(NULL, NO_MEM, 1));
	return (0);
}

/*
** Initialize shell struct with allocation tracker, environment variables 
** and working directory
*/

bool	init_shell(t_shell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_shell));
	if (!init_alloc_tracker(&(shell->alloc_tracker), env))
		return (error(NULL, NO_ALLOC, 1));
	if (init_work_dirs(shell))
		return (error(NULL, NO_WD, 1));
	if (init_env(shell, env))
		return (error(NULL, NO_ENV, 1));
	if (prompt(shell))
		return (error(NULL, NO_PROMPT, 1));
	return (false);
}
