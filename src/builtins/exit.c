/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:54:56 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 12:43:29 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_exit_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg == NULL)
		return (false);
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

// TODO: handle extreme overflow (9223372036854775808, treat as non-number (2)))
void	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	size_t	count;

	if (shell->cmd_count == 1)
		ft_putendl_fd("exit", STDOUT_FILENO);
	count = count_cmd_args(cmd);
	if (count == 1)
		(clean_shell(shell), exit(shell->status));
	if (!is_valid_exit_arg(cmd->args[1]))
		error_exit_s(shell, EXIT_INVALID_ARGUMENT, "exit", 2);
	if (count > 1)
	{
		shell->status = ft_atoi(cmd->args[1]);
	}
	if (count > 2)
	{
		shell->status = 1;
		error_context(EXIT_TOO_MANY_ARGUMENTS, "exit", 1);
		return ;
	}
	clean_shell(shell);
	exit(shell->status);
}
