/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:52 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/02 22:10:43 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Identify type of special tokens (operators)
*/

t_t_typ	identify_special_token(char *str)
{
	if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	if (!ft_strncmp(str, ">>", 2))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<", 1))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUT);
	if (!ft_strncmp(str, "|", 1))
		return (PIPE);
	return (END);
}

/*
**	Get length of special tokens (operators)
*/

int	get_special_length(char *str)
{
	if (!ft_strncmp(str, "<<", 2) || !ft_strncmp(str, ">>", 2))
		return (2);
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1) || !ft_strncmp(str,
			"|", 1))
		return (1);
	return (0);
}

/*
**	Get length of a token while handling quotes
*/

int	get_token_length(char *input)
{
	int		len;
	t_qstat	quote;

	quote = QUOTE_NONE;
	len = 0;
	while (input[len])
	{
		if (quote == QUOTE_NONE && ft_isblank(input[len]))
			break ;
		if (quote == QUOTE_NONE && get_special_length(input + len) != 0)
			break ;
		if (quote == QUOTE_NONE && is_quote(input[len]))
			quote = input[len];
		else if (quote != QUOTE_NONE && (input[len]) == quote)
			quote = QUOTE_NONE;
		len++;
	}
	return (len);
}

/*
**	Allocate and initialise a new token node
*/

static t_tok	*init_token(t_shell *shell, char *content, t_t_typ type)
{
	t_tok	*token;

	token = safe_malloc(shell, sizeof(t_tok));
	if (!token)
		error_exit(shell, NO_MEM, "init_token", EXIT_FAILURE);
	token->content = content;
	token->type = type;
	token->is_quoted = false;
	token->first_cmd = false;
	token->next = NULL;
	token->prev = NULL;
	if (ft_strchr(content, '"') != NULL || ft_strchr(content, '\'') != NULL)
		token->is_quoted = true;
	return (token);
}

/*
**	Add a new token node to the end of the list
*/

t_tok	*add_token(t_shell *shell, t_tok **lst, char *content, t_t_typ type)
{
	t_tok	*token;

	token = init_token(shell, content, type);
	if (!(*lst))
	{
		(*lst) = token;
		token->prev = token;
		token->next = token;
	}
	else
	{
		token->prev = (*lst)->prev;
		token->next = (*lst);
		(*lst)->prev->next = token;
		(*lst)->prev = token;
	}
	return (token);
}
