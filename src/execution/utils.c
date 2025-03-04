/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:02:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 18:56:27 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	receive_exit_status(int *status_code)
{
	if (WIFEXITED(*status_code))
		*status_code = WEXITSTATUS(*status_code);
	else if (WIFSIGNALED(*status_code))
		*status_code = WTERMSIG(*status_code) + 128;
	if (*status_code == 130)
		ft_putendl_fd("", 2);
	if (*status_code == 131)
		ft_putendl_fd("Quit (core dumped)", 2);
}

int	wait_for_children(t_shell *shell, t_cmd *cmd)
{
	int		status_code;
	size_t	i;
	pid_t	wait_result;

	status_code = 0;
	i = 0;
	wait_result = -1;
	while (i < shell->cmd_count)
	{
		while (true)
		{
			wait_result = waitpid(cmd->child_pid, &status_code, 0);
			if (wait_result != -1)
				break ;
			if (errno != EINTR)
				error_exit(shell, BAD_WAIT, "waitpid", EXIT_FAILURE);
		}
		receive_exit_status(&status_code);
		cmd = cmd->next;
		i++;
	}
	return (status_code);
}

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
