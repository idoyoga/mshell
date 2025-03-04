/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:41:04 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 12:47:55 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Parse a word token, trim quotes and assign a token type
**	This function processes a word token from the input string:
**	1. Determines the token's length and counts quotes.
**	2. If trimming the quotes results in an empty token, exit the function.
**	3. Calls 'determine_token_type()' to classify the token as CMD or ARG.
**	4. Adds the token to the token list.
**	5. Advances the 'input' pointer to the next token.
*/

static bool	parse_word_token(t_shell *shell, t_tok **lst, char **input,
		int token_length)
{
	t_t_typ	token_type;
	char	*content;

	content = safe_strndup(shell, *input, token_length);
	token_type = determine_token_type(lst);
	if (!add_token(shell, lst, content, token_type))
		return (false);
	*input += token_length;
	return (true);
}

/*
**	Identify and parse special tokens (operators)
**	1. Get the token length using 'get_special_length()'.
**	2. Use 'identify_special_token()' to classify the operator type.
**	3. Duplicate the token content and registers it in 'alloc_tracker'.
**	4. Prevent consecutive redirections (e.g., 'echo > < file').
**	5. Add the token to the list and advances the input pointer.
*/

static bool	parse_operator_token(t_shell *shell, t_tok **lst, char **input,
		int token_length)
{
	t_t_typ	type;
	t_tok	*prev;
	char	*content;

	type = identify_special_token(*input);
	content = ft_strndup(*input, token_length);
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
	*input += token_length;
	return (true);
}

/*
**	Extract the content of the token and add it to the token list
**	- If the input is a special operator, 'parse_operator_token()' is called.
**	- Otherwise, 'parse_word_token()' is called.
*/

static bool	extract_token_content(t_shell *shell, t_tok **lst, char **input)
{
	size_t	special_token_length;
	size_t	word_token_length;

	if (!input || !*input)
		return (false);
	special_token_length = get_special_length(*input);
	if (special_token_length > 0)
		return (parse_operator_token(shell, lst, input, special_token_length));
	word_token_length = get_token_length(*input);
	return (parse_word_token(shell, lst, input, word_token_length));
}

/*
**	Core function to tokenize user input in a structured token list.
*	1. Check for invalid starting pipe.
**	2. Skip leading whitespace.
**	3. Call 'extract_token_content()' to extract tokens.
**	4. Call 'validate_pipe_syntax()' to check for invalid consecutive pipes.
**	5. Update 'prev_token' to track the last processed token.
**	6. Ensure that the input does not end with a pipe.
*/

bool	tokenize(t_shell *shell, t_tok **lst, char *input)
{
	if (!lst || !input)
		return (false);
	*lst = NULL;
	while (*input)
	{
		while (ft_isblank(*input))
			input++;
		if (!*input)
			break ;
		if (!extract_token_content(shell, lst, &input))
			return (false);
		if ((*lst)->type == CMD)
			(*lst)->first_cmd = true;
		// TODO: never true? because we always check first token
		if ((*lst)->type == PIPE)
			(*lst)->first_cmd = false;
	}
	if ((*lst)->prev && (*lst)->prev->type == PIPE)
		return (error(END_PIPE, false));
	return (true);
}
