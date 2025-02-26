/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 21:15:51 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/26 02:26:45 by xgossing         ###   ########.fr       */
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
	if (current->type == CMD || (current->type == ARG
			&& current->prev->type != PIPE))
		i++;
	current = current->next;
	while (current != shell->tokens && current->type != PIPE)
	{
		if (current->type == CMD || current->type == ARG)
			i++;
		current = current->next;
	}
	return (i);
}

/*
**	Check if a token is a valid argument for a command:
**	- CMD tokens are always valid arguments.
**	- ARG tokens are valid if preceded by CMD or another ARG,
**		but not if preceded by PIPE at the start of the token list.
*/

static bool	is_valid_arg(t_shell *shell, t_tok *current)
{
	if (current->type == CMD)
		return (true);
	if (current->type == ARG && (current->prev->type == CMD
			|| current->prev->type == ARG)
		&& current->prev != shell->tokens->prev)
		return (true);
	return (false);
}

/*
**	Extract arguments from tokens into an array:
**	- Allocate memory for the array based on the argument count.
**	- Iterate through tokens and adds valid arguments.
**	- End the array with NULL for execve compatibility.
*/

static char	**extract_args(t_shell *shell, t_tok *token)
{
	t_tok	*current;
	char	**args;
	int		ac;
	int		i;

	ac = count_args(shell, token);
	args = safe_malloc(shell, sizeof(char *) * (ac + 1));
	current = token;
	i = 0;
	if (current->type != PIPE && is_valid_arg(shell, current))
		args[i++] = safe_strdup(shell, current->content);
	current = current->next;
	while (current != shell->tokens && current->type != PIPE)
	{
		if (is_valid_arg(shell, current))
			args[i++] = safe_strdup(shell, current->content);
		current = current->next;
	}
	args[i] = NULL;
	return (args);
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

static bool	process_token(t_shell *shell, t_tok **current, t_cmd **cmd,
		bool *redir)
{
	if (!*cmd)
		*cmd = add_cmd(shell, &shell->cmd);
	if (!*redir && ((*current)->type == REDIR_IN || (*current)->type == HEREDOC
			|| (*current)->type == REDIR_OUT
			|| (*current)->type == REDIR_APPEND))
	{
		if (!handle_redirection(shell, *current, *cmd))
		{
			skip_invalid_command(shell, current);
			return (false);
		}
		if ((*current)->type != HEREDOC)
			*redir = true;
		*current = (*current)->next;
		return (true);
	}
	if ((*current)->type == PIPE)
	{
		(*cmd)->next = add_cmd(shell, &shell->cmd);
		*redir = false;
	}
	else if (is_command_start((*current)))
		(*cmd)->args = extract_args(shell, (*current));
	*current = (*current)->next;
	return (true);
}

/*
**	Iterate through the tokenized input and build a linked list of commands.
**	- First iteration: '!shell->cmd' keeps loop running
**	- Subsequent iterations: loop continues as long as 'current != shell->tokens'
**	- Iterate through tokens, processing each using process_token().
**	- If a pipe is encountered:
**		- Add a new command node for the next piped command.
**		- Reset the redirection flag.
**		- Move to the next token.
*/

bool	parse_commands(t_shell *shell)
{
	t_tok	*current;
	t_cmd	*cmd;
	bool	redirected;

	cmd = NULL;
	shell->cmd = NULL;
	current = shell->tokens;
	redirected = false;
	while (current != shell->tokens || !shell->cmd)
	{
		if (!process_token(shell, &current, &cmd, &redirected))
			continue ;
		if (current && current->type == PIPE)
		{
			cmd = add_cmd(shell, &shell->cmd);
			redirected = false;
			current = current->next;
		}
	}
	return (true);
}
