/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 21:15:51 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/12 20:12:27 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Add an argument to the command
*/

static void	add_arg(t_shell *shell, char **args, int *i, char *content)
{
	args[*i] = safe_strdup(shell, content);
	if (!args[*i])
		error_exit(shell, NO_MEM, EXIT_FAILURE);
	(*i)++;
}

/*
**	Check if the argument is valid
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
**	Count the number of arguments in the command
*/

static int	count_args(t_shell *shell, t_tok *token)
{
	t_tok	*current;
	int		i;

	i = 0;
	current = token;
	if (current->type == CMD || (current->type == ARG \
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
**	Get the arguments of the command
*/

static char	**populate_args(t_shell *shell, t_tok *token)
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
**	Main command parsing function
*/

bool	parse_commands(t_shell *shell)
{
	t_tok	*current;
	t_cmd	*cmd;

	shell->cmd = NULL;
	current = shell->tokens;
	while (current != shell->tokens || !shell->cmd)
	{
		if (current->type == CMD || (current->type == ARG
				&& current->prev->type == PIPE))
		{
			cmd = add_cmd(shell, &shell->cmd);
			if (!cmd)
				error(NO_MEM, false);
			if (!handle_redirection(shell, current, cmd))
			{
				shell->status = 42;
				return (false);
			}
			cmd->args = populate_args(shell, current);
			if (!cmd->args)
				return (error(NO_MEM, false));
		}
		current = current->next;
	}
	return (true);
}
