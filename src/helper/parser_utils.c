/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:06:36 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 19:16:48 by dplotzl          ###   ########.fr       */
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

static int	invalid_pipes(t_tok *token)
{
	if (token->type == PIPE)
	{
		if (token->prev && token->prev->type == PIPE)
			return (error(END_PIPE, 2));
		else if (token->next->type == PIPE)
			return (error(CONSEC_PIPES, 2));
	}
	return (0);
}

int	validate_tokens(t_tok *token)
{
	t_tok	*current;

	current = token;
	if (current->type == PIPE)
		return (error(START_PIPE, 2));
	while (current)
	{
		if (invalid_pipes(current))
			return (2);
		current = current->next;
		if (current == token)
			break ;
	}
	return (0);
}
