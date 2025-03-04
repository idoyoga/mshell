/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:01:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 14:38:25 by xgossing         ###   ########.fr       */
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
		error_exit(shell, NO_MEM, "init_cmd", EXIT_FAILURE);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->fd_in = -2;
	cmd->fd_out = -2;
	cmd->skip = false;
	cmd->access_status = A_CAN_EXECUTE;
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

/*
**	Skip an invalid command after redirection failure
**	- If a command has a redirection failure, this function moves 'current'
**		forward until it reaches a pipe or the end.
**	- If we reach a  pipe, it ensures the next command is processed.
*/

void	skip_invalid_command(t_shell *shell, t_tok **current)
{
	if (!shell || !current || !*current)
		return ;
	while (*current && (*current)->type != PIPE)
	{
		*current = (*current)->next;
		if (*current == shell->tokens)
			return ;
	}
	if (*current && (*current)->type == PIPE)
		*current = (*current)->next;
}

/*
**	Determine whether a token should be CMD or ARG based on the previous token.
**	- If there is no previous token or if it follows a 'PIPE', it is a 'CMD'.
**	- If it follows a redirection ('<', '>', '>>', '<<'), it is an 'ARG'.
**	- If it follows an 'ARG' that itself follows a redirection, it is a 'CMD'.
*/

t_t_typ	determine_token_type(t_tok **lst)
{
	t_tok	*prev_token;

	prev_token = NULL;
	if (*lst)
		prev_token = (*lst)->prev;
	if (!prev_token || prev_token->type == PIPE)
		return (CMD);
	else if (prev_token->type == REDIR_IN || prev_token->type == REDIR_OUT
		|| prev_token->type == HEREDOC || prev_token->type == REDIR_APPEND)
		return (ARG);
	else if (!(*lst)->first_cmd && prev_token->type == ARG && prev_token->prev
		&& (prev_token->prev->type == REDIR_IN
			|| prev_token->prev->type == REDIR_OUT
			|| prev_token->prev->type == HEREDOC
			|| prev_token->prev->type == REDIR_APPEND))
		return (CMD);
	else
		return (ARG);
}
