/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/23 13:10:52 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	blank_line(char *input)
{
	while (*input)
	{
		if (!ft_isblank(*input))
			return (false);
		input++;
	}
	return (true);
}

/*
** Starts minishell, provides the prompt, reads input and executes commands
*/

static void	minishell(t_shell *shell)
{
	while (1)
	{
		shell->cmd_input = readline(shell->prompt);
		alloc_tracker_add(&(shell->alloc_tracker), shell->cmd_input, 0);
		if (!shell->cmd_input)
			break ;
		if (blank_line(shell->cmd_input))
			continue ;
		add_history(shell->cmd_input);
		if (ft_strcmp(shell->cmd_input, "exit") == 0)
			break ;
	}
	rl_clear_history();
}

/*
**	Clean up the shell struct, more functions might be added here
*/

static void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_allocs(&shell->alloc_tracker);
}

/*
**	Print error message and clean up shell struct (if shell is not NULL),
**	then return either true (error) or false (no error).
*/

bool	error(t_shell *shell, char *error, int status)
{
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

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)av;
	if (ac > 1)
		return (error(NULL, INV_ARGS, 1));
	if (!init_shell(&shell, env))
		return (error(&shell, NO_SHELL, 1));
	minishell(&shell);
	clean_shell(&shell);
	return (0);
}
