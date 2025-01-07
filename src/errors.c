/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 10:58:46 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/06 22:21:59 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	error(t_shell *shell, char *error, int status)
{
	int	i;

	i = 0;
	if (error && *error)
	{
		while (error[i])
			write (2, &error[i++], 1);
		write(2, "\n", 1);
	}	
	if (shell)
		clean_shell(shell);
	if (status == 1)
		return (true);
	else
		exit(status);
}
