/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 19:35:53 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/03 13:50:26 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quote(char c)
{
	return (c == QUOTE_SINGLE || c == QUOTE_DOUBLE);
}

static size_t	get_length_without_quotes(char *token)
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
		else if (quote != QUOTE_NONE && token[i] == (unsigned char)quote)
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
		else if (quote != QUOTE_NONE && token[ti] == (unsigned char)quote)
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

bool	remove_quotes(t_shell *shell)
{
	t_tok	*current;
	size_t	length_without_quotes;
	char	*new_content;

	current = shell->tokens;
	while (current)
	{
		if (current->content != NULL)
		{
			length_without_quotes = get_length_without_quotes(current->content);
			new_content = safe_calloc(shell, length_without_quotes + 1,
					sizeof(char));
			strip_quotes_from_token(current->content, new_content);
			current->content = new_content;
		}
		current = current->next;
		if (current == shell->tokens)
			break ;
	}
	return (true);
}
