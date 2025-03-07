/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:12:50 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 18:54:42 by xgossing         ###   ########.fr       */
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
	shell->alloc_tracker.shell = shell;
	shell->alloc_tracker.allocs = ft_calloc(initial_capacity, sizeof(void *));
	shell->alloc_tracker.is_array = ft_calloc(initial_capacity, sizeof(int));
	if (!shell->alloc_tracker.allocs || !shell->alloc_tracker.is_array)
	{
		free(shell->alloc_tracker.allocs);
		free(shell->alloc_tracker.is_array);
		shell->alloc_tracker.allocs = NULL;
		shell->alloc_tracker.is_array = NULL;
		error_exit(shell, NO_TRACK, "init_alloc_tracker", EXIT_FAILURE);
	}
	shell->alloc_tracker.count = 0;
	shell->alloc_tracker.capacity = initial_capacity;
	shell->alloc_tracker.initialized = true;
	return (true);
}

/*
**	Initialize the environment variables, if no environment is passed,
** 	create a barebone environment. Create a new list
** 	and add all environment variables to it.
*/

static bool	init_env(t_shell *shell, char **env)
{
	t_env	*lst;
	char	*tmp;
	int		i;

	if (!env || !(*env))
		env = create_default_env(shell);
	lst = NULL;
	i = -1;
	while (env[++i])
	{
		tmp = safe_strdup(shell, env[i]);
		if (!tmp)
			return (error(NO_MEM, false));
		if (!add_env_variable(shell, &lst, tmp))
			return (error(NO_MEM, false));
	}
	shell->env = lst;
	shell->env_count = i;
	return (true);
}

/*
** Initialize work_dir (PWD) and old_work_dir (OLDPWD), should also work with
** incomplete environment (PWD unset). Used also for cd builtin later.
*/

static bool	init_work_dirs(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd || !alloc_tracker_add(&shell->alloc_tracker, cwd, 0))
		return (error(GETCWD, false));
	shell->work_dir = safe_strdup(shell, cwd);
	if (!shell->work_dir)
		return (error(NO_MEM, false));
	shell->old_work_dir = safe_strdup(shell, "");
	if (!shell->old_work_dir)
		return (error(NO_MEM, false));
	return (true);
}

/*
**	Set the user name and create the prompt string. If the user name is not set,
**	"user" is used as a default.
*/

static bool	init_prompt(t_shell *shell)
{
	if (!shell)
		return (error(NO_SHELL, false));
	shell->user = getenv("USER");
	if (!shell->user)
		shell->user = safe_strdup(shell, "user");
	if (!shell->work_dir)
		return (error(NO_WD, false));
	shell->prompt = PROMPT_MESSAGE;
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
	if (env && *env)
	{
		while (env[i])
			i++;
		initial_capacity = (i * 2);
	}
	else
		initial_capacity = DEFAULT_ALLOC_CAPACITY;
	if (!init_alloc_tracker(shell, initial_capacity))
		return (error(NO_ALLOC, false));
	if (!init_env(shell, env))
		return (error(NO_ENV, false));
	if (!init_work_dirs(shell))
		return (error(NO_WD, false));
	if (!init_prompt(shell))
		return (error(NO_PROMPT, false));
	shell->fd_copies[0] = -2;
	shell->fd_copies[1] = -2;
	shell->abort = false;
	return (true);
}
