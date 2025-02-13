/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:26:32 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/12 21:57:43 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	execute_without_pipeline(t_shell *shell)
{
	(void)shell;
	printf("executing without pipeline\n");
}

// run single builtins without forking
// if receiving multiple commands,
// fork and execute for each command
void	execute(t_shell *shell)
{
	size_t	cmd_count;
	t_b_typ	type;

	cmd_count = cmd_lstlen(shell->cmd);
	if (cmd_count != 1)
		return (execute_with_pipeline(shell, cmd_count));
	type = identify_builtin(shell->cmd->args[0]);
	if (type == _NOT_A_BUILTIN)
		return (execute_without_pipeline(shell));
	return (execute_single_builtin(shell, type));
}
