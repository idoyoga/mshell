/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:02:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/02 22:26:42 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env_array(t_shell *shell)
{
	char	**envp;
	t_env	*current_env;
	int		i;

	envp = safe_calloc(shell, shell->env_count + 1, sizeof(char *));
	i = 0;
	current_env = shell->env;
	while (i < shell->env_count)
	{
		envp[i] = safe_strdup(shell, current_env->data);
		if (!envp[i])
			return (NULL);
		current_env = current_env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// second if checks might not even be necessary
// if there's a '/', bash always seems to treat it as a path
// and look for it in the current working directory
inline bool	is_path(char *str)
{
	if (!str)
		return (false);
	if (str[0] == '/' || ft_strncmp("./", str, 2) == 0 || ft_strncmp("../", str,
			3) == 0)
		return (true);
	if (ft_strchr(str, '/') != NULL)
		return (true);
	return (false);
}
