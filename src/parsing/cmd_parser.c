/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 21:15:51 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/20 19:45:25 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Count the number of arguments in the command to determine how much memory
**	to allocate for the args array.
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
**	Add an argument to the command's argument list
*/

static void	add_arg(t_shell *shell, char **args, int *i, char *content)
{
	args[*i] = safe_strdup(shell, content);
	if (!args[*i])
		error_exit(shell, NO_MEM, EXIT_FAILURE);
	(*i)++;
}

/*
**	Check if a token is a valid command argument
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
**	Extract arguments from the token list and store them in an array
*/

static char	**extract_args(t_shell *shell, t_tok *token)
{
	t_tok	*current;
	char	**args;
	int		ac;
	int		i;

	ac = count_args(shell, token);
	args = safe_malloc(shell, sizeof(char *) * (ac + 1));
	if (!args)
		error_exit(shell, NO_MEM, EXIT_FAILURE);
	current = token;
	i = 0;
	if (current->type != PIPE && is_valid_arg(shell, current))
		add_arg(shell, args, &i, current->content);
	current = current->next;
	while (current != shell->tokens && current->type != PIPE)
	{
		if (is_valid_arg(shell, current))
			add_arg(shell, args, &i, current->content);
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

/*
**	Convert tokenized input into a command list and handle redirections
**	before execution.
**	- First iteration: !shell->cmd keeps loop running
**	- Subsequent iterations: loop continues as long as current != shell->tokens
**	- Check for invalid redirections
**	- Add commands to the command list
**	- Handle redirections for each command
**	- Extract argumetns into the command structure
*/

bool	parse_commands(t_shell *shell)
{
	t_tok	*current;
	t_cmd	*cmd;

	shell->cmd = NULL;
	current = shell->tokens;
	while (current != shell->tokens || !shell->cmd)
	{
		if (invalid_redirection(current))
			return (error_token(shell, current));
		if (is_command_start(current))
		{
			cmd = add_cmd(shell, &shell->cmd);
			if (!cmd)
				error(NO_MEM, false);
			if (!handle_redirection(shell, current, cmd))
				return (shell->status = 42, false);
			cmd->args = extract_args(shell, current);
			if (!cmd->args)
				return (error(NO_MEM, false));
		}
		current = current->next;
	}
	return (true);
}
