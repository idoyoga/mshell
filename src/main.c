/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/16 19:44:47 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Advances the pointer to the next non-whitespace character
*/

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
	setup_signals(handle_sigint);
	while (1)
	{
		g_signal = 0;
		shell->cmd_input = readline(shell->prompt);
		if (!shell->cmd_input)
			break ;
		if (blank_line(shell->cmd_input))
		{
			free(shell->cmd_input);
			continue ;
		}
		add_history(shell->cmd_input);
		if (!tokenize_input(shell, shell->cmd_input))
		{
			free(shell->cmd_input);
			continue ;
		}
		if (shell->cmd != NULL)
			execute(shell);
		free(shell->cmd_input);
		shell->cmd_input = NULL;
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
