/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:01:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/20 19:35:15 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Allocate and initialize a new command structure
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

	if (!shell || !lst)
		return (NULL);
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

bool	is_command_start(t_tok *current)
{
	if (current->type == CMD)
		return (true);
	if (current->type == ARG && current->prev)
	{
		if (current->prev->type == PIPE || current->prev->type == REDIR_IN
			|| current->prev->type == REDIR_OUT || current->prev->type == HEREDOC
			|| current->prev->type == REDIR_APPEND)
			return (true);
	}
	return (false);
}

bool	invalid_redirection(t_tok *token)
{
	if (!token->next)
		return (true);
	if (token->type == REDIR_IN && token->next->type != ARG)
		return (true);
	if (token->type == REDIR_OUT && token->next->type != ARG)
		return (true);
	if (token->type == HEREDOC && token->next->type != ARG)
		return (true);
	if (token->type == REDIR_APPEND && token->next->type != ARG)
		return (true);
	return (false);
}
