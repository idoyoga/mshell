/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:19:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/13 21:02:14 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Check if the next token is an operator token
*/

static bool	is_operator_token(t_tok *token)
{
	if (token->next->type == REDIR_IN || token->next->type == REDIR_APPEND
		|| token->next->type == REDIR_OUT || token->next->type == HEREDOC
		|| token->next->type == PIPE)
		return (true);
	return (false);
}

/*
**	Open the file for input or output redirection
*/

static int	open_file(t_shell *shell, char *file, t_t_typ type)
{
	int		fd;
	int		flags;

	if (type == REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (type == REDIR_IN)
		flags = O_RDONLY;
	else if (type == HEREDOC)
		return (handle_heredoc(shell, file));
	else
		flags = 0;
	fd = open(file, flags, 0644);
	if (fd < 0)
		perror("open_file");
	return (fd);
}

/*
**	Process the redirection, assigning the correct file descriptor
**	before execution.
*/

static bool	process_redirection(t_shell *shell, t_cmd *cmd, t_tok *token)
{
	int	*fd;

	if (token->type == REDIR_IN || token->type == HEREDOC)
		fd = &cmd->fd_in;
	else
		fd = &cmd->fd_out;
	if (*fd >= 3)
	{
		close(*fd);
		*fd = -2;
	}
	if (!token->next || is_operator_token(token))
		return (false);
	*fd = open_file(shell, token->next->content, token->type);
	if (*fd == -1)
		return (error(FILE_ERR, false));
	return (true);
}

/*
**	Scan the token list for redirection tokens and apply them to the command
*/

bool	handle_redirection(t_shell *shell, t_tok *token, t_cmd *cmd)
{
	t_tok	*current;

	current = token;
	while (current->type == CMD || current->type == ARG)
	{
		current = current->next;
		if (current == shell->tokens)
			return (true);
	}
	while (current->type != PIPE && current != shell->tokens)
	{
		if (current->type == REDIR_IN || current->type == HEREDOC
			|| current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (!current->next || is_operator_token(current))
				return (false);
			if (!process_redirection(shell, cmd, current))
				return (false);
		}
		current = current->next;
	}
	return (true);
}
