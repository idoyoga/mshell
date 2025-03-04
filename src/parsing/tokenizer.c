/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 21:08:39 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 16:08:40 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Check for "\" and ";" in the input string and give an error if found.
**	According to the subject, we don't need to handle them.
*/

static bool	invalid_syntax(char *input)
{
	t_qstat	quote;

	quote = QUOTE_NONE;
	while (*input)
	{
		if (quote == QUOTE_NONE && is_quote(*input))
			quote = *input;
		else if (quote != QUOTE_NONE && (unsigned char)(*input) == quote)
			quote = QUOTE_NONE;
		else if (quote == QUOTE_NONE)
		{
			if (*input == '\\' && (*(input + 1) == '$' || *(input + 1) == '\0'))
				return (error(BACKSLASH, true));
			if (*input == ';' && (*(input + 1) == ' ' || *(input + 1) == '\0'))
				return (error(SEMICOLON, true));
		}
		input++;
	}
	return (false);
}

/*
**	Update the state of the quotes. If a single quote is found (and not
**	inside a double quote), toggle s_quote. If a double quote is found
**	(and is not inside a single quote), toggle d_quote.
*/

void	update_quote_state(bool *d_quote, bool *s_quote, char c)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '"' && !*s_quote)
		*d_quote = !*d_quote;
}

/*
**	Check for unclosed quotes in the input string.
*/

static bool	check_unclosed_quotes(t_shell *shell, const char *input)
{
	bool	d_quote;
	bool	s_quote;
	int		i;

	i = -1;
	d_quote = false;
	s_quote = false;
	while (input[++i])
		update_quote_state(&d_quote, &s_quote, input[i]);
	if (d_quote || s_quote)
		return (shell->status = 42, error(QUOTES, true));
	return (false);
}

/*
**	Check first for invalid syntax and unclosed quotes,
**	then handle environment variable expansion, tokenize the input
**	and parse the commands.
*/
bool	tokenize_input(t_shell *shell, char *input)
{
	input = shell->cmd_input;
	if (invalid_syntax(input) || check_unclosed_quotes(shell, input))
		return (shell->status = 2, false);
	if (!tokenize(shell, &shell->tokens, input))
		return (shell->status = 2, false);
	if (!expand_dilla_variables(shell))
		return (shell->status = 2, false);
	if (invalid_redirection(shell->tokens))
		return (shell->status = 2, error_token(shell, shell->tokens));
	if (validate_tokens(shell->tokens))
		return (shell->status = 2, false);
	if (!parse_commands(shell))
		return (false);
	return (true);
}
