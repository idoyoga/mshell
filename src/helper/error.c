/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:09:55 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/07 13:06:32 by dplotzl          ###   ########.fr       */
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

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_allocs(&shell->alloc_tracker);
}
