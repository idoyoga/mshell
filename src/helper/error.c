/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:09:55 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/20 19:46:43 by dplotzl          ###   ########.fr       */
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

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_allocs(&shell->alloc_tracker);
}
