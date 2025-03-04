/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:16:18 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 20:16:59 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	quoteless_len(char *token)
{
	size_t	length_without_quotes;
	size_t	i;
	t_qstat	quote;

	length_without_quotes = 0;
	i = 0;
	quote = QUOTE_NONE;
	while (token[i])
	{
		if (quote == QUOTE_NONE && is_quote(token[i]))
			quote = token[i];
		else if (quote != QUOTE_NONE && (unsigned char)token[i] == quote)
			quote = QUOTE_NONE;
		else
			length_without_quotes++;
		i++;
	}
	return (length_without_quotes);
}

static void	strip_quotes_from_token(char *token, char *destination)
{
	size_t	i;
	size_t	ti;
	t_qstat	quote;

	i = 0;
	ti = 0;
	quote = QUOTE_NONE;
	while (token[ti])
	{
		if (quote == QUOTE_NONE && is_quote(token[ti]))
			quote = token[ti];
		else if (quote != QUOTE_NONE && (unsigned char)token[ti] == quote)
			quote = QUOTE_NONE;
		else
		{
			destination[i] = token[ti];
			i++;
		}
		ti++;
	}
	destination[i] = '\0';
}

void	strip_redir_token_quotes(t_shell *shell, t_tok *current_token)
{
	size_t	length_without_quotes;
	char	*new_content;

	if (current_token->content != NULL)
	{
		length_without_quotes = quoteless_len(current_token->content);
		new_content = safe_calloc(shell, length_without_quotes + 1,
				sizeof(char));
		strip_quotes_from_token(current_token->content, new_content);
		current_token->content = new_content;
	}
}
