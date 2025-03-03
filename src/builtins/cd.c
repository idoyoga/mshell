/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:56:21 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/03 19:39:26 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_directory(t_shell *shell, char *directory)
{
	char	*working_directory;

	working_directory = getcwd(NULL, 0);
	if (!working_directory)
	{
		// handle critical error
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(directory, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return ;
	}
	if (chdir(directory) != 0)
	{
		free(working_directory);
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(directory, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		shell->status = 1;
		return ;
	}
	// set OLDPWD in env
	// set new PWD
	// update shell prompt
	// set success status code?
	free(working_directory);
}

void	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*home_dir;
	size_t	arg_count;

	home_dir = NULL;
	arg_count = count_cmd_args(cmd);
	if (arg_count > 2)
	{
		shell->status = 1;
		error(CD_TOO_MANY_ARGUMENTS, 1);
		return ;
	}
	if (arg_count == 1)
	{
		home_dir = get_env_value(shell, "HOME");
		if (!home_dir)
		{
			error(CD_NO_HOME, 1);
			return ;
		}
		return (change_directory(shell, home_dir));
	}
	change_directory(shell, cmd->args[1]);
}
