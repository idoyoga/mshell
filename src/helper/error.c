/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:09:55 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/22 21:23:41 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Prints an error message to stderr and returns true if status is not 0.
*/

bool	error(const char *error_msg, int status)
{
	if (error_msg && *error_msg)
	{
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\n", 1);
	}
	if (status == 0)
		return (false);
	else
		return (true);
}

/*
**	Prints an error message to stderr and exits the program with the given
**	exit status.
*/

void	error_exit(t_shell *shell, const char *error_msg, int exit_status)
{
	if (error_msg && *error_msg)
	{
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\n", 1);
	}
	if (shell)
		clean_shell(shell);
	exit(exit_status);
}

/*
**	Print a syntax error message for an unexpected token
**	1. If the next token is `shell->tokens` (indicating the end of input),
**	   it prints `newline`, signaling a missing argument.
**	2. Otherwise, it prints the unexpected token's content.
*/

bool	error_token(t_shell *shell, t_tok *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token->next == shell->tokens)
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd(token->next->content, 2);
	ft_putstr_fd("'\n", 2);
	return (false);
}

/*
**	Print an error message for a command that failed to execute.
*/

void	error_cmd(t_shell *shell, const char *cmd_name)
{
	char	*err_msg;

	err_msg = ft_strjoin_four("minishell: ", cmd_name, ": ", strerror(errno));
	alloc_tracker_add(&shell->alloc_tracker, err_msg, 0);
	if (!err_msg)
		error(NO_MEM, false);
	ft_putendl_fd(err_msg, 2);
}
