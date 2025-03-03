/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 09:36:20 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/03 19:32:39 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Allocate and initialize a new environment variable node.
*/

static t_env	*init_env(t_shell *shell, char *data)
{
	t_env	*env;

	if (!shell || !data)
		return (NULL);
	env = safe_malloc(shell, sizeof(t_env));
	if (!env)
		error_exit(shell, NO_MEM, "init_env", EXIT_FAILURE);
	env->data = data;
	env->next = NULL;
	env->prev = NULL;
	return (env);
}

/*
**	Add a new environment variable to our list.
**	If the list is empty, the new node becomes the first node,
**	with both 'next' and 'prev' pointing to itself.
**	Otherwise, the new node is added to the end of the list.
*/

t_env	*add_env_variable(t_shell *shell, t_env **lst, char *data)
{
	t_env	*env;
	t_env	*last;

	if (!shell || !lst || !data)
		return (NULL);
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
		last = (*lst)->prev;
		env->prev = last;
		env->next = (*lst);
		last->next = env;
		(*lst)->prev = env;
	}
	return (env);
}

/*
**	Unlink an environment node from our list.
**	If the list contains only one node, it is set to NULL.
*/

static void	unlink_env_node(t_env **lst, t_env *node)
{
	if (!lst || !node)
		return ;
	if (node == node->next)
		*lst = NULL;
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		if (*lst == node)
			*lst = node->next;
	}
}

/*
**	Remove an environment variable from the list based on its name.
**	Ensure safe handling of node data and memory deallocation.
*/

bool	remove_env_variable(t_shell *shell, t_env **lst, char *var_name)
{
	t_env	*node;
	size_t	len;

	if (!shell || !lst || !*lst || !var_name)
		return (false);
	node = *lst;
	len = ft_strlen(var_name);
	while (node)
	{
		if (ft_strlen(node->data) >= len && ft_strncmp(node->data, var_name,
				len) == 0 && (node->data[len] == '=' || !node->data[len]))
		{
			unlink_env_node(lst, node);
			shell->env_count--;
			return (true);
		}
		node = node->next;
		if (node == *lst)
			break ;
	}
	return (false);
}
