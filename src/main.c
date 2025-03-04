/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:48:13 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 17:47:13 by dplotzl          ###   ########.fr       */
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
	printf("Skip: %d\n", command->skip);
	printf("FD_in: %d\n", command->fd_in);
	printf("FD_out: %d\n", command->fd_out);
	printf("(end of command %lu)\n\n", num + 1);
}

static void	print_env_data(t_env *env)
{
	t_env	*current;

	current = env;
	printf("Environment:\n");
	while (current)
	{
		printf("%s\n", current->data);
		current = current->next;
		if (current == env)
			break ;
	}
}

void	print_parsed_data(t_shell *shell)
{
	t_cmd	*current_command;
	size_t	i;

	printf("Printing shell data...\n");
	print_env_data(shell->env);
	printf("Status: %d\n", shell->status);
	printf("Prompt: %s\n", shell->prompt);
	printf("Cmd_input: %s\n", shell->cmd_input);
	printf("Home_dir: %s\n", shell->home_dir);
	printf("Work_dir: %s\n", shell->work_dir);
	printf("Old_work_dir: %s\n", shell->old_work_dir);
	printf("User: %s\n", shell->user);
	current_command = shell->cmd;
	printf("Commands (%lu):\n\n", shell->cmd_count);
	i = 0;
	while (i < shell->cmd_count)
	{
		print_command(current_command, i);
		current_command = current_command->next;
		i++;
	}
}

static void	print_tab(char **tab)
{
	int	i;

	if (!(tab))
	{
		printf("NULL");
		return ;
	}
	i = 0;
	printf("[");
	while (tab[i])
	{
		printf("%s", tab[i]);
		if (tab[i + 1])
			printf(", ");
		i++;
	}
	printf("]");
}

void	print_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	if (!cmd)
	{
		printf("No command\n");
		return ;
	}
	while (tmp->next != cmd)
	{
		printf("Skip -> %d, fd_in -> %d, fd_out -> %d, cmd : ",
		tmp->skip, tmp->fd_in, tmp->fd_out);
		print_tab(tmp->args);
		printf("\n");
		tmp = tmp->next;
	}
	printf("Skip -> %d, fd_in -> %d, fd_out -> %d, cmd : ",
		 tmp->skip, tmp->fd_in, tmp->fd_out);
	print_tab(tmp->args);
	printf("\n");
}

static void	free_cycle(void **alloc, int is_array)
{
	int		i;
	char	**array;

	if (!alloc || !*alloc)
		return ;
	if (is_array)
	{
		array = (char **)*alloc;
		i = 0;
		while (array[i])
		{
			printf("🟠 Freeing cycle array element: %p\n", array[i]);
			free(array[i]);
			i++;
		}
	}
	printf("🟢 Freeing cycle allocation: %p\n", *alloc);
	free(*alloc);
	*alloc = NULL;
}

void	free_cycle_allocs(t_alloc *tracker)
{
	int	i;

	if (!tracker || !tracker->allocs)
		return ;
	printf("🔄 Calling free_cycle_allocs()...\n");
	i = 0;
	while (i < tracker->count)
	{
		if (tracker->cycle_only[i] && tracker->allocs[i])
		{
			alloc_tracker_remove(tracker, tracker->allocs[i]);
			free_cycle(&tracker->allocs[i], tracker->is_array[i]);
			tracker->allocs[i] = NULL;
			tracker->is_array[i] = 0;
			tracker->cycle_only[i] = 0;
		}
		i++;
	}
	free(tracker->allocs);
	free(tracker->is_array);
	free(tracker->cycle_only);
	*tracker = (t_alloc){0};
}

static void alloc_tracker_purge_cycle(t_alloc *tracker)
{
	int i;
	int j;

	if (!tracker || tracker->count == 0)
		return;
	printf("🧹 Calling alloc_tracker_purge_cycle()...\n");
	free_cycle_allocs(tracker);
	i = 0;
	j = 0;
	while (i < tracker->count)
	{
		if (tracker->allocs[i] != NULL)
		{
			tracker->allocs[j] = tracker->allocs[i];
			tracker->is_array[j] = tracker->is_array[i];
			tracker->cycle_only[j] = tracker->cycle_only[i];
			j++;
		}
		i++;
	}
	tracker->count = j;
}

/*
**	Start minishell, provide the prompt, read input and execute commands
*/

static void	minishell(t_shell *shell)
{
	while (1)
	{
		shell->cmd = NULL;
		setup_signals(handle_sigint, SIG_IGN);
		g_signal = 0;
		shell->abort = false;
		shell->cmd_input = alloc_tracker_replace(&shell->alloc_tracker,
				shell->cmd_input, readline(shell->prompt));
		if (g_signal != 0)
			shell->status = g_signal + 128;
		if (!shell->cmd_input)
			break ;
		setup_signals(SIG_IGN, SIG_IGN);
		if (blank_line(shell->cmd_input))
			continue ;
		add_history(shell->cmd_input);
		if (!tokenize_input(shell, shell->cmd_input))
			continue ;
		print_cmd(shell->cmd);
		if (shell->cmd != NULL && !shell->abort)
		{
			prepare_execution(shell);
			dispatch(shell);
			printf(" 🚀 Commands dispatched.\n");
			/* postpare_execution(shell); */
		}
		alloc_tracker_purge_cycle(&shell->alloc_tracker);
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
