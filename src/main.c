/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/07 13:06:32 by dplotzl          ###   ########.fr       */
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
		if (!shell->cmd_input)
			break ;
		if (blank_line(shell->cmd_input))
			continue ;
		add_history(shell->cmd_input);
		if (!tokenize_input(shell, shell->cmd_input))
			continue ;
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)av;
	if (ac > 1)
		return (error(INV_ARGS, 1));
	if (!init_shell(&shell, env))
		return (error(NO_SHELL, 1));
	minishell(&shell);
	clean_shell(&shell);
	return (0);
}
