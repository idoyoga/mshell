/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 15:11:57 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/07 14:28:42 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
 * Create a new node for the linked list of env variables by taking a single
 * env variable string and splitting it into key and value ("KEY=VALUE").
 * If no "=" is found, the whole string is used as key and value is set to NULL.
*/

static t_env	*create_env_node(t_shell *shell, char *env_var)
{
	t_env	*node;
	char	*equal_sign;

	node = (t_env *)ft_calloc(sizeof(t_env), 1);
	if (!node)
		return(error(shell, NO_MEM, EXIT_FAILURE), NULL);
	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		node->key = ft_substr(env_var, 0, equal_sign - env_var);
		node->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		node->key = ft_strdup(env_var);
		node->value = NULL;
	}
	if (!node->key || (equal_sign && !node->value))
		return (free_env_node(&node), error(NULL, NO_MEM, 1), NULL);
	return (node);
}

/*
 * Initialize environment list
*/

t_env	*init_env(t_shell *shell, char **env)
{
	t_env	*env_list;
	t_env	*new_env;
	int		i;

	i = -1;
	env_list = NULL;
	while (env[++i])
	{
		new_env = create_env_node(shell, env[i]);
		if (!new_env)
			return (free_env_list(&env_list), 
				error(NULL, NO_MEM, EXIT_FAILURE), NULL);
		new_env->next = env_list;
		env_list = new_env;
	}
	shell->env_count = i;
	printf("env_count: %d\n", shell->env_count); // debug
	shell->allocs[shell->alloc_count++] = env_list;
	return (env_list);
}
