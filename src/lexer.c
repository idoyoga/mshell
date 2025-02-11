/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 21:08:39 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/23 14:54:15 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	validate_syntax(char *input)
{
	while (*input)
	{
		if (*input == '\\' && (*(input + 1) == '$' || *(input + 1) == '\0'))
			return (error(NULL, BACKSLASH, false));
		if (*input == ';' && (*(input +1) == ' ' || *(input + 1) == '\0'))
			return (error(NULL, SEMICOLON, false));
		input++;
	}
	return (true);
}

/*
**	Check if there are open quotes in the cmd_input
**	If there are open quotes, print an error message,
**	set status to 2 and return false
*/

static bool	is_open_quote(t_shell *shell, char *input)
{
	bool	dquote;
	bool	squote;

	dquote = false;
	squote = false;
	while (*input)
	{
		if (*input == '\"' && !dquote)
			squote = !squote;
		else if (*input == '\'' && !squote)
			dquote = !dquote;
		input++;
	}
	if (dquote || squote)
	{
		error(NULL, QUOTES, 0);
		shell->status = 2;
		return (true);
	}
	return (false);
}

/*
**	Lexer function
**	1. Check for open QUOTES
**	2. Replace environment variables
**	3. Tokenization
**	4. Debugging: Print token list
*/

bool	lexer(t_shell *shell, char *input)
{
	input = shell->cmd_input;
	if (is_open_quote(shell, input))
		return (false);
	if (!validate_syntax(input))
		return (false);
	if (!parse_token_list(shell, &shell->tokens, input))
		return (false);
	return (true);
}
