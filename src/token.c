/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:41:04 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/23 14:58:08 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Handles normal tokens (command names or arguments)	
*/

static bool	handle_normal_token(t_shell *shell, t_tok **lst, char **input)
{
	char		*content;
	int			len;
	bool		is_quoted;

	len = get_token_length(*input, &is_quoted);
	if (len <= 0)
		return (true);
	content = (char *)wrap_malloc(&(shell->alloc_tracker), len + 1);
	if (!content)
		return (error(NULL, NO_MEM, false));
	parse_token(*input, content, len);
	if (!add_token(shell, lst, content, WORD))
		return (false);
	*input += len;
	return (true);
}

/*
**	Handles special tokens (redirections, pipes, etc.)
*/

static bool	handle_special_token(t_shell *shell, t_tok **lst, char **input)
{
	t_t_typ	type;
	t_tok	*prev;
	char	*content;
	int		len;

	len = get_special_length(*input);
	if (len == 0)
		return (false);
	type = get_special_type(*input);
	content = ft_strndup(*input, len);
	if (!content || !alloc_tracker_add(&(shell->alloc_tracker), content, 0))
		return (error(NULL, NO_MEM, false));
	prev = NULL;
	if (*lst)
		prev = (*lst)->prev;
	if (prev && (prev->type == REDIR_OUT || prev->type == REDIR_APPEND
		|| prev->type == REDIR_IN))
		return (error(NULL, CONSEC_REDIR, false));
	if (!add_token(shell, lst, content, type))
		return (false);
	*input += len;
	return (true);
}

/*
**	Handle tokens and categorize them as special (redirections, pipes, etc.)
**	or normal (command names or arguments)
*/

static bool	handle_tokens(t_shell *shell, t_tok **lst, char **input)
{
	char	*end;
	char	*quoted_content;

	if (**input == '\'' || **input == '\"')
	{
		end = handle_quotes(*input);
		if (end > *input)
		{
			quoted_content = ft_strndup(*input + 1, end - *input - 1);
			if (!quoted_content || !add_token(shell, lst, quoted_content, WORD)
				|| alloc_tracker_add(&shell->alloc_tracker, quoted_content, 0))
				return (error(NULL, NO_MEM, false));
			*input = end + 1;
		}
		else
			return (error(NULL, QUOTES, false));
	}
	else if (get_special_length(*input))
		return (handle_special_token(shell, lst, input));
	else if (**input)
		return (handle_normal_token(shell, lst, input));
	return (true);
}

/*
**	Handle consecutive pipes
*/

static bool	handle_pipe(t_tok *prev_token, t_tok **lst)
{
	if (*lst && prev_token && prev_token->type == PIPE
		&& (*lst)->prev->type == PIPE)
	{
		return (error(NULL, CONSEC_PIPES, false));
	}
	return (true);
}

/*
**	Parse the input string and create a list of tokens,
**	checks for open pipes and consecutive pipes
*/

bool	parse_token_list(t_shell *shell, t_tok **lst, char *input)
{
	t_tok	*prev_token;

	if (!lst || !input)
		return (false);
	*lst = NULL;
	prev_token = NULL;
	input = skip_blanks(input);
	if (*input == '|')
		return (error(NULL, START_PIPE, false));
	while (*input)
	{
		input = skip_blanks(input);
		if (!handle_tokens(shell, lst, &input))
			return (false);
		if (!handle_pipe(prev_token, lst))
			return (false);
		if (*lst)
			prev_token = (*lst)->prev; // Update previous token
	}
	if (prev_token && prev_token->type == PIPE)
		return (error(NULL, END_PIPE, false));
	return (true);
}
