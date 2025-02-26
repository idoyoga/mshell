/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:02:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/25 23:01:37 by xgossing         ###   ########.fr       */
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
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// static bool	is_path(char *binary)
// {
// 	if (!binary || !*binary)
// 		return (false);
// 	if (*binary == '/' || ft_strncmp(binary, "./", 2) == 0 || ft_strncmp(binary,
// 			"../", 3) == 0)
// 		return (true);
// 	return (false);
// }

// char	*get_absolute_path(t_shell *shell, char *binary)
// {
// 	char	*abspath;
// 	char	**paths;
// 	int		i;

// 	if (binary == NULL)
// 		return (NULL);
// 	if (is_path(binary))
// 	{
// 		return (safe_strdup(shell, binary));
// 	}
// 	if (0 == find_or_check_env(shell, "PATH", NULL, true))
// 	{
// 		return (safe_strdup(shell, binary));
// 	}
// 	// If PATH is empty, or the binary starts with /, ./ or ../,
// 	// look for the binary in CWD. If not found return NULL.
// 	// Else, first split the PATH by ':', then generate a string
// 	// for each path, attaching the binary to the end
// 	// test each one for access, the first accessible one
// 	// is returned. If none match, NULL is returned.
// }
