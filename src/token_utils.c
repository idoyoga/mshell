/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 15:46:52 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/23 14:56:23 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Get the type of a special token
*/

t_t_typ	get_special_type(char *str)
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
	return (WORD);
}

/*
**	Get the length of a special token
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
**	Get the length of the token
*/

int	get_token_length(char *input, bool *is_quoted)
{
	int		len;
	char	current_quote;

	len = 0;
	*is_quoted = false;
	while (input[len] && !get_special_length(input + len)
		&& !ft_isblank(input[len]))
	{
		if (input[len] == '\'' || input[len] == '"')
		{
			*is_quoted = true;
			current_quote = input[len++];
			while (input[len] && input[len] != current_quote)
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
** Parse token content while handling quotes
*/

void	parse_token(char *input, char *content, int len)
{
	char	current_quote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (j < len)
	{
		if (input[j] == '\'' || input[j] == '"')
		{
			current_quote = input[j++];
			while (j < len && input[j] != current_quote)
				content[i++] = input[j++];
			if (j < len)
				j++;
		}
		else
			content[i++] = input[j++];
	}
	content[i] = '\0';
}

/*
**	Add a token node to the token list
*/

bool	add_token(t_shell *shell, t_tok **lst, char *content, t_t_typ type)
{
	t_tok	*node;

	if (!content)
		return (false);
	node = (t_tok *)wrap_malloc(&(shell->alloc_tracker), sizeof(t_tok));
	if (!node)
		return (error(NULL, NO_MEM, false));
	node->content = content;
	node->type = type;
	node->next = NULL;
	node->prev = NULL;
	if (!(*lst))
	{
		(*lst) = node;
		node->prev = node;
		node->next = node;
	}
	else
	{
		node->prev = (*lst)->prev;
		node->next = (*lst);
		(*lst)->prev->next = node;
		(*lst)->prev = node;
	}
	return (true);
}
