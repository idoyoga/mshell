/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 21:15:51 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/03 16:28:23 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Count the number of arguments for a command:
**	- Start counting if the token is CMD or ARG not preceded by a PIPE.
**	- Iterate through tokens until PIPE or the start of the token list.
**	- Return the total number of arguments, including the command itself.
*/

static int	count_args(t_shell *shell, t_tok *token)
{
	t_tok	*current;
	int		i;

	i = 0;
	current = token;
	if (!current->is_null && (current->type == CMD || (current->type == ARG
				&& current->prev->type != PIPE)))
		i++;
	current = current->next;
	while (current != shell->tokens && current->type != PIPE)
	{
		if (!current->is_null && (current->type == CMD || (current->type == ARG
					&& current->prev->type > PIPE)))
			i++;
		current = current->next;
	}
	printf("counted %d args\n", i);
	return (i);
}

/*
**	Process a single token and determine if it is a redirection,
		command or pipe.
**
**	- If no command has been added yet, create a new command node.
**	- If the token is a redirection ('<', '>', '>>', '<<'):
**		- Call 'handle_redirection' to process it.
**		- If the redirection fails, the invalid command is skipped.
**		- If the redirection is successful (and is not a heredoc),
**			set the 'redirected' flag to prevent further redirection handling.
**	- If the token is a pipe:
**		- Create a new command node for the next piped command.
**		- Reset 'redirected' flag to allow new redirections in the next command.
**	- If the token is CMD or ARG:
**		- Extract arguments into the command node.
*/

// when we encounter a pipe, syntax check should ensure
// that there'll always be a token afterwards
// since the first redirect thing we hit will go over
// all the redirections in that subcommand
// we still want to enter this function for every
// token, though - even the redir ones that
// we have already handled
// but we have to make sure that each ARG
// that follows a redirection is properly skipped
// cmd->type END should never be encountered
static bool	process_token(t_shell *shell, t_tok **token, t_cmd **cmd,
		bool *redir)
{
	if ((*token)->is_null)
		return (true);
	if ((*token)->type == PIPE)
	{
		*redir = false;
		*cmd = NULL;
	}
	if ((*token)->type > END && (*token)->type < PIPE)
	{
		printf("redirection for %s\n", (*token)->content);
		if (!(*redir))
		{
			if (!handle_redirection(shell, *token, *cmd))
			{
				(*cmd)->skip = true;
				return (false);
			}
			if ((*token)->type != HEREDOC)
				*redir = true;
		}
		*token = (*token)->next;
		// skip the extra redirection token
	}
	else if ((*token)->type > PIPE && !(*token)->is_null)
	{
		(*cmd)->args[(*cmd)->argc] = safe_strdup(shell, (*token)->content);
		(*cmd)->argc++;
	}
	return (true);
}

/*
**	Iterate through the tokenized input and build a linked list of commands.
**	- First iteration: '!shell->cmd' keeps loop running
**	- Subsequent iterations: loop continues as long as 'current != shell->tokens'
**	- Iterate through tokens, processing each using process_token().
*/

bool	parse_commands(t_shell *shell)
{
	t_tok		*current;
	t_cmd		*cmd;
	bool		redirected;
	static int	i = 0;

	cmd = NULL;
	shell->cmd = NULL;
	current = shell->tokens;
	redirected = false;
	while (!shell->abort)
	{
		if (!cmd)
		{
			cmd = add_cmd(shell, &shell->cmd);
			cmd->argc = count_args(shell, current);
			cmd->args = safe_calloc(shell, cmd->argc + 1, sizeof(char *));
			cmd->argc = 0;
		}
		// into this cmd, until a pipe is encountered
		// if the token is_null, then do not add an arg for it
		// when encountering a pipe in process_token,
		// set this cmd to NULL and let this `if` case
		// init another cmd if there's more tokens
		// after the pipe
		if (!process_token(shell, &current, &cmd, &redirected))
		{
			// a command that's skipped should probably be marked as noop? idk
			// it's already marked as 'skip' anyway so no need
			skip_invalid_command(shell, &current);
			cmd = NULL;
			redirected = false;
		}
		else
			current = current->next;
		if (current == shell->tokens)
			break ;
		if (++i > 50) // debug
			break ;
	}
	return (true);
}
