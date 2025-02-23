/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:19:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/23 09:39:43 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
**	- If '<<', call 'handle_heredoc', otherwise call 'open_redirection_file'.
**	- If successful, assign the new fd to 'cmd->fd_in' or 'cmd->fd_out'.
**	- If an old fd was already open, it is closed before assigning the new one.
*/

static bool	process_redirection(t_shell *shell, t_cmd *cmd, t_tok *token)
{
	int	*fd;
	int	old_fd;
	int	new_fd;

	if (token->type == REDIR_IN || token->type == HEREDOC)
		fd = &cmd->fd_in;
	else if (token->type == REDIR_OUT || token->type == REDIR_APPEND)
		fd = &cmd->fd_out;
	else
		return (false);
	old_fd = *fd;
	if (token->type == HEREDOC)
		new_fd = handle_heredoc(shell, token->next->content);
	else
		new_fd = open_redirection_file(token->next->content, token->type);
	if (new_fd == -1)
	{
		error_cmd(shell, token->next->content);
		return (false);
	}
	if (old_fd >= 3)
		close(old_fd);
	*fd = new_fd;
	return (true);
}

/*
**	Process a redirection and track failure
**	This function acts as a wrapper around 'process_redirection':
**	- If 'process_redirection' fails, 'redir_fail' is set to 'true',
**	  and 'cleanup_fds(cmd)' is called to close any open file descriptors.
*/

static bool	check_redirection(t_shell *shell, t_cmd *cmd, t_tok *token,
								bool *redir_fail)
{
	if (!process_redirection(shell, cmd, token))
	{
		*redir_fail = true;
		cleanup_fds(cmd);
		return (false);
	}
	return (true);
}

/*
**	Process and apply redirections for a command.
**	- If a redirection fails, the function immediately returns 'false'
**	  to indicate the command should be skipped.
**	- After processing all redirections, 'cleanup_fds(cmd)' ensures
**	  unnecessary file descriptors are closed.
*/

bool	handle_redirection(t_shell *shell, t_tok *token, t_cmd *cmd)
{
	t_tok	*current;
	bool	redir_fail;

	current = token;
	redir_fail = false;
	while (current->type == CMD || current->type == ARG)
	{
		current = current->next;
		if (current == shell->tokens)
			return (true);
	}
	while (current && current->type != PIPE && current != shell->tokens)
	{
		if (current->type == REDIR_IN || current->type == HEREDOC
			|| current->type == REDIR_OUT || current->type == REDIR_APPEND)
			if (!check_redirection(shell, cmd, current, &redir_fail))
				return (false);
		current = current->next;
	}
	cleanup_fds(cmd);
	if (redir_fail)
		return (false);
	return (true);
}
