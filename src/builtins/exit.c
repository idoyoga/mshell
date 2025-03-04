/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:54:56 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 16:09:47 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	g_max_positive[] = "9223372036854775807";
static const char	g_max_negative[] = "9223372036854775808";

static int	count_digits(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (i);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (-1);
		i++;
	}
	return (i);
}

static bool	is_valid_exit_arg(char *arg)
{
	bool	is_negative;
	int		start;
	int		digits;

	start = 0;
	if (arg == NULL)
		return (false);
	if (arg[start] == '\0')
		return (false);
	is_negative = arg[start] == '-';
	if (arg[start] == '-' || arg[start] == '+')
		start++;
	digits = count_digits(arg + start);
	if (digits == -1 || digits > 19)
		return (false);
	if (digits == 19)
	{
		if (is_negative && ft_strncmp(arg + start, g_max_negative, 19) > 0)
			return (false);
		if (!is_negative && ft_strncmp(arg + start, g_max_positive, 19) > 0)
			return (false);
	}
	return (true);
}

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
