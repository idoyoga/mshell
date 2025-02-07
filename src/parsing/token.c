/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:41:04 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/07 14:59:43 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Parse a word token (CMD or ARG) and add it to the token list
*/

static bool	parse_word_token(t_shell *shell, t_tok **lst, char **input)
{
	t_t_typ	token_type;
	t_tok	*prev_token;
	char	*content;
	int		quote_count;
	int		len;

	len = get_token_length(*input, &quote_count);
	if ((len - (2 * quote_count)) < 0)
		return (true);
	content = trim_quotes(shell, *input, len);
	if (!content)
		error_exit(shell, NO_MEM, EXIT_FAILURE);
	prev_token = NULL;
	if (*lst)
		prev_token = (*lst)->prev;
	if (!prev_token || prev_token->type == PIPE)
		token_type = CMD;
	else
		token_type = ARG;
	if (!add_token(shell, lst, content, token_type))
		return (false);
	*input += len;
	return (true);
}

/*
**	Identify and parse special tokens (operators)
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
**	Tokenize the input string and create a linked list of tokens,
**	checking for pipe syntax errors
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
