/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 21:08:39 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/23 09:44:45 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Check for "\" and ";" in the input string and give an error if found.
**	According to the subject, we don't need to handle them.
*/

static bool	invalid_syntax(char *input)
{
	while (*input)
	{
		if (*input == '\\' && (*(input + 1) == '$' || *(input + 1) == '\0'))
			return (error(BACKSLASH, true));
		if (*input == ';' && (*(input +1) == ' ' || *(input + 1) == '\0'))
			return (error(SEMICOLON, true));
		input++;
	}
	return (false);
}

/*
**	Remove quotes from a string while preserving the content.
*/

char	*trim_quotes(t_shell *shell, char *input, int len)
{
	char	*content;
	char	quote;
	int		i;
	int		j;

	content = safe_malloc(shell, len + 1);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i++];
			while (i < len && input[i] != quote)
				content[j++] = input[i++];
			if (i < len)
				i++;
		}
		else
			content[j++] = input[i++];
	}
	content[j] = '\0';
	return (content);
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
		return (false);
	if (!expand_dollar_variables(shell, &input))
		return (false);
	if (!tokenize(shell, &shell->tokens, input))
		return (false);
	if (invalid_redirection(shell->tokens))
		return (error_token(shell, shell->tokens));
	if (!parse_commands(shell))
		return (false);
	return (true);
}
