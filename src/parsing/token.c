/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:41:04 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/22 21:29:10 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Parse a word token, trim quotes and assign a token type
**	This function processes a word token from the input string:
**	1. Determines the token's length and counts quotes.
**	2. If trimming the quotes results in an empty token, the function exits early.
**	3. Calls `determine_token_type()` to classify the token as `CMD` or `ARG`.
**	4. Adds the token to the token list.
**	5. Advances the `input` pointer to the next token.
*/

static bool	parse_word_token(t_shell *shell, t_tok **lst, char **input)
{
	t_t_typ	token_type;
	char	*content;
	int		quote_count;
	int		len;

	len = get_token_length(*input, &quote_count);
	if ((len - (2 * quote_count)) < 0)
		return (true);
	content = trim_quotes(shell, *input, len);
	token_type = determine_token_type(lst);
	if (!add_token(shell, lst, content, token_type))
		return (false);
	*input += len;
	return (true);
}

/*
**	Identify and parse special tokens (operators)
**	1. Gets the token length using `get_special_length()`.
**	2. Uses `identify_special_token()` to classify the operator type.
**	3. Duplicates the token content and registers it in `alloc_tracker`.
**	4. Prevents consecutive redirections (e.g., `echo > < file`).
**	5. Adds the token to the list and advances the input pointer.
*/

static bool	parse_operator_token(t_shell *shell, t_tok **lst, char **input)
{
	t_t_typ	type;
	t_tok	*prev;
	char	*content;
	int		len;

	len = get_special_length(*input);
	if (len == 0)
		return (false);
	type = identify_special_token(*input);
	content = ft_strndup(*input, len);
	if (!content || !alloc_tracker_add(&(shell->alloc_tracker), content, 0))
		return (error(NO_MEM, false));
	prev = NULL;
	if (*lst)
		prev = (*lst)->prev;
	if (prev && (prev->type == REDIR_OUT || prev->type == REDIR_APPEND
			|| prev->type == REDIR_IN))
		return (error(CONSEC_REDIR, false));
	if (!add_token(shell, lst, content, type))
		return (false);
	*input += len;
	return (true);
}

/*
**	Extract the content of the token and add it to the token list
**	- If the input starts with a special operator, `parse_operator_token()` is called.
**	- Otherwise, `parse_word_token()` is called.
*/

static bool	extract_token_content(t_shell *shell, t_tok **lst, char **input)
{
	if (!input || !*input)
		return (false);
	if (get_special_length(*input))
		return (parse_operator_token(shell, lst, input));
	if (!parse_word_token(shell, lst, input))
		return (false);
	return (true);
}

/*
**	Handle consecutive pipes
*/

static bool	validate_pipe_syntax(t_tok *prev_token, t_tok **lst)
{
	if (*lst && prev_token && prev_token->type == PIPE
		&& (*lst)->prev->type == PIPE)
	{
		return (error(CONSEC_PIPES, false));
	}
	return (true);
}

/*
**	Core function to tokenize user input in a structured token list.
*	1. Check for invalid starting `PIPE`.
**	2. Skips leading whitespace.
**	3. Calls `extract_token_content()` to extract tokens.
**	4. Calls `validate_pipe_syntax()` to check for invalid consecutive pipes.
**	5. Updates `prev_token` to track the last processed token.
**	6. Ensures that the input does not end with a `PIPE`, returning an error if it does.
*/

bool	tokenize(t_shell *shell, t_tok **lst, char *input)
{
	t_tok	*prev_token;

	if (!lst || !input)
		return (false);
	*lst = NULL;
	prev_token = NULL;
	if (*input == '|')
		return (error(START_PIPE, false));
	while (*input)
	{
		while (ft_isblank(*input))
			input++;
		if (!*input)
			break ;
		if (!extract_token_content(shell, lst, &input))
			return (false);
		if (!validate_pipe_syntax(prev_token, lst))
			return (false);
		if (*lst)
			prev_token = (*lst)->prev;
	}
	if (prev_token && prev_token->type == PIPE)
		return (error(END_PIPE, false));
	return (true);
}
