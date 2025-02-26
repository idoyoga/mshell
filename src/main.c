/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/25 23:44:07 by xgossing         ###   ########.fr       */
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

static void	print_command(t_cmd *command, size_t num)
{
	size_t	i;

	printf("(start of command %lu)\n", num + 1);
	printf("Cmd: %s\n", command->cmd);
	printf("Args:\n");
	if (command->args == NULL)
		printf("None? D:\n");
	else
	{
		i = 0;
		while (command->args[i] != NULL)
		{
			printf("%lu: '%s'\n", i, command->args[i]);
			i++;
		}
	}
	printf("FD_in: %d\n", command->fd_in);
	printf("FD_out: %d\n", command->fd_out);
	printf("(end of command %lu)\n\n", num + 1);
}

void	print_parsed_data(t_shell *shell)
{
	size_t	cmd_count;
	t_cmd	*current_command;
	size_t	i;

	printf("Printing shell data...\n");
	printf("Status: %d\n", shell->status);
	printf("Prompt: %s\n", shell->prompt);
	printf("Cmd_input: %s\n", shell->cmd_input);
	printf("Home_dir: %s\n", shell->home_dir);
	printf("Work_dir: %s\n", shell->work_dir);
	printf("Old_work_dir: %s\n", shell->old_work_dir);
	printf("User: %s\n", shell->user);
	current_command = shell->cmd;
	cmd_count = 0;
	while (current_command)
	{
		cmd_count++;
		current_command = current_command->next;
		if (current_command == shell->cmd)
			break ;
	}
	printf("Commands (%lu):\n\n", cmd_count);
	i = 0;
	while (i < cmd_count)
	{
		print_command(current_command, i);
		current_command = current_command->next;
		i++;
	}
}

static size_t	cmd_lstlen(t_cmd *cmd)
{
	size_t	i;
	t_cmd	*current;

	if (cmd == NULL)
		return (0);
	i = 1;
	current = cmd->next;
	while (current != cmd)
	{
		i++;
		current = current->next;
	}
	return (i);
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
		shell->cmd_input = alloc_tracker_replace(&shell->alloc_tracker,
				shell->cmd_input, readline(shell->prompt));
		if (!shell->cmd_input)
			break ;
		setup_signals(SIG_IGN, SIG_IGN);
		if (blank_line(shell->cmd_input))
			continue ;
		add_history(shell->cmd_input);
		if (!tokenize_input(shell, shell->cmd_input))
			continue ;
		if (shell->cmd != NULL)
		{
			prepare_execution(shell, cmd_lstlen(shell->cmd));
			dispatch(shell, cmd_lstlen(shell->cmd));
			postpare_execution(shell, cmd_lstlen(shell->cmd));
		}
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
	{
		clean_shell(&shell);
		return (error(NO_SHELL, 1));
	}
	minishell(&shell);
	clean_shell(&shell);
	return (0);
}
