// static void	print_command(t_cmd *command, size_t num)
// {
// 	size_t	i;

// 	printf("(start of command %lu)\n", num + 1);
// 	printf("Cmd: %s\n", command->cmd);
// 	printf("Args:\n");
// 	if (command->args == NULL)
// 		printf("None? D:\n");
// 	else
// 	{
// 		i = 0;
// 		while (command->args[i] != NULL)
// 		{
// 			printf("%lu: '%s'\n", i, command->args[i]);
// 			i++;
// 		}
// 	}
// 	printf("Skip: %d\n", command->skip);
// 	printf("FD_in: %d\n", command->fd_in);
// 	printf("FD_out: %d\n", command->fd_out);
// 	printf("(end of command %lu)\n\n", num + 1);
// }

// static void	print_env_data(t_env *env)
// {
// 	t_env	*current;

// 	current = env;
// 	printf("Environment:\n");
// 	while (current)
// 	{
// 		printf("%s\n", current->data);
// 		current = current->next;
// 		if (current == env)
// 			break ;
// 	}
// }

// void	print_parsed_data(t_shell *shell)
// {
// 	t_cmd	*current_command;
// 	size_t	i;

// 	printf("Printing shell data...\n");
// 	print_env_data(shell->env);
// 	printf("Status: %d\n", shell->status);
// 	printf("Prompt: %s\n", shell->prompt);
// 	printf("Cmd_input: %s\n", shell->cmd_input);
// 	// printf("Home_dir: %s\n", shell->home_dir);
// 	printf("Work_dir: %s\n", shell->work_dir);
// 	printf("Old_work_dir: %s\n", shell->old_work_dir);
// 	printf("User: %s\n", shell->user);
// 	current_command = shell->cmd;
// 	printf("Commands (%lu):\n\n", shell->cmd_count);
// 	i = 0;
// 	while (i < shell->cmd_count)
// 	{
// 		print_command(current_command, i);
// 		current_command = current_command->next;
// 		i++;
// 	}
// }

// static void	print_tab(char **tab)
// {
// 	int	i;

// 	if (!(tab))
// 	{
// 		printf("NULL");
// 		return ;
// 	}
// 	i = 0;
// 	printf("[");
// 	while (tab[i])
// 	{
// 		printf("%s", tab[i]);
// 		if (tab[i + 1])
// 			printf(", ");
// 		i++;
// 	}
// 	printf("]");
// }

// void	print_cmd(t_cmd *cmd)
// {
// 	t_cmd	*tmp;

// 	tmp = cmd;
// 	if (!cmd)
// 	{
// 		printf("No command\n");
// 		return ;
// 	}
// 	while (tmp->next != cmd)
// 	{
// 		printf("Skip -> %d, fd_in -> %d, fd_out -> %d, count -> %d, cmd : ",
// 			tmp->skip, tmp->fd_in, tmp->fd_out, tmp->argc);
// 		print_tab(tmp->args);
// 		printf("\n");
// 		tmp = tmp->next;
// 	}
// 	printf("Skip -> %d, fd_in -> %d, fd_out -> %d, cmd : ", tmp->skip,
// 		tmp->fd_in, tmp->fd_out);
// 	print_tab(tmp->args);
// 	printf("\n");
// }
