/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:01:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/07 12:08:26 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Initialize a new command node
*/

static t_cmd	*init_cmd(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = safe_malloc(shell, sizeof(t_cmd));
	if (!cmd)
		error_exit(shell, NO_MEM, EXIT_FAILURE);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->fd_in = -2;
	cmd->fd_out = -2;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

/*
**	Add a new command node to the list
*/

t_cmd	*add_cmd(t_shell *shell, t_cmd **lst)
{
	t_cmd	*cmd;

	cmd = init_cmd(shell);
	if (!cmd)
		return (NULL);
	if (!*lst)
	{
		*lst = cmd;
		cmd->prev = cmd;
		cmd->next = cmd;
	}
	else
	{
		cmd->prev = (*lst)->prev;
		cmd->next = *lst;
		(*lst)->prev->next = cmd;
		(*lst)->prev = cmd;
	}
	return (cmd);
}
