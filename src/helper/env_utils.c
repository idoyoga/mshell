/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 09:36:20 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/22 20:44:34 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Allocate and initialise new env node
*/

static t_env	*init_env(t_shell *shell, char *data)
{
	t_env	*env;

	if (!data)
		return (NULL);
	env = safe_malloc(shell, sizeof(t_env));
	if (!env)
		error_exit(shell, NO_MEM, EXIT_FAILURE);
	env->data = data;
	env->next = NULL;
	env->prev = NULL;
	return (env);
}

/*
**	Add a new node to the end of the list. If the list is empty,
**	the new node will be the first node and made circular.
*/

t_env	*add_env_var(t_shell *shell, t_env **lst, char *data)
{
	t_env	*env;

	env = init_env(shell, data);
	if (!env)
		return (NULL);
	if (!(*lst))
	{
		(*lst) = env;
		env->prev = env;
		env->next = env;
	}
	else
	{
		env->prev = (*lst)->prev;
		env->next = (*lst);
		(*lst)->prev->next = env;
		(*lst)->prev = env;
	}
	return (env);
}

/*
**	Create the prompt, using the user and working directory
*/

char	*create_prompt(t_shell *shell)
{
	char	*prompt;
	char	*work_dir;
	size_t	len;
	size_t	total_len;

	work_dir = shell->work_dir;
	shell->home_dir = getenv("HOME");
	if (!shell->home_dir)
		return (error(NO_HOME, 1), NULL);
	len = ft_strlen(shell->home_dir);
	if (ft_strncmp(shell->home_dir, work_dir, len) == 0 && work_dir[len] == '/')
		work_dir += len;
	total_len = ft_strlen(shell->user) + ft_strlen(work_dir) + 6;
	prompt = safe_calloc(shell, total_len, sizeof(char));
	if (!prompt)
		error_exit(shell, NO_MEM, EXIT_FAILURE);
	ft_strlcpy(prompt, shell->user, total_len);
	ft_strlcat(prompt, ":~", total_len);
	ft_strlcat(prompt, work_dir, total_len);
	ft_strlcat(prompt, "$ ", total_len);
	return (prompt);
}

int	env_var_count(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}
