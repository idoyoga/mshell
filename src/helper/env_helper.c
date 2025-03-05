/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:07:01 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 22:05:44 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_shell *shell, char *key)
{
	t_env	*current;
	size_t	len;

	if (!shell | !shell->env || !key)
		return (NULL);
	current = shell->env;
	len = ft_strlen(key);
	while (current)
	{
		if (ft_strlen(current->data) > len && ft_strncmp(current->data, key,
				len) == 0 && current->data[len] == '=')
			return (current->data + len + 1);
		current = current->next;
		if (current == shell->env)
			break ;
	}
	return (NULL);
}

/*
**	Create a default environment with PATH and SHLVL variables in case
**	no environment is passed.
*/

char	**create_default_env(t_shell *shell)
{
	char	**env;
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
		ft_strlcpy(cwd, "/", 2);
	env = safe_malloc(shell, sizeof(char *) * 4);
	if (!env)
		return (NULL);
	env[0] = safe_strdup(shell, "PATH=/usr/bin:/bin");
	env[1] = safe_strdup(shell, "SHLVL=1");
	env[2] = safe_strdup(shell, safe_strjoin(shell, "PWD=", cwd));
	env[3] = NULL;
	if (!env[0] || !env[1] || !env[2])
		error_exit(shell, NO_ENV, "create_default_env", EXIT_FAILURE);
	return (env);
}

static char	*join_key_value_pair(t_shell *shell, char *key, char *value)
{
	char	*key_value_string;
	char	*temp;

	temp = safe_strjoin(shell, key, "=");
	key_value_string = safe_strjoin(shell, temp, value);
	alloc_tracker_remove(&shell->alloc_tracker, temp);
	free(temp);
	return (key_value_string);
}

/*
**	Upsert an environment variable.
*/

bool	upsert_env_variable(t_shell *shell, t_env **lst, char *key,
		char *new_value)
{
	t_env	*node;
	char	*updated_value;
	int		len;

	if (!lst || !*lst || !key || !new_value)
		return (false);
	if (find_or_check_env(shell, key, NULL, true))
	{
		node = *lst;
		while (node)
		{
			len = match_env_variable(key, node->data);
			if (len > 0)
			{
				node->data = join_key_value_pair(shell, key, new_value);
				return (true);
			}
			node = node->next;
			if (node == *lst)
				break ;
		}
	}
	updated_value = join_key_value_pair(shell, key, new_value);
	return (add_env_variable(shell, lst, updated_value) != NULL);
}
