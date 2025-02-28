/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:54:56 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/27 20:38:54 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_exit_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '\0')
		return (false);
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

void	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	size_t	count;

	if (shell->cmd_count == 1)
		printf("exit\n");
	count = count_cmd_args(cmd);
	if (count == 1)
	{
		clean_shell(shell);
		exit(shell->status);
	}
	if (!is_valid_exit_arg(cmd->args[1]))
	{
		error_exit(shell, EXIT_INVALID_ARGUMENT, "exit", 2);
	}
	if (count > 1)
		shell->status = ft_atoi(cmd->args[1]); // TODO: protect
	if (count > 2)
	{
		shell->status = 1;
		error(EXIT_TOO_MANY_ARGUMENTS, 1); // TODO: add context to error()?
		return ;
	}
	clean_shell(shell);
	exit(shell->status);
}
