/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:54:38 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 18:49:25 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_flag(char *token)
{
	int	i;

	i = 1;
	while (token[i] != '\0')
	{
		if (!ft_isalpha(token[i]))
			return (false);
		i++;
	}
	return (true);
}

static bool	ft_contains(char *str, char c)
{
	int	i;

	if (str == NULL)
		return (false);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (true);
		i++;
	}
	return (false);
}

// if string starts with a single '-',
// read the next characters until a non-alphabetical character is found
// if a space is found first,
// the flag is valid and should be checked for -n
// if any of the characters in the valid flag is 'n', return true
// if the flag contains a non-alphabetical character before a space
// / end of token is encountered, the flag is invalid and should be
// treated as an argument to be printed out instead
static bool	has_n_flag(char **args)
{
	if (args[0] && args[1] && args[1][0] == '-')
	{
		if (is_valid_flag(args[1]) && ft_contains(args[1], 'n'))
			return (true);
	}
	return (false);
}

void	builtin_echo(t_shell *shell, t_cmd *cmd)
{
	int		arg_index;
	bool	has_n;

	has_n = has_n_flag(cmd->args);
	arg_index = 1;
	if (has_n)
		arg_index = 2;
	while (cmd->args[arg_index] != NULL)
	{
		printf("%s", cmd->args[arg_index]);
		if (cmd->args[arg_index + 1] != NULL)
			printf(" ");
		arg_index++;
	}
	if (!has_n)
		printf("\n");
	shell->status = 0;
}
