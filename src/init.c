/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:12:50 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/14 14:55:56 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** Initialize work_dir (PWD) and old_work_dir (OLDPWD), should also work with
** incomplete environment (PWD unset). Used also for cd builtin later.
*/

static bool init_work_dirs(t_shell *shell)
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
		alloc_tracker_add(&(shell->alloc_tracker), tmp);
		if (!tmp)
			return (error(NULL, NO_MEM, 1));
		if (!append_node(shell, &list, tmp))
			return (error(NULL, NO_MEM, 1));
	}
	shell->env = list;
	shell->env_count = i;
	return (false);
}

/*
** Create the prompt string, which consists of the user name and the path.
*/

static char *create_prompt(t_shell *shell)
{
	char	*prompt;
	char	*work_dir;
	size_t	len;
	size_t	total_len;

	work_dir = shell->work_dir;
	shell->home_dir = getenv("HOME");
	if (!shell->home_dir)
		return (error(shell, NO_HOME, 1), NULL);
	len = ft_strlen(shell->home_dir);
	if (ft_strncmp(shell->home_dir, work_dir, len) == 0)
		work_dir += len;
	total_len = ft_strlen(shell->user) + ft_strlen(work_dir) + 6;
	prompt = ft_calloc(sizeof(char), total_len);
	alloc_tracker_add(&(shell->alloc_tracker), prompt);
	if (!prompt)
		return (error(shell, NO_MEM, 1), NULL);
	ft_strlcpy(prompt, shell->user, total_len);
	ft_strlcat(prompt, ":~", total_len);
	ft_strlcat(prompt, work_dir, total_len);
	ft_strlcat(prompt, "$ ", total_len);
	return (prompt);
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
** Initialize the shell struct, allocate memory for the alloc tracker and
** set all values to 0. If any of the initialization functions fail, return
** an error and exit with status 1.
*/

bool	init_shell(t_shell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->alloc_tracker.allocs = ft_calloc(MAX_ALLOCS, sizeof(void *));
	shell->alloc_tracker.capacity = MAX_ALLOCS;
	if (!shell->alloc_tracker.allocs)
		return (error(NULL, NO_ALLOC, 1));
	if (init_work_dirs(shell))
		return (error(NULL, NO_WD, 1));
	if (init_env(shell, env))
		return (error(NULL, NO_ENV, 1));
	if (prompt(shell))
		return (error(NULL, NO_PROMPT, 1));
	return (false);
}
