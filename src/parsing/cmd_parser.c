/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 21:15:51 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/25 20:03:57 by dplotzl          ###   ########.fr       */
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
**	  but not if preceded by PIPE at the start of the token list.
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

static bool	process_token(t_shell *shell, t_tok **current, t_cmd **cmd, bool *redirected)
{
	if (!*cmd)
		*cmd = add_cmd(shell, &shell->cmd);
	if (!*redirected && ((*current)->type == REDIR_IN || (*current)->type == HEREDOC
		|| (*current)->type == REDIR_OUT || (*current)->type == REDIR_APPEND))
	{
		if (!handle_redirection(shell, *current, *cmd))
		{
			skip_invalid_command(shell, current);
			return (false);
		}
		if ((*current)->type != HEREDOC)
			*redirected = true;
		*current = (*current)->next;
		return (true);
	}
	if (is_command_start((*current)))
		(*cmd)->args = extract_args(shell, (*current));
	*current = (*current)->next;
	return (true);
}

/*
**	Convert tokenized input into a command list and handle redirections
**	before execution.
**	- First iteration: !shell->cmd keeps loop running
**	- Subsequent iterations: loop continues as long as current != shell->tokens
**	- Check if current token is start of a command
**	- Add commands to the command list
**	- Handle redirections for each command
**	- Extract arguments into the command structure
*/

bool parse_commands(t_shell *shell)
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
    }
    return (true);
}
