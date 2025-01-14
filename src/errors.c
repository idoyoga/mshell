/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 10:58:46 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/14 14:52:10 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** Print error message and clean up shell struct (if shell is not NULL),
** then return either true (error) or false (no error).
*/

bool	error(t_shell *shell, char *error, int status)
{
	int	i;

	i = 0;
	if (error && *error)
	{
		write(2, error, ft_strlen(error));
		write(2, "\n", 1);
	}	
	if (shell)
		clean_shell(shell);
	if (status != 0)
		return (true);
	else
		return (false);
}
