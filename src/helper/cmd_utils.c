/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:01:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/24 00:00:00 by dplotzl          ###   ########.fr       */
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

/*
**	Skip an invalid command after redirection failure
**	- If a command has a redirection failure, this function moves 'current'
**	  forward until it reaches a pipe or the end.
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
			break ;
	}
	if (*current && (*current)->type == PIPE)
		*current = (*current)->next;
}

/*
**	Check if a redirection is incorrectly formatted
**	A redirection is invalid if:
**	- It is the last token with no argument following it ('echo >' or 'cat <').
**	- It is followed by a token that is not an argument (e.g., 'echo > | wc').
**	- A 'HEREDOC' ('<<') or 'APPEND' ('>>') does not have an argument after it.
*/

bool	invalid_redirection(t_tok *token)
{
	t_tok	*current;

	if (!token)
		return (true);
	current = token;
	while (current)
	{
		if (!current->next)
			return (true);
		if ((current->type == REDIR_IN || current->type == REDIR_OUT
				|| current->type == HEREDOC || current->type == REDIR_APPEND
				|| current->type == REDIR_APPEND)
			&& (current->next->type != ARG))
			return (true);
		current = current->next;
		if (current == token)
			break ;
	}
	return (false);
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
	else if (prev_token->type == ARG && prev_token->prev
		&& (prev_token->prev->type == REDIR_IN
			|| prev_token->prev->type == REDIR_OUT
			|| prev_token->prev->type == HEREDOC
			|| prev_token->prev->type == REDIR_APPEND))
		return (CMD);
	else
		return (ARG);
}
