/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:19:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/03 18:18:38 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
**	Return the appropriate flags for open():
**	- '>' -> Truncate the file and write ('O_TRUNC | O_WRONLY | O_CREAT').
**	- '>>'-> Append to the file ('O_APPEND | O_WRONLY | O_CREAT').
**	- '<' -> Open file for reading only ('O_RDONLY').
**	- 'O_CLOEXEC' closes the fd automatically when 'execve' is called.
*/

static int	get_redirection_flags(t_t_typ type)
{
	if (type == REDIR_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC);
	else if (type == REDIR_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC);
	else if (type == REDIR_IN)
		return (O_RDONLY | O_CLOEXEC);
	return (-1);
}

/*
**	Open files for input/output redirection
**	1. Call 'get_redirection_flags(type)' to determine the appropriate file mode.
**	2. If an invalid redirection type is detected, return an error.
**	3. Use '0644' to open the file with the correct permissions.
**	4. If the 'open()' call fails, return '-1' (handle error in caller)
*/

static int	open_redirection_file(char *file, t_t_typ type)
{
	int	flags;
	int	new_fd;

	flags = get_redirection_flags(type);
	if (flags == -1)
		return (error(INV_REDIR, -1));
	new_fd = open(file, flags, 0644);
	if (new_fd == -1)
		return (-1);
	return (new_fd);
}

/*
**	Process each redirection token:
**	- Assign correct file descriptors for input/output.
**	- If '<<', call 'eeeeeeeeeeedoc', otherwise call 'open_redirection_file'.
**	- If file opening fails, an error is logged and 'false' returned.
**	- Close any previously opened file descriptor before assigning the new one.
*/

static bool	process_redirection(t_shell *shell, t_cmd *cmd, t_tok *token)
{
	int	*fd;
	int	new_fd;

	if (cmd == NULL)
	{
		if (token->type == HEREDOC)
			new_fd = handle_heredoc(shell, token->next->content,
					token->next->is_quoted);
		else
			new_fd = open_redirection_file(token->next->content, token->type);
		if (new_fd == -1)
		{
			cmd->skip = true;
			return (error_cmd(shell, token->next->content), false);
			return (false);
		}
		close(new_fd);
		return (true);
	}
	if (token->type == REDIR_IN || token->type == HEREDOC)
		fd = &cmd->fd_in;
	else if (token->type == REDIR_OUT || token->type == REDIR_APPEND)
		fd = &cmd->fd_out;
	else
		return (false);
	if (token->type == HEREDOC)
		new_fd = handle_heredoc(shell, token->next->content,
				token->next->is_quoted);
	else
		new_fd = open_redirection_file(token->next->content, token->type);
	if (new_fd == -1)
	{
		cmd->skip = true;
		error_cmd(shell, token->next->content);
		return (false);
	}
	if (*fd != -2)
		close(*fd);
	*fd = new_fd;
	return (true);
}

/*
**	Process redirection tokens for a command.
**	- If a redirection token is found, call 'process_redirection'.
**	- If redirection processing fails, break the loop.
**	- Move to the next token after handling redirections.
*/

bool	handle_redirection(t_shell *shell, t_tok *token, t_cmd *cmd)
{
	t_tok	*current;

	current = token;
	while (current && current->type != PIPE)
	{
		if (!cmd->skip && (current->type == REDIR_IN
				|| current->type == REDIR_OUT || current->type == REDIR_APPEND))
		{
			if (!process_redirection(shell, cmd, current))
				cmd->skip = true;
			current = current->next;
		}
		else if (current->type == HEREDOC)
		{
			if (!process_redirection(shell, cmd, current))
				return (false);
			current = current->next;
		}
		current = current->next;
		if (current == shell->tokens)
			break ;
	}
	return (true);
}
