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

static bool	init_node(t_shell *shell, t_env **node, char *data)
{
	if (!data)
		return (false);
	*node = (t_env *)wrap_malloc(&(shell->alloc_tracker), sizeof(t_env));
	if (!(*node))
		return (false);
	(*node)->data = data;
	(*node)->next = NULL;
	(*node)->prev = NULL;
	return (true);
}

/*
**	Append a new node to the end of the list. If the list is empty,
**	the new node will be the first node and made circular.
*/

bool	add_env_var(t_shell *shell, t_env **lst, char *data)
{
	t_env	*node;

	if (!data || !init_node(shell, &node, data))
		return (false);
	if (!(*lst))
	{
		(*lst) = node;
		node->prev = node;
		node->next = node;
	}
	else
	{
		node->prev = (*lst)->prev;
		node->next = (*lst);
		(*lst)->prev->next = node;
		(*lst)->prev = node;
	}
	return (true);
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
		return (error(NULL, NO_HOME, 1), NULL);
	len = ft_strlen(shell->home_dir);
	if (ft_strncmp(shell->home_dir, work_dir, len) == 0 && work_dir[len] == '/')
		work_dir += len;
	total_len = ft_strlen(shell->user) + ft_strlen(work_dir) + 6;
	prompt = (char *)wrap_calloc(&shell->alloc_tracker, total_len,
			sizeof(char));
	if (!prompt)
		return (error(NULL, NO_MEM, 1), NULL);
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
