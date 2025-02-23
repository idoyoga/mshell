/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:52 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/23 09:29:35 by dplotzl          ###   ########.fr       */
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
	if (!ft_strncmp(str, "<", 1) || !ft_strncmp(str, ">", 1)
		|| !ft_strncmp(str, "|", 1))
		return (1);
	return (0);
}

/*
**	Get length of a token while handling quotes
*/

int	get_token_length(char *input, int *quote)
{
	int		len;

	len = 0;
	*quote = 0;
	while (input[len] && !get_special_length(input + len)
		&& !ft_isblank(input[len]))
	{
		if (input[len] == '\'' || input[len] == '"')
		{
			(*quote)++;
			if (input[len++] == '"')
				while (input[len] && input[len] != '"')
					len++;
			else
				while (input[len] && input[len] != '\'')
					len++;
			if (input[len])
				len++;
		}
		else
			len++;
	}
	return (len);
}

/*
**	Allocate and initialise a new token node
*/

static	t_tok	*init_token(t_shell *shell, char *content, t_t_typ type)
{
	t_tok	*token;

	token = safe_malloc(shell, sizeof(t_tok));
	if (!token)
		error_exit(shell, NO_MEM, "init_token", EXIT_FAILURE);
	token->content = content;
	token->file = NULL;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
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
