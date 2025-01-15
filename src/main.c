/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/15 17:38:00 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** Clean up the shell struct, more functions might be added here
*/

static void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_allocs(&shell->alloc_tracker);
}

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

/*
** Check if argument count is valid and initialization was successful
*/

static bool	check_init(t_shell *shell, int ac, char **env)
{
	if (ac > 1)
		return (error(NULL, INV_ARGS, 1));
	if (init_shell(shell, env))
		return (error(shell, NO_SHELL, 1));
	return (false);
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)av;
	if (check_init(&shell, ac, env))
		return (EXIT_FAILURE);
	clean_shell(&shell);
	return (0);
}
