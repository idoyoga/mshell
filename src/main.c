/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/14 14:54:25 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
 * Check if argument count is valid and initialization was successful
*/

static bool	check_init(t_shell *shell, int ac, char **env)
{
	if (ac > 1)
		return (error(NULL, INV_ARGS, 1));
	if (init_shell(shell, env))
		return (error(shell, NO_SHELL, 1));
	return (false);
}

int main(int ac, char **av, char **env)
{
	t_shell	shell;
	
	(void)av;
	if (check_init(&shell, ac, env))
		return (error(NULL, INIT_FAIL, EXIT_FAILURE));
	clean_shell(&shell);
	return (0);
}
