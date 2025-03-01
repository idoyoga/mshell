/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:06:36 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/01 15:09:59 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
