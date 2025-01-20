/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:12:50 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/20 20:07:03 by dplotzl          ###   ########.fr       */
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
	if (!tmp || !alloc_tracker_add(&(shell->alloc_tracker), tmp))
		return (error(NULL, NO_MEM, false));
	if (!add_env_var(shell, &(shell->env), tmp))
		return (error(NULL, NO_MEM, false));
	cwd = getcwd(NULL, 0);
	if (!cwd || !alloc_tracker_add(&(shell->alloc_tracker), cwd))
		return (error(NULL, GETCWD, false));
	tmp = ft_strjoin("PWD=", cwd);
	if (!tmp || !alloc_tracker_add(&(shell->alloc_tracker), tmp))
		return (error(NULL, NO_MEM, false));
	if (!add_env_var(shell, &(shell->env), tmp))
		return (error(NULL, NO_MEM, false));
	shell->work_dir = ft_strdup(cwd);
	if (!shell->work_dir
		|| !alloc_tracker_add(&(shell->alloc_tracker), shell->work_dir))
		return (error(NULL, NO_MEM, false));
	shell->old_work_dir = ft_strdup("");
	if (!shell->old_work_dir
		|| !alloc_tracker_add(&(shell->alloc_tracker), shell->old_work_dir))
		return (error(NULL, NO_MEM, false));
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
		return (error(NULL, NO_ENV, false));
	lst = NULL;
	i = -false;
	while (env[++i])
	{
		tmp = ft_strdup(env[i]);
		if (!tmp || !alloc_tracker_add(&(shell->alloc_tracker), tmp))
			return (error(NULL, NO_MEM, false));
		if (!add_env_var(shell, &lst, tmp))
			return (error(NULL, NO_MEM, false));
	}
	shell->env = lst;
	shell->env_count = i;
	return (true);
}

/*
** Set the user name and create the prompt string. If the user name is not set,
** return an error and exit with status false.
*/

static bool	prompt(t_shell *shell)
{
	shell->user = getenv("USER");
	if (!shell->user)
		return (error(NULL, NO_USER, false));
	if (!shell->work_dir)
		return (error(NULL, NO_MEM, false));
	shell->prompt = create_prompt(shell);
	if (!shell->prompt)
		return (error(NULL, NO_MEM, false));
	return (true);
}

/*
** Initialize shell struct with allocation tracker, environment variables 
** and working directory
*/

bool	init_shell(t_shell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_shell));
	if (!init_alloc_tracker(&(shell->alloc_tracker), env))
		return (error(NULL, NO_ALLOC, false));
	if (!init_work_dirs(shell))
		return (error(NULL, NO_WD, false));
	if (!init_env(shell, env))
		return (error(NULL, NO_ENV, false));
	if (!prompt(shell))
		return (error(NULL, NO_PROMPT, false));
	return (true);
}
