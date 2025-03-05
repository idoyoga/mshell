/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 22:31:24 by xgossing         ###   ########.fr       */
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

static void	scrub_command(t_shell *shell, t_cmd *command)
{
	size_t	i;

	i = 0;
	while (command->args && command->args[i])
	{
		alloc_tracker_remove(&shell->alloc_tracker, command->args[i]);
		free(command->args[i]);
		i++;
	}
	alloc_tracker_remove(&shell->alloc_tracker, command->args);
	free(command->args);
	command->args = NULL;
	if (command->cmd)
	{
		alloc_tracker_remove(&shell->alloc_tracker, command->cmd);
		free(command->cmd);
		command->cmd = NULL;
	}
}

static void	scrub_commands(t_shell *shell, t_cmd *current_command)
{
	while (current_command)
	{
		scrub_command(shell, current_command);
		current_command = current_command->next;
		if (current_command == shell->cmd)
			break ;
	}
}

/*
**	Start minishell, provide the prompt, read input and execute commands
*/

static void	minishell(t_shell *shell)
{
	while (1)
	{
		setup_signals(handle_sigint, SIG_IGN);
		g_signal = 0;
		shell->abort = false;
		shell->cmd_input = alloc_tracker_replace(&shell->alloc_tracker,
				shell->cmd_input, readline(shell->prompt));
		if (g_signal != 0)
			shell->status = g_signal + 128;
		if (!shell->cmd_input)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		setup_signals(SIG_IGN, SIG_IGN);
		if (blank_line(shell->cmd_input))
			continue ;
		add_history(shell->cmd_input);
		if (!tokenize_input(shell, shell->cmd_input))
			continue ;
		if (shell->cmd != NULL && !shell->abort)
			(dispatch(shell), postpare_execution(shell));
		scrub_commands(shell, shell->cmd);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)av;
	if (ac > 1)
		return (error(INV_ARGS, 1));
	if (!init_shell(&shell, env))
	{
		clean_shell(&shell);
		return (error(NO_SHELL, 1));
	}
	minishell(&shell);
	rl_clear_history();
	clean_shell(&shell);
	return (0);
}
